#include "pre_start.h"
#include <pebble.h>
#include "racing.h"
#include "timer.h"

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_font_avenir_50;
static GFont s_res_pixelmix_16;
static TextLayer *timer_layer;
static TextLayer *time_layer;
static TextLayer *speed_layer;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_background_color(s_window, GColorBlack);
  window_set_fullscreen(s_window, true);
  
  s_res_font_avenir_50 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_AVENIR_50));
  s_res_pixelmix_16 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_PIXELMIX_16));
  // timer_layer
  timer_layer = text_layer_create(GRect(0, 45, 144, 50));
  text_layer_set_background_color(timer_layer, GColorClear);
  text_layer_set_text_color(timer_layer, GColorWhite);
  text_layer_set_text(timer_layer, "-5:00");
  text_layer_set_text_alignment(timer_layer, GTextAlignmentCenter);
  text_layer_set_font(timer_layer, s_res_font_avenir_50);
  layer_add_child(window_get_root_layer(s_window), (Layer *)timer_layer);
  
  // time_layer
  time_layer = text_layer_create(GRect(0, 0, 144, 20));
  text_layer_set_text(time_layer, "00:00:00");
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
  text_layer_set_font(time_layer, s_res_pixelmix_16);
  layer_add_child(window_get_root_layer(s_window), (Layer *)time_layer);
  
  // speed_layer
  speed_layer = text_layer_create(GRect(22, 127, 100, 20));
  text_layer_set_text(speed_layer, "10 kts");
  layer_add_child(window_get_root_layer(s_window), (Layer *)speed_layer);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(timer_layer);
  text_layer_destroy(time_layer);
  text_layer_destroy(speed_layer);
  fonts_unload_custom_font(s_res_font_avenir_50);
  fonts_unload_custom_font(s_res_pixelmix_16);
}
// END AUTO-GENERATED UI CODE








int abs(int v) 
{
	return v * ((v>0) - (v<0));
}

void update_countdown_display() {
	snprintf(
			 timer_str_buffer, 
			 TIMER_STR_BUFFER_BYTES, 
			 "-%d:%02d",
			 abs(minutes), 
			 abs(seconds)
			 );
	text_layer_set_text(timer_layer, timer_str_buffer);
}

static void handle_tick(struct tm *tick_time, TimeUnits units_changed) {
	if(started) {
		timer_counter++;
		
		parse_time();
		
		if(timer_counter == 0) {
			vibes_long_pulse();
      
      // Switch to racing mode
		  show_racing();
		} else {
      if(timer_counter < 0) { // in start sequence
  			if (timer_counter >= -10 || seconds == -30) {  // last 10 seconds
  				vibes_short_pulse();
  			} else if(
  			   seconds == 0 // every minutes
  			   || (minutes == 0 && seconds % 10 == 0) // every 10 seconds in the last minute
  			   ) {
  				vibes_double_pulse();
  			}
  			
  			update_countdown_display();
  		}
    }
    
    
	}
	
  strftime(time_str_buffer, TIME_STR_BUFFER_BYTES, "%H:%M:%S", tick_time);
	text_layer_set_text(time_layer, time_str_buffer);
}

void select_single_click_handler(ClickRecognizerRef recognizer, void *context) {	
	started = ! started;
	vibes_long_pulse();
}

void up_single_click_handler(ClickRecognizerRef recognizer, void *context) {	
	if (timer_counter < 0) {
		parse_time();

		if(started) {
			//minutes--;
			vibes_long_pulse();
		}
		
		seconds = 0;
		minutes--;
		timer_counter = minutes * 60;
		update_countdown_display();
	}
}

void down_single_click_handler(ClickRecognizerRef recognizer, void *context) {	
	if (timer_counter < -60 && minutes < 0) {
		parse_time();
		seconds = 0;
		if ( !started && seconds == 0) {
			minutes++;
		}
		timer_counter = minutes * 60;
		update_countdown_display();
		
		if ( started ) {
			vibes_long_pulse();
		}
	}
}

void click_config_provider(Window *s_window) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_single_click_handler);
}







static void handle_window_unload(Window* window) {
  destroy_ui();
  tick_timer_service_unsubscribe();
}

void show_pre_start(void) {
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
  tick_timer_service_subscribe(SECOND_UNIT, handle_tick);
  window_set_click_config_provider(s_window, (ClickConfigProvider) click_config_provider);
}

void hide_pre_start(void) {
  window_stack_remove(s_window, false);
}