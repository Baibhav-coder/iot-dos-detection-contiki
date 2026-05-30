#ifndef TRAFFIC_CLASSIFIER_H
#define TRAFFIC_CLASSIFIER_H

#include <stdint.h>

/* ── Traffic Categories ── */
#define CAT_SENSING     0   /* Periodic sensor data — regular, predictable */
#define CAT_EVENT       1   /* Event-based alerts — bursty, legitimate spikes */
#define CAT_CONTROL     2   /* Control/management — low volume, administrative */
#define CAT_FLOOD       3   /* Flood/DoS — high rate, anomalous */
#define NUM_CATEGORIES  4

/* Port ranges used for classification */
#define PORT_SENSING_MIN   5000
#define PORT_SENSING_MAX   5099
#define PORT_EVENT_MIN     5100
#define PORT_EVENT_MAX     5199
#define PORT_CONTROL_MIN   5200
#define PORT_CONTROL_MAX   5299

uint8_t classify_packet(uint16_t src_port, uint16_t pkt_size);

#endif /* TRAFFIC_CLASSIFIER_H */
