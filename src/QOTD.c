#include <pebble.h>

static Window *window;
static TextLayer *quote_layer;
static TextLayer *author_layer;
static GBitmap *quote_bitmap;
static BitmapLayer *image_layer;

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(quote_layer, "Select");
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(quote_layer, "Up");
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(quote_layer, "Down");
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void add_bitmap_layer(Layer *window_layer){
  quote_bitmap = gbitmap_create_with_resource(RESOURCE_ID_QUOTE);
  image_layer = bitmap_layer_create(GRect(0, 0, 144, 40));
  bitmap_layer_set_bitmap(image_layer,quote_bitmap );
  Layer *quote_layer = bitmap_layer_get_layer(image_layer);
  layer_add_child(window_layer, quote_layer);
}

static void add_text_layer_for_window(TextLayer *text_layer, char *text, GRect rect, GTextAlignment alignment) {
  Layer *window_layer = window_get_root_layer(window);

  text_layer = text_layer_create(rect);
  text_layer_set_text(text_layer, text);
  text_layer_set_text_alignment(text_layer, alignment);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void add_quote_layer(Layer *window_layer){
  add_text_layer_for_window(quote_layer,"Listening is a magnetic and strange thing, a creative force. The friends who listen to us are the ones we move toward. When we are listened to, it creates us, makes us unfold and expand.",GRect(0, 50, 144, 88) ,GTextAlignmentCenter);
}

static void add_author_layer(Layer *window_layer){
  add_text_layer_for_window(author_layer, "--Karl A. Menniger", GRect(0, 140, 144, 90), GTextAlignmentCenter);
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
