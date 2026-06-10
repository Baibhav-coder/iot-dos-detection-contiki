/*
 * D-ConCReCT Distributed Server - Ported to Contiki-NG
 * Original: Borgiani et al. (2021) - vspnet/ConCReCT
 * Ported by: Baibhav Chowdhury (24232033), NCI MSc Cybersecurity
 *
 * Every parent node monitors its own child nodes.
 * If a child exceeds the traffic threshold 3 times, it is muted
 * by sending a "wait" message (duty-cycle throttling).
 *
 * Contiki 2.7 APIs replaced with Contiki-NG equivalents:
 *   - uip_newdata() / uip_udp_packet_send() -> simple_udp callbacks
 *   - rpl_set_root() -> NETSTACK_ROUTING.root_start()
 *   - NETSTACK_MAC.off(1) -> removed (not needed in Contiki-NG)
 *   - uip_ds6_nbr_cache -> rpl_nbr APIs
 */

#include "contiki.h"
#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip-ds6.h"
#include "net/routing/rpl-lite/rpl.h"
#include "sys/etimer.h"
#include "sys/log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOG_MODULE "DConCReCT-S"
#define LOG_LEVEL LOG_LEVEL_INFO

/* Port definitions - same as original */
#define UDP_CLIENT_PORT 8765
#define UDP_SERVER_PORT 5678

/* Matrix dimensions - same as original D-ConCReCT */
#define L 20        /* max 20 children per parent node */
#define NCOLS 4         /* columns: [id, TMN, warnings, muted] */

/* Column index definitions - same as original */
#define COL_ID      0
#define COL_TMN     1   /* Traffic Measured at Node */
#define COL_AVISO   2   /* Warning counter */
#define COL_PENAL   3   /* Penalty/mute flag */

/* Detection parameters - same as original */
#define AVISO_MAX   3   /* max warnings before muting */
#define PERIOD      180 /* seconds before warning reset */

/* UDP connection handle */
static struct simple_udp_connection udp_conn;

/* The child monitoring matrix - core D-ConCReCT data structure */
static int MatP[L][NCOLS];

/*---------------------------------------------------------------------------*/
/* Matrix initialisation - zero all entries */
static void
criaM(int Mat[L][NCOLS])
{
  int i, j;
  for(i = 0; i < L; i++)
    for(j = 0; j < C; j++)
      Mat[i][j] = 0;
}

/*---------------------------------------------------------------------------*/
/* Reset TMN and warnings after each PERIOD - not the mute flag */
static void
limpaM(int Mat[L][NCOLS])
{
  int i;
  for(i = 0; i < L; i++) {
    Mat[i][COL_AVISO] = 0;
    Mat[i][COL_TMN]   = 0;
  }
}

/*---------------------------------------------------------------------------*/
/* Remove mute from a node that has sent "awake" */
static void
DesmutaNo(int Mat[L][NCOLS], int id)
{
  int i;
  for(i = 0; i < L; i++) {
    if(Mat[i][COL_ID] == id) {
      Mat[i][COL_PENAL] = 0;
      LOG_INFO("Node %d unmuted (awake received)\n", id);
    }
  }
}

/*---------------------------------------------------------------------------*/
/* Print current matrix state */
static void
imprimeM(int Mat[L][NCOLS])
{
  int i;
  LOG_INFO("--- Child Matrix ---\n");
  for(i = 0; i < L; i++) {
    if(Mat[i][COL_ID] == 0) break;
    if(Mat[i][COL_PENAL] == 1)
      LOG_INFO("  Node %d: MUTED (suspected attacker)\n", Mat[i][COL_ID]);
    else
      LOG_INFO("  Node %d: Active | TMN=%d Warnings=%d\n",
               Mat[i][COL_ID], Mat[i][COL_TMN], Mat[i][COL_AVISO]);
  }
}

/*---------------------------------------------------------------------------*/
/*
 * STATIC THRESHOLD FORMULA - original D-ConCReCT polynomial
 * f = number of children the sender has
 * TEN = Traffic Expected at Node
 * This is what the Dynamic Threshold module will REPLACE in Phase 2.
 */
static float
Limitetrafego(int f)
{
  float f2 = (float)f * f;
  float f3 = f2 * f;
  return 0.295f + 2.94f * f - 0.056f * f2 + 0.0006f * f3;
}

