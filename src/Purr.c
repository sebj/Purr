#include "pebble.h"

#define PURR_TIMEOUT 300000
#define PURR_INTRO_TIMEOUT 3500

static Window *window;
static TextLayer *text_layer;
static AppTimer *timer;

static const VibePattern purr_pattern = {
  .durations = (uint32_t []) {100, 75, 200},
  .num_segments = 3
};

static void purr(void *data) {
  vibes_enqueue_custom_pattern(purr_pattern);

  timer = app_timer_register(PURR_TIMEOUT, purr, NULL);
}

static void hide_intro(void *data) {
  text_layer_destroy(text_layer);
}

void init() {
  window = window_create();
  window_set_background_color(window, GColorBlack);
  window_stack_push(window, true);

  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);
  text_layer = text_layer_create((GRect){ .origin = { 0, 50 }, .size = bounds.size });
  text_layer_set_text(text_layer, "Purr");
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_background_color(text_layer, GColorClear);
  text_layer_set_text_color(text_layer, GColorWhite);
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));

  timer = app_timer_register(PURR_TIMEOUT, purr, NULL);
  app_timer_register(PURR_INTRO_TIMEOUT, hide_intro, NULL);
}

void deinit() {
  app_timer_cancel(timer);
  
  if (text_layer != NULL) {
    text_layer_destroy(text_layer);
  }

  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}