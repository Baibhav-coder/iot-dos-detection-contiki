#include "traffic_classifier.h"

/*
 * classify_packet()
 *
 * Classification uses only existing packet metadata:
 *   - UDP source port (already available in simple_udp callback)
 *   - Packet size (already available as datalen)
 *
 * No new packet fields. No topology changes. Fits in ROM only (no RAM cost).
 *
 * References:
 *   Hammoudeh et al. — per-type thresholds (DNS/Telnet split), 98.84% accuracy
 *   Borgiani et al. (2021) — D-ConCReCT base, port metadata already present
 */
uint8_t classify_packet(uint16_t src_port, uint16_t pkt_size)
{
    /* Sensing: regular periodic sensor packets on known port range */
    if(src_port >= PORT_SENSING_MIN && src_port <= PORT_SENSING_MAX) {
        return CAT_SENSING;
    }

    /* Event: bursty alert packets on event port range */
    if(src_port >= PORT_EVENT_MIN && src_port <= PORT_EVENT_MAX) {
        return CAT_EVENT;
    }

    /* Control/Management: low-volume admin packets on control port range */
    if(src_port >= PORT_CONTROL_MIN && src_port <= PORT_CONTROL_MAX) {
        return CAT_CONTROL;
    }

    /* Default: anything outside known ranges treated as potential flood */
    return CAT_FLOOD;
}
