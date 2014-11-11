#include "racing.h"
#include <pebble.h>
#include "pre_start.h"
#include "timer.h"

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_roboto_condensed_21;
static GFont s_res_pixelmix_16;
static TextLayer *timer_layer;
static TextLayer *time_layer;
static TextLayer *speed_layer;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_background_color(s_window, GColorBlack);
  window_set_fullscreen(s_window, 1);
  
  s_res_roboto_condensed_21 = fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21);
  s_res_pixelmix_16 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_PIXELMIX_16));
  // timer_layer
  timer_layer = text_layer_create(GRect(10, 31, 124, 24));
  text_layer_set_background_color(timer_layer, GColorClear);
  text_layer_set_text_color(timer_layer, GColorWhite);
  text_layer_set_text(timer_layer, "00:00:00");
  text_layer_set_font(timer_layer, s_res_roboto_condensed_21);
  layer_add_child(window_get_root_layer(s_window), (Layer *)timer_layer);
  
  // time_layer
  time_layer = text_layer_create(GRect(0, 0, 144, 20));
  text_layer_set_text(time_layer, "00:00:00");
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
  text_layer_set_font(time_layer, s_res_pixelmix_16);
  layer_add_child(window_get_root_layer(s_window), (Layer *)time_layer);
  
  // speed_layer
  speed_layer = text_layer_create(GRect(20, 77, 100, 20));
  text_layer_set_text(speed_layer, "10 kts");
  layer_add_child(window_get_root_layer(s_window), (Layer *)speed_layer);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(timer_layer);
  text_layer_destroy(time_layer);
  text_layer_destroy(speed_layer);
  fonts_unload_custom_font(s_res_pixelmix_16);
}
// END AUTO-GENERATED UI CODE

void update_timer_display() {
	snprintf(
    timer_str_buffer, 
    TIMER_STR_BUFFER_BYTES, 
    "%02d:%02d:%02d",
    abs(hours),
    abs(minutes), 
    abs(seconds)
  );	

  text_layer_set_text(timer_layer, timer_str_buffer);
}

static void handle_tick(struct tm *tick_time, TimeUnits units_changed) {
  timer_counter++;
  parse_time();
  update_timer_display();
}

static void handle_window_unload(Window* window) {
  destroy_ui();
}

void show_racing(void) {
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
  hide_pre_start();
  
  tick_timer_service_subscribe(SECOND_UNIT, handle_tick);
}

void hide_racing(void) {
  window_stack_remove(s_window, true);
}
