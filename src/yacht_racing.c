#include <pebble.h>
#include <time.h>
#include "pre_start.h"
#include "timer.h"
// #include "phone.h"
  
static void init() {
  timer_counter = -5 * 60;
  parse_time();
  /*
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  */
}

static void deinit() {

}

int main(void) {
  init();
  show_pre_start();
  app_event_loop();
  deinit();
}