/*---------------------------------------------------------------------------*/
/*
 * Core detection function - called for every packet received from a child.
 * Returns 1 if node should be muted, 0 otherwise.
 */
static int
consultaM(int Mat[L][NCOLS], int id, int f)
{
  int i;
  int TEN = (int)Limitetrafego(f + 1);

  for(i = 0; i < L; i++) {

    /* Empty slot - register new child */
    if(Mat[i][COL_ID] == 0) {
      Mat[i][COL_ID]  = id;
      Mat[i][COL_TMN] = 1;
      LOG_INFO("New child registered: Node %d\n", id);
      return 0;
    }

    /* Found the child's row */
    if(Mat[i][COL_ID] == id) {
      Mat[i][COL_TMN]++;

      if(Mat[i][COL_TMN] > (int)TEN) {
        Mat[i][COL_AVISO]++;

        if(Mat[i][COL_AVISO] > AVISO_MAX) {
          /* Threshold exceeded too many times - MUTE the node */
          LOG_INFO("Node %d PENALISED | Warnings=%d TMN=%d\n",
                   id, Mat[i][COL_AVISO], Mat[i][COL_TMN]);
          Mat[i][COL_AVISO] = 0;
          Mat[i][COL_TMN]   = 0;
          Mat[i][COL_PENAL] = 1;
          return 1;
        } else {
          /* Warning issued but not yet muted */
          LOG_INFO("Node %d WARNING %d | TMN=%d TEN=%d\n",
                   id, Mat[i][COL_AVISO], Mat[i][COL_TMN], TEN);
          Mat[i][COL_TMN] = 0;
          return 0;
        }
      } else {
        /* Traffic within threshold */
        if(Mat[i][COL_PENAL] != 1)
          Mat[i][COL_PENAL] = 0;
        return 0;
      }
    }
  }
  return 0;
}

/*---------------------------------------------------------------------------*/
/*
 * UDP receive callback - Contiki-NG simple_udp API
 * Replaces tcpip_handler() from original Contiki 2.7 code
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
  int mute_node;

  /* Copy data safely */
  if(datalen >= sizeof(buf)) datalen = sizeof(buf) - 1;
  memcpy(buf, data, datalen);
  buf[datalen] = '\0';

  /* Get last byte of sender IPv6 address as node ID */
  sender_id = sender_addr->u8[15];

  LOG_INFO("Received '%s' from node %d\n", buf, sender_id);

  /* Handle "awake" message - node has woken from mute */
  if(strcmp(buf, "awake") == 0) {
    DesmutaNo(MatP, sender_id);
    return;
  }

  /* Extract number of children from message prefix (e.g. "3:Nf - Hello 1") */
  f = atoi(buf);

  /* Run detection */
  mute_node = consultaM(MatP, sender_id, f);

  if(mute_node == 1) {
    /* Send "wait" to throttle the offending child */
    LOG_INFO("Sending WAIT to node %d\n", sender_id);
    simple_udp_sendto(&udp_conn, "wait", 4, sender_addr);
  }
  /* If OK, no reply needed (matches original behaviour) */
}

/*---------------------------------------------------------------------------*/
PROCESS(udp_server_process, "D-ConCReCT Server");
AUTOSTART_PROCESSES(&udp_server_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(udp_server_process, ev, data)
{
  static struct etimer periodic_timer;

  PROCESS_BEGIN();

  LOG_INFO("D-ConCReCT Distributed Server starting...\n");

  /* Initialise child monitoring matrix */
  criaM(MatP);

  /* Start as RPL DODAG root - Contiki-NG API */
  NETSTACK_ROUTING.root_start();

  /* Register UDP connection on server port */
  simple_udp_register(&udp_conn, UDP_SERVER_PORT, NULL,
                      UDP_CLIENT_PORT, udp_rx_callback);

  /* Set timer to reset warnings every PERIOD seconds */
  etimer_set(&periodic_timer, PERIOD * CLOCK_SECOND);

  while(1) {
    PROCESS_WAIT_EVENT();

    if(etimer_expired(&periodic_timer)) {
      /* Reset warning counters periodically */
      limpaM(MatP);
      LOG_INFO("Warning counters reset (period expired)\n");
      imprimeM(MatP);
      etimer_reset(&periodic_timer);
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
