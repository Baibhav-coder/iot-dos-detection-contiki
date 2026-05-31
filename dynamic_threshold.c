#include "dynamic_threshold.h"
#include <string.h>

static float static_threshold(int f) {
  float f2 = (float)f * f;
  float f3 = f2 * f;
  return 0.295f + 2.94f * f - 0.056f * f2 + 0.0006f * f3;
}

static dyn_node_t nodes[DYN_MAX_NODES];
static uint8_t node_count = 0;

void dyn_init(void) {
  memset(nodes, 0, sizeof(nodes));
  node_count = 0;
}

static dyn_node_t *find_node(uint8_t node_id) {
  uint8_t i;
  for(i = 0; i < node_count; i++) {
    if(nodes[i].node_id == node_id) return &nodes[i];
  }
  if(node_count < DYN_MAX_NODES) {
    nodes[node_count].node_id = node_id;
    node_count++;
    return &nodes[node_count - 1];
  }
  return NULL;
}

void dyn_record(uint8_t node_id, uint8_t category, int16_t tmn_value) {
  dyn_node_t *n = find_node(node_id);
  if(n == NULL || category >= NUM_CATEGORIES) return;
  uint8_t idx = n->hist_idx[category];
  n->history[category][idx] = tmn_value;
  n->hist_idx[category] = (idx + 1) % DYN_WINDOW_SIZE;
  if(n->window_count < 255) n->window_count++;
}

int16_t dyn_get_threshold(uint8_t node_id, uint8_t category, int f) {
  dyn_node_t *n = find_node(node_id);
  if(n == NULL || n->window_count < WARMUP_WINDOWS) {
    return (int16_t)static_threshold(f + 1);
  }
  int32_t sum = 0;
  uint8_t i;
  for(i = 0; i < DYN_WINDOW_SIZE; i++) sum += n->history[category][i];
  int16_t mean = (int16_t)(sum / DYN_WINDOW_SIZE);
  int32_t var = 0;
  for(i = 0; i < DYN_WINDOW_SIZE; i++) {
    int16_t diff = n->history[category][i] - mean;
    var += (int32_t)(diff * diff);
  }
  var = var / DYN_WINDOW_SIZE;
  int16_t stddev = 0;
  int16_t x = (int16_t)var;
  if(x > 0) {
    int16_t s = x;
    int16_t t = (s + 1) / 2;
    while(t < s) { s = t; t = (s + x / s) / 2; }
    stddev = s;
  }
  return mean + (int16_t)(K_SIGMA * stddev);
}

uint8_t dyn_check_cumdev(uint8_t node_id, uint8_t category, int16_t tmn, int16_t threshold) {
  dyn_node_t *n = find_node(node_id);
  if(n == NULL || category >= NUM_CATEGORIES) return 0;
  int16_t near_limit = (threshold * 8) / 10;
  if(tmn >= near_limit && tmn <= threshold) {
    n->cumdev[category] += (tmn - near_limit);
  } else {
    if(n->cumdev[category] > 0) n->cumdev[category]--;
  }
  if(n->cumdev[category] > (DYN_WINDOW_SIZE * threshold / 2)) {
    n->cumdev[category] = 0;
    return 1;
  }
  return 0;
}

void dyn_reset_node(uint8_t node_id) {
  dyn_node_t *n = find_node(node_id);
  if(n == NULL) return;
  memset(n->history, 0, sizeof(n->history));
  memset(n->hist_idx, 0, sizeof(n->hist_idx));
  memset(n->cumdev, 0, sizeof(n->cumdev));
  n->window_count = 0;
}
