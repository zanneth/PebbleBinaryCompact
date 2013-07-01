/*
 * binarycompact.c
 *
 * Compact binary watch face.
 *
 * Charles Magahern <charles@magahern.com>
 * Date Created: 06/30/2013
 */

#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#define MY_UUID { 0xC0, 0x76, 0xEA, 0xE9, 0x19, 0xAC, 0x41, 0x4E, 0xA6, 0xCB, 0x0F, 0x79, 0xC8, 0x12, 0xD1, 0x9C }
PBL_APP_INFO(MY_UUID, "Binary Compact", "zanneth", 1, 0, DEFAULT_MENU_ICON, APP_INFO_WATCH_FACE);

// constants
#define COMPONENT_RECT_WIDTH    6
#define COMPONENT_RECT_HEIGHT   12

// globals
static Window window;
static Layer hour_layer;
static Layer minute_layer;

// callbacks
static void init_handler(AppContextRef app_ctx);
static void tick_handler(AppContextRef app_ctx, PebbleTickEvent *event);

// drawing functions
static void draw_hour_layer(Layer *layer, GContext *ctx);
static void draw_minute_layer(Layer *layer, GContext *ctx);
static void draw_binary_rects(GContext *ctx, GRect frame, unsigned components, unsigned number);

void init_handler(AppContextRef app_ctx)
{
    window_init(&window, "MainWindow");
    window_set_background_color(&window, GColorBlack);
    window_stack_push(&window, true);

    const GRect window_bounds = window.layer.bounds;
    const int16_t layers_width = window_bounds.size.w;
    const int16_t layers_height = window_bounds.size.h / 2;

    // setup hour layer
    GRect hour_bounds = GRect(0, 0, layers_width, layers_height);
    layer_init(&hour_layer, hour_bounds);
    layer_set_update_proc(&hour_layer, &draw_hour_layer);
    layer_add_child(&window.layer, &hour_layer);

    // setup minute layer
    GRect minute_bounds = GRect(0, layers_height, layers_width, layers_height);
    layer_init(&minute_layer, minute_bounds);
    layer_set_update_proc(&mintue_layer, &draw_minute_layer);
    layer_add_child(&window.layer, &minute_layer);
}

void tick_handler(AppContextRef app_ctx, PebbleTickEvent *event)
{
    // TODO
}

void draw_hour_layer(Layer *layer, GContext *ctx)
{
    // TODO
}

void draw_minute_layer(Layer *layer, GContext *ctx)
{
    // TODO
}

void draw_binary_rects(GContext *ctx, GRect frame, unsigned components, unsigned number)
{
    // TODO
}

void pbl_main(void *params)
{
    PebbleAppHandlers handlers = {
        .init_handler = &init_handler,
        .tick_info = {
            .tick_handler = &tick_handler,
            .tick_units   = SECOND_UNIT
        }
    };

    app_event_loop(params, &handlers);
}
