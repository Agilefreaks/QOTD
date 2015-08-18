#include <pebble.h>

static Window *window;
static ScrollLayer *scroll_layer;
static TextLayer *quote_layer;
static TextLayer *author_layer;
static GBitmap *quote_bitmap;
static BitmapLayer *image_layer;
static GFont author_font;
static char scroll_text[] = "Listening is a magnetic and strange thing, a creative force. The friends who listen to us are the ones we move toward. When we are listened to, it creates us, makes us unfold and expand.Listening is a magnetic and strange thing, a creative force. The friends who listen to us are the ones we move toward. When we are listened to, it creates us, makes us unfold and expand.";

static void click_config_provider(void *context) {
}

static void add_bitmap_layer(ScrollLayer *scroll_layer){
  quote_bitmap = gbitmap_create_with_resource(RESOURCE_ID_QUOTE);
  image_layer = bitmap_layer_create(GRect(0, 0, 144, 40));
  bitmap_layer_set_bitmap(image_layer,quote_bitmap );
  Layer *quote_layer = bitmap_layer_get_layer(image_layer);
  scroll_layer_add_child(scroll_layer, quote_layer);
}

static void prepare_text_layer(TextLayer *text_layer, char *text, GRect rect, GTextAlignment alignment) {

}

static void add_quote_layer(ScrollLayer *scroll_layer){
  GRect max_text_bounds = GRect(0, 41, 144, 2000);
  quote_layer = text_layer_create(max_text_bounds);
  text_layer_set_text(quote_layer, scroll_text);
  text_layer_set_text_alignment(quote_layer, GTextAlignmentCenter);

  GSize max_size = text_layer_get_content_size(quote_layer);
  text_layer_set_size(quote_layer, max_size);
  scroll_layer_set_content_size(scroll_layer, GSize(144, max_size.h + 4));

  scroll_layer_add_child(scroll_layer, text_layer_get_layer(quote_layer));
}

static void add_author_layer(ScrollLayer *scroll_layer){
  GRect max_text_bounds = GRect(0, 0, 144, 2000);
  author_layer = text_layer_create(max_text_bounds);
  text_layer_set_text(author_layer, "Karl A. Menniger");
  text_layer_set_text_alignment(author_layer, GTextAlignmentCenter);

  author_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_AUTHOR_25));
  text_layer_set_font(author_layer, author_font);

  scroll_layer_add_child(scroll_layer, text_layer_get_layer(author_layer));
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);
  GRect max_text_bounds = GRect(0, 0, 144, 2000);

  scroll_layer = scroll_layer_create(bounds);
  scroll_layer_set_click_config_onto_window(scroll_layer, window);

  add_bitmap_layer(scroll_layer);
  add_quote_layer(scroll_layer);
  //add_author_layer(scroll_layer);

  layer_add_child(window_layer, scroll_layer_get_layer(scroll_layer));
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
