/*
 * D-ConCReCT Distributed Client - Ported to Contiki-NG
 * Original: Borgiani et al. (2021) - vspnet/ConCReCT
 * Ported by: Baibhav Chowdhury (24232033), NCI MSc Cybersecurity
 *
 * Each node acts as BOTH a client (sends to root) AND a parent
 * (monitors its own children). This is the distributed design.
 *
 * Normal send interval: 60 seconds
 * Muted send interval:  240 seconds (penalty)
 * Attack mode:          1 second (simulated by button or flag)
 */

#include "contiki.h"
#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip-ds6.h"
#include "net/routing/rpl-lite/rpl.h"
#include "sys/etimer.h"
#include "sys/log.h"
#include "lib/random.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOG_MODULE "DConCReCT-C"
#define LOG_LEVEL LOG_LEVEL_INFO

/* Port definitions */
#define UDP_CLIENT_PORT 5000
#define UDP_SERVER_PORT 5678

/* Timing - matching original D-ConCReCT */
#define PERIOD_NORMAL   60    /* seconds between sends - normal mode */
#define PERIOD_MUTED    240   /* seconds between sends - muted/penalty mode */
#define PERIOD_ATTACK   1     /* seconds between sends - attack simulation */
#define PERIOD_MAT      180   /* seconds before warning matrix reset */

/* Matrix - each intermediate node also monitors its own children */
#define L 20
#define NCOLS 4
#define COL_ID      0
#define COL_TMN     1
#define COL_AVISO   2
#define COL_PENAL   3
#define AVISO_MAX   3

/* State variables */
static struct simple_udp_connection udp_conn;   /* upward to root/server */
static struct simple_udp_connection parent_conn; /* lateral to parent node */
static int mute = 0;           /* 0=normal, 1=just muted, 2=sleeping */
static int filhosMeu = 0;      /* count of MY children */
static int MatP[L][NCOLS];         /* matrix for monitoring MY children */
static uint32_t tx_count = 0;

/*---------------------------------------------------------------------------*/
static void criaM(int Mat[L][NCOLS])
{
  int i, j;
  for(i = 0; i < L; i++)
    for(j = 0; j < C; j++)
      Mat[i][j] = 0;
}

static void limpaM(int Mat[L][NCOLS])
{
  int i;
  for(i = 0; i < L; i++) {
    Mat[i][COL_AVISO] = 0;
    Mat[i][COL_TMN]   = 0;
  }
}

static void DesmutaNo(int Mat[L][NCOLS], int id)
{
  int i;
  for(i = 0; i < L; i++) {
    if(Mat[i][COL_ID] == id) {
      Mat[i][COL_PENAL] = 0;
      LOG_INFO("Child node %d unmuted\n", id);
    }
  }
}

static float Limitetrafego(int f)
{
  float f2 = (float)f * f;
  float f3 = f2 * f;
  return 0.295f + 2.94f * f - 0.056f * f2 + 0.0006f * f3;
}

static int consultaM(int Mat[L][NCOLS], int id, int f)
{
  int i;
  float TEN = Limitetrafego(f + 1);

  for(i = 0; i < L; i++) {
    if(Mat[i][COL_ID] == 0) {
      Mat[i][COL_ID]  = id;
      Mat[i][COL_TMN] = 1;
      filhosMeu++;
      return 0;
    }
    if(Mat[i][COL_ID] == id) {
      Mat[i][COL_TMN]++;
      if((float)Mat[i][COL_TMN] > TEN) {
        Mat[i][COL_AVISO]++;
        if(Mat[i][COL_AVISO] > AVISO_MAX) {
          LOG_INFO("Child %d PENALISED | Warnings=%d TMN=%d\n",
                   id, Mat[i][COL_AVISO], Mat[i][COL_TMN]);
          Mat[i][COL_AVISO] = 0;
          Mat[i][COL_TMN]   = 0;
          Mat[i][COL_PENAL] = 1;
          return 1;
        } else {
          Mat[i][COL_TMN] = 0;
          return 0;
        }
      } else {
        if(Mat[i][COL_PENAL] != 1) Mat[i][COL_PENAL] = 0;
        return 0;
      }
    }
  }
  return 0;
}

/*---------------------------------------------------------------------------*/
/*
 * Receive callback - handles messages from:
 *   1. Root/server: "wait" (we are muted) or "ok"
 *   2. Our own children: their traffic reports (we monitor them)
 */
