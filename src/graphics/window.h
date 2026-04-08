#ifndef WINDOW_H
#define WINDOW_H

#include "renderer.h"
#include "GLFW/glfw3.h"
#include <stdbool.h>
#include "../input/keyboard.h"
#include "../input/mouse.h"

struct window;

typedef void (*window_resize_callback)(struct window *, int, int, void *);
typedef void (*window_cursor_position_callback)(struct window *, double, double,
                                                void *);
typedef void (*window_scroll_callback)(struct window *, double, double, void *);

struct window {
    GLFWwindow *glfw_window;
    int width;
    int height;
    char *title;
    struct keyboard keyboard;
    struct mouse mouse;
    double delta_time;
    double previous_time;
    window_resize_callback resize_callback;
    void *resize_callback_arg;
    window_cursor_position_callback cursor_position_callback;
    void *cursor_position_callback_arg;
    window_scroll_callback scroll_callback;
    void *scroll_callback_arg;
};

void window_init(struct window *window, int width, int height, char *title);
void window_destroy(struct window *window);
void window_shutdown();
bool window_should_close(struct window *window);
void window_swap_buffers(struct window *window);
float window_get_aspect_ratio(struct window *window);
void window_reset_cursor(struct window *window);
void window_capture_cursor(struct window *window);
void window_set_resize_callback(struct window *window,
                                window_resize_callback function, void *arg);
void window_set_cursor_position_callback(
    struct window *window, window_cursor_position_callback function, void *arg);
void window_set_scroll_callback(struct window *window,
                                window_scroll_callback function, void *arg);
double window_get_delta_time(struct window *window);
void window_update_delta_time(struct window *window);
void window_update_input(struct window *window);

#endif
