#include <pebble.h>
#include <time.h>
#include "pre_start.h"
#include "timer.h"

#define KEY_SPEED 0
  
TextLayer *speed_layer;
  
static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Store incoming information
  static char speed_buffer[8];
  
  // Read first item
  Tuple *t = dict_read_first(iterator);

  // For all items
  while(t != NULL) {
    // Which key was received?
    switch(t->key) {
    case KEY_SPEED:
      snprintf(speed_buffer, sizeof(speed_buffer), "%d", (int)t->value->int32);
      APP_LOG(APP_LOG_LEVEL_INFO, "Speed parsed");
      break;
    default:
      APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
      break;
    }

    // Look for next item
    t = dict_read_next(iterator);
  }
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "%s", speed_buffer);
  //text_layer_set_text(speed_layer, speed_buffer);
  //Window *curWindow = window_stack_get_top_window();
  //TextLayer *speedLayer = text_layer_get_layer(speed_layer);
  
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}
  
static void handle_tick(struct tm *tick_time, TimeUnits units_changed) {
  // Begin dictionary
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  // Add a key-value pair
  dict_write_uint8(iter, 0, 0);

  // Send the message!
  app_message_outbox_send();
}

static void init() {
  timer_counter = -5 * 60;
  parse_time();
  
  // Register phone callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  
  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  
  tick_timer_service_subscribe(SECOND_UNIT, handle_tick);
  
  APP_LOG(APP_LOG_LEVEL_INFO, "App Initialized.");
}

static void deinit() {

}

int main(void) {
  init();
  show_pre_start();
  app_event_loop();
  deinit();
}