static void
udp_rx_callback(struct simple_udp_connection *c,
                const uip_ipaddr_t *sender_addr,
                uint16_t sender_port,
                const uip_ipaddr_t *receiver_addr,
                uint16_t receiver_port,
                const uint8_t *data,
                uint16_t datalen)
{
  char buf[32];
  int sender_id;
  int f = 0;
  int mute_child;

  if(datalen >= sizeof(buf)) datalen = sizeof(buf) - 1;
  memcpy(buf, data, datalen);
  buf[datalen] = '\0';

  sender_id = sender_addr->u8[15];

  /* Message from parent/root: we are being muted */
  if(strcmp(buf, "wait") == 0 && mute == 0) {
    LOG_INFO("MUTED by parent node %d\n", sender_id);
    mute = 1;
    return;
  }

  /* "awake" from a child that has woken up */
  if(strcmp(buf, "awake") == 0) {
    DesmutaNo(MatP, sender_id);
    return;
  }

  /* Otherwise it is a traffic report from one of our children */
  f = atoi(buf);
  mute_child = consultaM(MatP, sender_id, f);

  if(mute_child == 1) {
    /* Forward "wait" to the offending child */
    LOG_INFO("Forwarding WAIT to child node %d\n", sender_id);
    simple_udp_sendto(&parent_conn, "wait", 4, sender_addr);
  }
}

/*---------------------------------------------------------------------------*/
PROCESS(udp_client_process, "D-ConCReCT Client");
AUTOSTART_PROCESSES(&udp_client_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(udp_client_process, ev, data)
{
  static struct etimer send_timer;
  static struct etimer mat_timer;
  static int send_interval = 1;
  char buf[32];
  uip_ipaddr_t dest_ipaddr;

  PROCESS_BEGIN();

  LOG_INFO("D-ConCReCT Client starting...\n");

  criaM(MatP);

  /* Register connection to send upward to root */
  simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL,
                      UDP_SERVER_PORT, udp_rx_callback);

  /* Register connection to listen for children */
  simple_udp_register(&parent_conn, UDP_SERVER_PORT, NULL,
                      UDP_CLIENT_PORT, udp_rx_callback);

  /* Random initial backoff to avoid simultaneous starts */
  etimer_set(&send_timer, random_rand() % (PERIOD_NORMAL * CLOCK_SECOND));
  etimer_set(&mat_timer, PERIOD_MAT * CLOCK_SECOND);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&send_timer) ||
                             etimer_expired(&mat_timer));

    /* Reset warning matrix periodically */
    if(etimer_expired(&mat_timer)) {
      limpaM(MatP);
      LOG_INFO("Child matrix reset\n");
      etimer_reset(&mat_timer);
    }

    /* Send traffic report upward */
    if(etimer_expired(&send_timer)) {

      if(mute == 2) {
        /* Just woken from mute - send "awake" to parent */
        if(NETSTACK_ROUTING.node_is_reachable() &&
           NETSTACK_ROUTING.get_root_ipaddr(&dest_ipaddr)) {
          LOG_INFO("Sending AWAKE after mute period\n");
          simple_udp_sendto(&udp_conn, "awake", 5, &dest_ipaddr);
        }
        mute = 0;
        send_interval = PERIOD_NORMAL;
      }

      if(mute == 1) {
        /* Enter muted sleep period */
        send_interval = PERIOD_MUTED;
        mute = 2;
      }

      if(mute == 0) {
        /* Normal operation - send traffic report to root */
        if(NETSTACK_ROUTING.node_is_reachable() &&
           NETSTACK_ROUTING.get_root_ipaddr(&dest_ipaddr)) {

          /* Message format: "<num_children>:Nf - Hello <seq>" */
          snprintf(buf, sizeof(buf), "%d:Nf - Hello %lu",
                   filhosMeu, (unsigned long)tx_count);
          tx_count++;

          LOG_INFO("Sending '%s' to root (children=%d)\n", buf, filhosMeu);
          simple_udp_sendto(&udp_conn, buf, strlen(buf), &dest_ipaddr);
        } else {
          LOG_INFO("Not reachable yet - waiting for RPL\n");
        }
      }

      etimer_set(&send_timer, send_interval * CLOCK_SECOND);
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
