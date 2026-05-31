/*
 * D-ConCReCT Distributed Server - Phase 2 Enhanced
 * Original: Borgiani et al. (2021) - vspnet/ConCReCT
 * Ported by: Baibhav Chowdhury (24232033), NCI MSc Cybersecurity
 *
 * Phase 2 Enhancements:
 *   1. Adaptive Traffic Segmentation - classify_packet() per packet
 *   2. Dynamic Threshold - dyn_get_threshold() replaces static polynomial
 *   3. Cumulative Deviation Monitor - dyn_check_cumdev() catches stealth attacks
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
#include "dynamic_threshold.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOG_MODULE "DConCReCT-S"
#define LOG_LEVEL LOG_LEVEL_INFO

#define UDP_CLIENT_PORT 5000
#define UDP_SERVER_PORT 5678

#define L       20
#define NCOLS   8

#define COL_ID          0
#define COL_TMN         1
#define COL_AVISO       2
#define COL_PENAL       3
#define COL_TMN_SENSING 4
#define COL_TMN_EVENT   5
#define COL_TMN_CONTROL 6
#define COL_TMN_FLOOD   7

#define AVISO_MAX   6
#define PERIOD      180

static struct simple_udp_connection udp_conn;
static int MatP[L][NCOLS];

static void criaM(int Mat[L][NCOLS]) {
  int i, j;
  for(i = 0; i < L; i++)
    for(j = 0; j < NCOLS; j++)
      Mat[i][j] = 0;
}

static void limpaM(int Mat[L][NCOLS]) {
  int i;
  for(i = 0; i < L; i++) {
    Mat[i][COL_AVISO] = 0;
    Mat[i][COL_TMN] = 0;
    Mat[i][COL_TMN_SENSING] = 0;
    Mat[i][COL_TMN_EVENT] = 0;
    Mat[i][COL_TMN_CONTROL] = 0;
    Mat[i][COL_TMN_FLOOD] = 0;
  }
}

static void DesmutaNo(int Mat[L][NCOLS], int id) {
  int i;
  for(i = 0; i < L; i++) {
    if(Mat[i][COL_ID] == id) {
      Mat[i][COL_PENAL] = 0;
      dyn_reset_node((uint8_t)id);
      LOG_INFO("Node %d unmuted (awake received)\n", id);
    }
  }
}

static void imprimeM(int Mat[L][NCOLS]) {
  int i;
  LOG_INFO("--- Child Matrix (Phase 2) ---\n");
  for(i = 0; i < L; i++) {
    if(Mat[i][COL_ID] == 0) break;
    if(Mat[i][COL_PENAL] == 1) {
      LOG_INFO("  Node %d: MUTED\n", Mat[i][COL_ID]);
    } else {
      LOG_INFO("  Node %d: Active|TMN=%d Warn=%d S=%d E=%d C=%d F=%d\n",
               Mat[i][COL_ID], Mat[i][COL_TMN], Mat[i][COL_AVISO],
               Mat[i][COL_TMN_SENSING], Mat[i][COL_TMN_EVENT],
               Mat[i][COL_TMN_CONTROL], Mat[i][COL_TMN_FLOOD]);
    }
  }
}

static int consultaM(int Mat[L][NCOLS], int id, int f, uint8_t category) {
  int i;
  int16_t TEN = dyn_get_threshold((uint8_t)id, category, f);

  for(i = 0; i < L; i++) {
    if(Mat[i][COL_ID] == 0) {
      Mat[i][COL_ID] = id;
      Mat[i][COL_TMN] = 1;
      Mat[i][COL_TMN_SENSING + category] = 1;
      LOG_INFO("New child registered: Node %d (cat=%d)\n", id, category);
      return 0;
    }
    if(Mat[i][COL_ID] == id) {
      Mat[i][COL_TMN]++;
      if(category < NUM_CATEGORIES) {
        Mat[i][COL_TMN_SENSING + category]++;
      }
      if(category == CAT_FLOOD) {
        LOG_INFO("Node %d FLOOD pkt|TMN_flood=%d\n", id, Mat[i][COL_TMN_FLOOD]);
      }
      /* Check cumulative deviation for stealth attacks */
      if(dyn_check_cumdev((uint8_t)id, category, (int16_t)Mat[i][COL_TMN], TEN)) {
        LOG_INFO("Node %d STEALTH ATTACK detected (cumdev)|cat=%d\n", id, category);
        Mat[i][COL_PENAL] = 1;
        return 1;
      }
      if(Mat[i][COL_TMN] > (int)TEN) {
        Mat[i][COL_AVISO]++;
        if(Mat[i][COL_AVISO] > AVISO_MAX) {
          LOG_INFO("Node %d PENALISED|Warnings=%d TMN=%d cat=%d\n",
                   id, Mat[i][COL_AVISO], Mat[i][COL_TMN], category);
          Mat[i][COL_AVISO] = 0;
          Mat[i][COL_TMN] = 0;
          Mat[i][COL_PENAL] = 1;
          return 1;
        } else {
          LOG_INFO("Node %d WARNING %d|TMN=%d TEN=%d cat=%d\n",
                   id, Mat[i][COL_AVISO], Mat[i][COL_TMN], (int)TEN, category);
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

static void udp_rx_callback(struct simple_udp_connection *c,
                const uip_ipaddr_t *sender_addr,
                uint16_t sender_port,
                const uip_ipaddr_t *receiver_addr,
                uint16_t receiver_port,
                const uint8_t *data,
                uint16_t datalen) {
  char buf[32];
  int sender_id;
  int f = 0;
  int mute_node;
  uint8_t category;

  if(datalen >= sizeof(buf)) datalen = sizeof(buf) - 1;
  memcpy(buf, data, datalen);
  buf[datalen] = '\0';

  sender_id = sender_addr->u8[15];
  category = classify_packet(sender_port, datalen);

  LOG_INFO("Rx '%s' node %d port=%d cat=%d\n", buf, sender_id, sender_port, category);

  if(strcmp(buf, "awake") == 0) {
    DesmutaNo(MatP, sender_id);
    return;
  }

  f = atoi(buf);
  mute_node = consultaM(MatP, sender_id, f, category);

  if(mute_node == 1) {
    LOG_INFO("Sending WAIT to node %d\n", sender_id);
    simple_udp_sendto(&udp_conn, "wait", 4, sender_addr);
  }
}

PROCESS(udp_server_process, "D-ConCReCT Server");
AUTOSTART_PROCESSES(&udp_server_process);

PROCESS_THREAD(udp_server_process, ev, data) {
  static struct etimer periodic_timer;
  PROCESS_BEGIN();

  LOG_INFO("D-ConCReCT Phase 2 Server starting...\n");

  criaM(MatP);
  dyn_init();
  NETSTACK_ROUTING.root_start();

  simple_udp_register(&udp_conn, UDP_SERVER_PORT, NULL,
                      0, udp_rx_callback);

  etimer_set(&periodic_timer, PERIOD * CLOCK_SECOND);

  while(1) {
    PROCESS_WAIT_EVENT();
    if(etimer_expired(&periodic_timer)) {
      limpaM(MatP);
      LOG_INFO("Period reset\n");
      imprimeM(MatP);
      etimer_reset(&periodic_timer);
    }
  }

  PROCESS_END();
}
