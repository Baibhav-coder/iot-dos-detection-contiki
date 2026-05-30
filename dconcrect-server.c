/*
 * D-ConCReCT Distributed Server - Phase 2 Enhanced
 * Original: Borgiani et al. (2021) - vspnet/ConCReCT
 * Ported by: Baibhav Chowdhury (24232033), NCI MSc Cybersecurity
 *
 * Phase 2 Enhancements:
 *   Enhancement 1 — Adaptive Traffic Segmentation
 *     Each child node now has 4 per-category TMN counters instead of 1.
 *     classify_packet() assigns each packet to CAT_SENSING, CAT_EVENT,
 *     CAT_CONTROL, or CAT_FLOOD based on UDP source port.
 *     Reference: Hammoudeh et al. — per-type thresholds, 98.84% accuracy.
 *
 *   Enhancement 2 — Dynamic Threshold (warm-up phase only, this version)
 *     Static D-ConCReCT polynomial used as seed during warm-up.
 *     Full sliding window dynamic threshold follows in next iteration.
 *     Reference: Raeiszadeh et al. (2024), Guo (2021), Rafiei et al. (2019).
 *
 * Bug fix: criaM() used undefined 'C' — corrected to NCOLS.
 */

#include "contiki.h"
#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip-ds6.h"
#include "net/routing/rpl-lite/rpl.h"
#include "sys/etimer.h"
#include "sys/log.h"
#include "traffic_classifier.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOG_MODULE "DConCReCT-S"
#define LOG_LEVEL LOG_LEVEL_INFO

/* Port definitions */
#define UDP_CLIENT_PORT 5000
#define UDP_SERVER_PORT 5678

/* Matrix dimensions */
#define L       20      /* max 20 children per parent node */

/*
 * EXTENDED MATRIX COLUMNS — Phase 2
 *
 * Original D-ConCReCT: MatP[20][4]
 *   [id, TMN, warnings, muted]
 *
 * Phase 2 Extended: MatP[20][8]
 *   [id, TMN_total, warnings, muted,
 *    TMN_sensing, TMN_event, TMN_control, TMN_flood]
 *
 * RAM cost:
 *   20 nodes x 8 columns x 2 bytes (int) = 320 bytes
 *   Original was 20 x 4 x 2 = 160 bytes
 *   Increase: +160 bytes — well within 10KB Tmote Sky RAM
 */
#define NCOLS   8

/* Column indices */
#define COL_ID          0
#define COL_TMN         1   /* Total TMN (all categories combined) */
#define COL_AVISO       2   /* Warning counter */
#define COL_PENAL       3   /* Penalty/mute flag */
#define COL_TMN_SENSING 4   /* Per-category: sensing packets */
#define COL_TMN_EVENT   5   /* Per-category: event/alarm packets */
#define COL_TMN_CONTROL 6   /* Per-category: control/management packets */
#define COL_TMN_FLOOD   7   /* Per-category: flood/unknown packets */

/* Detection parameters */
#define AVISO_MAX   3   /* max warnings before muting */
#define PERIOD      180 /* seconds before warning reset */

/* UDP connection handle */
static struct simple_udp_connection udp_conn;

/* Extended child monitoring matrix */
static int MatP[L][NCOLS];

/*---------------------------------------------------------------------------*/
/* Matrix initialisation - zero all entries */
static void
criaM(int Mat[L][NCOLS])
{
  int i, j;
  for(i = 0; i < L; i++)
    for(j = 0; j < NCOLS; j++)   /* Bug fix: was 'C', now NCOLS */
      Mat[i][j] = 0;
}

