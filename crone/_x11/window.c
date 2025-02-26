
#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <core.h>
#include <window.h>

typedef struct XWindow {
    Window window;
    Display *display;
    Atom wmDeleteMessage;
    XVisualInfo visualInfo;
    bool shouldClose;
} XWindow;

const long EVENT_MASK = StructureNotifyMask | KeyPressMask | KeyReleaseMask;

void* window_getWindow() {
    Display *display = XOpenDisplay(nullptr);

    if (display == nullptr) { CRASH("failed to get X display"); }

    int screen = DefaultScreen(display);
    XVisualInfo visualInfo;
    int status = XMatchVisualInfo(display, screen, 32, TrueColor, &visualInfo);

    //printf("%d\n", status);
    //printf("%d\n", visualInfo.bits_per_rgb);

    XSetWindowAttributes attributes;
    attributes.colormap = XCreateColormap(display, RootWindow(display, screen), visualInfo.visual, AllocNone);
    attributes.border_pixel = 0;
    attributes.background_pixel = 0;
    attributes.background_pixmap = None;

    Window window = XCreateWindow(display, RootWindow(display, screen),
        0, 0, // position
        100, 100, // dimension
        1, visualInfo.depth, InputOutput, visualInfo.visual, CWColormap | CWBorderPixel | CWBackPixel | CWBackPixmap, &attributes
    );

    XStoreName(display, window, "tringl c:");

    XMapWindow(display, window);
    XSelectInput(display, window, EVENT_MASK);

    XFlush(display);

    XWindow *ptr = malloc(sizeof(XWindow));
    if (ptr == NULL) { CRASH("failed malloc: xwindow"); }

    ptr -> window = window;
    ptr -> display = display;
    ptr -> wmDeleteMessage = XInternAtom(display, "WM_DELETE_WINDOW", False); // TODO what is this
    ptr -> visualInfo = visualInfo;
    ptr -> shouldClose = false;
    // TODO queue for events

    XSetWMProtocols(display, window, &ptr->wmDeleteMessage, 1); // TODO ??

    return ptr;
}

void window_pollEvents(void *window_void) {
    XWindow *window = window_void;
    XEvent event;

    while (XCheckTypedWindowEvent(window -> display, window -> window, ClientMessage, &event)) {
        if (event.xclient.data.l[0] == window -> wmDeleteMessage) {
            fprintf(stderr, "[_x11/window.c] window close requested\n");
            window -> shouldClose = true;
        }
    }
}

inline bool window_shouldClose(void *window) {
    return ((XWindow*)window)->shouldClose;
}

inline void window_cleanup(void *window_void) {
    XWindow *window = window_void;
    XDestroyWindow(window->display, window->window);
    XCloseDisplay(window->display);
    free(window);
}

inline void* window_display_ptr(void *window_void) {
    return ((XWindow *)window_void)->display;
}

inline void* window_window_ptr(void *window_void) {
    return &(((XWindow *)window_void)->window);
}

inline void* window_visual_id_ptr(void *window_void) {
    return &(((XWindow *)window_void)->visualInfo.visualid);
}

