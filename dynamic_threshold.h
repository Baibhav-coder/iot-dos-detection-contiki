#ifndef DYNAMIC_THRESHOLD_H
#define DYNAMIC_THRESHOLD_H

#include <stdint.h>
#include "traffic_classifier.h"

/*
 * Dynamic Threshold Module - Phase 2 Enhancement 2
 * Baibhav Chowdhury (24232033), NCI MSc Cybersecurity
 *
 * Replaces static D-ConCReCT polynomial TEN = 0.295 + 2.94f - 0.056f^2 + 0.0006f^3
 * with per-category sliding window threshold: threshold = mean + k * stddev
 *
 * References:
 *   Raeiszadeh et al. (2024) - adaptive thresholds with concept drift, 89.71% accuracy
 *   Guo (2021) - dynamic threshold in RPL/Contiki for DIS messages
 *   Rafiei et al. (2019) - DEEM warm-up phase design precedent
 */

/* Sliding window size - number of monitoring periods to keep */
#define DYN_WINDOW_SIZE     2

/* Sensitivity multiplier - threshold = mean + K_SIGMA * stddev */
#define K_SIGMA             2

/* Warm-up period - use static threshold until this many windows collected */
#define WARMUP_WINDOWS      2

/* Max nodes tracked */
#define DYN_MAX_NODES       5

/*
 * Per-node per-category sliding window history
 * Stores last DYN_WINDOW_SIZE TMN values for each category
 *
 * RAM cost:
 *   20 nodes x 4 categories x 5 windows x 2 bytes = 800 bytes
 *   Well within 10KB Tmote Sky RAM
 */
typedef struct {
    uint8_t node_id;
    uint8_t window_count;   /* how many windows collected so far */
    int16_t history[NUM_CATEGORIES][DYN_WINDOW_SIZE];
    uint8_t hist_idx[NUM_CATEGORIES]; /* circular buffer index per category */
    /* Cumulative deviation monitor */
    int16_t cumdev[NUM_CATEGORIES];   /* running sum of near-threshold deviations */
} dyn_node_t;

/* Initialise the dynamic threshold module */
void dyn_init(void);

/* Record a TMN observation for a node/category at end of monitoring window */
void dyn_record(uint8_t node_id, uint8_t category, int16_t tmn_value);

/*
 * Get the current threshold for a node/category
 * During warm-up: returns static D-ConCReCT threshold (f = num children)
 * After warm-up:  returns mean + K_SIGMA * stddev from sliding window
 */
int16_t dyn_get_threshold(uint8_t node_id, uint8_t category, int f);

/*
 * Cumulative deviation monitor
 * Returns 1 if sustained near-threshold traffic detected (stealth attack)
 * Returns 0 otherwise
 * Reference: Limitation 1 fix from CA2 proposal
 */
uint8_t dyn_check_cumdev(uint8_t node_id, uint8_t category, int16_t tmn, int16_t threshold);

/* Reset a node's history (called when node is unmuted) */
void dyn_reset_node(uint8_t node_id);

#endif /* DYNAMIC_THRESHOLD_H */
