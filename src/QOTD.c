#include <pebble.h>
#define KEY_CONTENT 0
#define KEY_TITLE 1

static Window *window;

static ScrollLayer *scroll_layer;
static TextLayer *content_layer;
static TextLayer *title_layer;
static GFont title_font;
// Store incoming information
static char content_buffer[1024];
static char title_buffer[100];
static char codesmell_layer_buffer[1124];

////////////////////////////////

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Read first item
 Tuple *t = dict_read_first(iterator);

 // For all items
 while(t != NULL) {
   // Which key was received?
   switch(t->key) {
   case KEY_CONTENT:
      APP_LOG(APP_LOG_LEVEL_ERROR, "%d %s", (int)t->key, (char*)t->value);
      snprintf(content_buffer, sizeof(content_buffer), "%s", t->value->cstring);
     break;
   case KEY_TITLE:
    APP_LOG(APP_LOG_LEVEL_ERROR, "%d %s", (int)t->key, (char*)t->value);
    snprintf(title_buffer, sizeof(title_buffer), "%s", t->value->cstring);
     break;
   default:
     APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
     break;
   }

   // Look for next item
   t = dict_read_next(iterator);
 }


 snprintf(codesmell_layer_buffer, sizeof(codesmell_layer_buffer), "%s, %s", content_buffer, title_buffer);
 text_layer_set_size(content_layer, GSize(144, 2000));
 text_layer_set_text(content_layer, codesmell_layer_buffer);
 GSize max_size = text_layer_get_content_size(content_layer);
 text_layer_set_size(content_layer, max_size);
 scroll_layer_set_content_size(scroll_layer, GSize(144, max_size.h + 4));
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

static void click_config_provider(void *context) {
}

static void prepare_text_layer(TextLayer *text_layer, char *text, GRect rect, GTextAlignment alignment) {

}

static void add_content(ScrollLayer *scroll_layer){
  GRect max_text_bounds = GRect(0, 41, 144, 2000);
  content_layer = text_layer_create(max_text_bounds);
  text_layer_set_text(content_layer, "Loading today's codesmell");
  text_layer_set_text_alignment(content_layer, GTextAlignmentCenter);

  GSize max_size = text_layer_get_content_size(content_layer);
  text_layer_set_size(content_layer, max_size);
  scroll_layer_set_content_size(scroll_layer, GSize(144, max_size.h + 4));

  scroll_layer_add_child(scroll_layer, text_layer_get_layer(content_layer));
}

static void add_title_layer(ScrollLayer *scroll_layer){
  GRect max_text_bounds = GRect(0, 0, 144, 2000);
  title_layer = text_layer_create(max_text_bounds);
  text_layer_set_text(title_layer, "Homer J. Simpson");
  text_layer_set_text_alignment(title_layer, GTextAlignmentCenter);

  title_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_AUTHOR_25));
  text_layer_set_font(title_layer, title_font);

  scroll_layer_add_child(scroll_layer, text_layer_get_layer(title_layer));
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);
  GRect max_text_bounds = GRect(0, 0, 144, 2000);

  scroll_layer = scroll_layer_create(bounds);
  scroll_layer_set_click_config_onto_window(scroll_layer, window);

  add_content(scroll_layer);
  //add_title_layer(window_layer);

  layer_add_child(window_layer, scroll_layer_get_layer(scroll_layer));
}



static void window_unload(Window *window) {
  text_layer_destroy(content_layer);
  text_layer_destroy(title_layer);
}

static void init(void) {
  //////////////////////////////
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
///////////////////
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
