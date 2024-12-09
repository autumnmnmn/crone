
typedef void* window_handle;

window_handle window_getWindow();

void window_pollEvents(window_handle window);

bool window_shouldClose(window_handle window);

void window_cleanup(window_handle window);

#ifdef USE_X11
void* window_display_ptr(window_handle window);

void* window_window_ptr(window_handle window);

void* window_visual_id_ptr(window_handle window);
#endif

