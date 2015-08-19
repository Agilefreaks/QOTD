#include <pebble.h>


static Window *window;
static TextLayer *quote_layer;
static TextLayer *author_layer;
static GBitmap *quote_bitmap;
static BitmapLayer *image_layer;
static GFont author_font;
////////////////////////////////
static void inbox_received_callback(DictionaryIterator *iterator, void *context) {

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

static void add_bitmap_layer(Layer *window_layer){
  quote_bitmap = gbitmap_create_with_resource(RESOURCE_ID_QUOTE);
  image_layer = bitmap_layer_create(GRect(0, 0, 144, 40));
  bitmap_layer_set_bitmap(image_layer,quote_bitmap );
  Layer *quote_layer = bitmap_layer_get_layer(image_layer);
  layer_add_child(window_layer, quote_layer);
}

static void prepare_text_layer(TextLayer *text_layer, char *text, GRect rect, GTextAlignment alignment) {

}

static void add_quote_layer(Layer *window_layer){
  quote_layer = text_layer_create(GRect(0, 41, 144, 88));
  text_layer_set_text(quote_layer, "D`oh!");
  text_layer_set_text_alignment(quote_layer, GTextAlignmentCenter);

  layer_add_child(window_layer, text_layer_get_layer(quote_layer));
}

static void add_author_layer(Layer *window_layer){
  author_layer = text_layer_create(GRect(0, 130, 144, 30));
  text_layer_set_text(author_layer, "Homer J. Simpson");
  text_layer_set_text_alignment(author_layer, GTextAlignmentCenter);

  author_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_AUTHOR_25));
  text_layer_set_font(author_layer, author_font);

  layer_add_child(window_layer, text_layer_get_layer(author_layer));
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  add_bitmap_layer(window_layer);
  add_quote_layer(window_layer);
  add_author_layer(window_layer);
}



static void window_unload(Window *window) {
  text_layer_destroy(quote_layer);
  text_layer_destroy(author_layer);
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
