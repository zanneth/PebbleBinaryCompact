/*
 * binarycompact.c
 *
 * Compact binary watch face.
 *
 * Charles Magahern <charles@magahern.com>
 * Date Created: 06/30/2013
 */

#include <stdint.h>
#include <stdbool.h>
#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#define MY_UUID { 0xC0, 0x76, 0xEA, 0xE9, 0x19, 0xAC, 0x41, 0x4E, 0xA6, 0xCB, 0x0F, 0x79, 0xC8, 0x12, 0xD1, 0x9C }
PBL_APP_INFO(MY_UUID, "Binary Compact", "zanneth", 1, 0, RESOURCE_ID_IMAGE_MENU_ICON, APP_INFO_WATCH_FACE);

// constants
#define COMPONENT_RECT_WIDTH    10
#define COMPONENT_RECT_HEIGHT   16

// globals
static PblTm current_time;
static Window window;
static Layer hour_layer;
static Layer minute_layer;
static Layer second_layer;

// callbacks
static void init_handler(AppContextRef app_ctx);
static void tick_handler(AppContextRef app_ctx, PebbleTickEvent *event);

// drawing functions
static void draw_hour_layer(Layer *layer, GContext *ctx);
static void draw_minute_layer(Layer *layer, GContext *ctx);
static void draw_second_layer(Layer *layer, GContext *ctx);
static void draw_binary_rects(GContext *ctx, GRect frame, unsigned components, int32_t number);

// utility
static void get_binary_components(int32_t number, unsigned num_components, bool *out_components);

void init_handler(AppContextRef app_ctx)
{
    window_init(&window, "MainWindow");
    window_set_background_color(&window, GColorBlack);
    window_stack_push(&window, true);

    const GRect window_bounds = window.layer.bounds;
    const int16_t layers_width = window_bounds.size.w;
    const int16_t layers_height = window_bounds.size.h / 3;

    // setup hour layer
    GRect hour_bounds = GRect(0, 0, layers_width, layers_height);
    layer_init(&hour_layer, hour_bounds);
    layer_set_update_proc(&hour_layer, &draw_hour_layer);
    layer_add_child(&window.layer, &hour_layer);

    // setup minute layer
    GRect minute_bounds = GRect(0, layers_height, layers_width, layers_height);
    layer_init(&minute_layer, minute_bounds);
    layer_set_update_proc(&minute_layer, &draw_minute_layer);
    layer_add_child(&window.layer, &minute_layer);

    // setup second layer
    GRect second_bounds = GRect(0, layers_height * 2, layers_width, layers_height);
    layer_init(&second_layer, second_bounds);
    layer_set_update_proc(&second_layer, &draw_second_layer);
    layer_add_child(&window.layer, &second_layer);

    // setup current time
    get_time(&current_time);
}

void tick_handler(AppContextRef app_ctx, PebbleTickEvent *event)
{
    memcpy(&current_time, event->tick_time, sizeof(PblTm));

    if (event->units_changed & HOUR_UNIT) {
        layer_mark_dirty(&hour_layer);
    }

    if (event->units_changed & MINUTE_UNIT) {
        layer_mark_dirty(&minute_layer);
    }

    if (event->units_changed & SECOND_UNIT) {
        layer_mark_dirty(&second_layer);
    }
}

void draw_hour_layer(Layer *layer, GContext *ctx)
{
    int hours;
    int components;

    if (clock_is_24h_style()) {
        hours = current_time.tm_hour;
        components = 5;
    } else {
        hours = current_time.tm_hour % 12;
        if (hours == 0) {
            hours = 12;
        }
        components = 4;
    }

    draw_binary_rects(ctx, layer->frame, components, hours);
}

void draw_minute_layer(Layer *layer, GContext *ctx)
{
    draw_binary_rects(ctx, layer->frame, 6, current_time.tm_min);
}

void draw_second_layer(Layer *layer, GContext *ctx)
{
    draw_binary_rects(ctx, layer->frame, 6, current_time.tm_sec);
}

void draw_binary_rects(GContext *ctx, GRect frame, unsigned components, int32_t number)
{
    bool binary_components[components];
    get_binary_components(number, components, binary_components);

    const int16_t hpadding = (frame.size.w - (components * COMPONENT_RECT_WIDTH)) / (components + 1);
    const int16_t origin_y = (frame.size.h / 2) - (COMPONENT_RECT_HEIGHT / 2);
    GRect cur_draw_rect = GRect(0, origin_y, COMPONENT_RECT_WIDTH, COMPONENT_RECT_HEIGHT);

    graphics_context_set_fill_color(ctx, GColorWhite);
    for (unsigned i = 0; i < components; ++i) {
        int16_t curx = (i * COMPONENT_RECT_WIDTH) + ((i + 1) * hpadding);
        cur_draw_rect.origin.x = curx;

        if (binary_components[i]) {
            graphics_fill_rect(ctx, cur_draw_rect, 0, GCornerNone);
        } else {
            graphics_draw_round_rect(ctx, cur_draw_rect, 0);
        }
    }
}

void get_binary_components(int32_t number, unsigned num_components, bool *out_components)
{
    for (unsigned i = 0; i < num_components; ++i) {
        out_components[num_components - 1 - i] = (number & (1 << i));
    }
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