/*---------------------------------------------------------------------------*/
/* Reset TMN and warnings after each PERIOD - not the mute flag */
static void
limpaM(int Mat[L][NCOLS])
{
  int i;
  for(i = 0; i < L; i++) {
    Mat[i][COL_AVISO]       = 0;
    Mat[i][COL_TMN]         = 0;
    Mat[i][COL_TMN_SENSING] = 0;
    Mat[i][COL_TMN_EVENT]   = 0;
    Mat[i][COL_TMN_CONTROL] = 0;
    Mat[i][COL_TMN_FLOOD]   = 0;
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
/* Print current matrix state including per-category counters */
static void
imprimeM(int Mat[L][NCOLS])
{
  int i;
  LOG_INFO("--- Child Matrix (Phase 2) ---\n");
  for(i = 0; i < L; i++) {
    if(Mat[i][COL_ID] == 0) break;
    if(Mat[i][COL_PENAL] == 1) {
      LOG_INFO("  Node %d: MUTED\n", Mat[i][COL_ID]);
    } else {
      LOG_INFO("  Node %d: Active | TMN=%d Warn=%d | S=%d E=%d C=%d F=%d\n",
               Mat[i][COL_ID],
               Mat[i][COL_TMN],
               Mat[i][COL_AVISO],
               Mat[i][COL_TMN_SENSING],
               Mat[i][COL_TMN_EVENT],
               Mat[i][COL_TMN_CONTROL],
               Mat[i][COL_TMN_FLOOD]);
    }
  }
}

/*---------------------------------------------------------------------------*/
/*
 * STATIC THRESHOLD FORMULA - original D-ConCReCT polynomial
 * Used as warm-up seed until dynamic threshold takes over (Phase 2 next step).
 * f = number of children the sender has
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
 * Core detection function - Phase 2 enhanced.
 *
 * Now accepts traffic category from classifier.
 * Per-category TMN counter is incremented in addition to total TMN.
 * Detection still uses total TMN against static threshold (warm-up phase).
 * Full per-category dynamic threshold replaces this in next iteration.
 *
 * Returns 1 if node should be muted, 0 otherwise.
 */
static int
consultaM(int Mat[L][NCOLS], int id, int f, uint8_t category)
{
  int i;
  int TEN = (int)Limitetrafego(f + 1);

  for(i = 0; i < L; i++) {

    /* Empty slot - register new child */
    if(Mat[i][COL_ID] == 0) {
      Mat[i][COL_ID]  = id;
      Mat[i][COL_TMN] = 1;
      /* Increment per-category counter */
      Mat[i][COL_TMN_SENSING + category] = 1;
      LOG_INFO("New child registered: Node %d (cat=%d)\n", id, category);
      return 0;
    }

    /* Found the child's row */
    if(Mat[i][COL_ID] == id) {
      Mat[i][COL_TMN]++;

      /* Increment per-category counter */
      if(category < NUM_CATEGORIES) {
        Mat[i][COL_TMN_SENSING + category]++;
      }

      /* Log if flood category detected */
      if(category == CAT_FLOOD) {
        LOG_INFO("Node %d FLOOD packet | TMN_flood=%d\n",
                 id, Mat[i][COL_TMN_FLOOD]);
      }

      if(Mat[i][COL_TMN] > TEN) {
        Mat[i][COL_AVISO]++;

        if(Mat[i][COL_AVISO] > AVISO_MAX) {
          LOG_INFO("Node %d PENALISED | Warnings=%d TMN=%d cat=%d\n",
                   id, Mat[i][COL_AVISO], Mat[i][COL_TMN], category);
          Mat[i][COL_AVISO] = 0;
          Mat[i][COL_TMN]   = 0;
          Mat[i][COL_PENAL] = 1;
          return 1;
        } else {
          LOG_INFO("Node %d WARNING %d | TMN=%d TEN=%d cat=%d\n",
                   id, Mat[i][COL_AVISO], Mat[i][COL_TMN], TEN, category);
          Mat[i][COL_TMN] = 0;
          return 0;
        }
      } else {
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
 * Phase 2: classify each packet before passing to consultaM
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
  uint8_t category;

  /* Copy data safely */
  if(datalen >= sizeof(buf)) datalen = sizeof(buf) - 1;
  memcpy(buf, data, datalen);
  buf[datalen] = '\0';

  /* Get last byte of sender IPv6 address as node ID */
  sender_id = sender_addr->u8[15];

  /* PHASE 2: Classify the packet by source port */
  category = classify_packet(sender_port, datalen);

  LOG_INFO("Received '%s' from node %d (port=%d cat=%d)\n",
           buf, sender_id, sender_port, category);

  /* Handle "awake" message */
  if(strcmp(buf, "awake") == 0) {
    DesmutaNo(MatP, sender_id);
    return;
  }

  /* Extract number of children from message */
  f = atoi(buf);

  /* Run detection with category */
  mute_node = consultaM(MatP, sender_id, f, category);

  if(mute_node == 1) {
    LOG_INFO("Sending WAIT to node %d\n", sender_id);
    simple_udp_sendto(&udp_conn, "wait", 4, sender_addr);
  }
}

/*---------------------------------------------------------------------------*/
PROCESS(udp_server_process, "D-ConCReCT Server");
AUTOSTART_PROCESSES(&udp_server_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(udp_server_process, ev, data)
{
  static struct etimer periodic_timer;

  PROCESS_BEGIN();

  LOG_INFO("D-ConCReCT Phase 2 Server starting...\n");

  criaM(MatP);
  NETSTACK_ROUTING.root_start();

  simple_udp_register(&udp_conn, UDP_SERVER_PORT, NULL,
                      UDP_CLIENT_PORT, udp_rx_callback);

  etimer_set(&periodic_timer, PERIOD * CLOCK_SECOND);

  while(1) {
    PROCESS_WAIT_EVENT();

    if(etimer_expired(&periodic_timer)) {
      limpaM(MatP);
      LOG_INFO("Warning counters reset (period expired)\n");
      imprimeM(MatP);
      etimer_reset(&periodic_timer);
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
