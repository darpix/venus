/**
 * @file window.c
 * Venus Graphics Engine
 * Copyright (C) 2020, Wesley Studt
 */

#include "window.h"

/*
 * Currently I'm only working on Linux support... The goal is to get a lot of the higher level stuff together and then at that
 * point I can afford to rework the low-level stuff to work on Windows. I don't have access to a Mac so who knows when I'll
 * have the opportunity to write code for Macs
 */

/* This option should really be set in the compiler options but this works just as well for now
#define VS_COMPILE_LINUX
#ifdef VS_COMPILE_LINUX*/

/*
 * https://github.com/Dav1dde/glad
 * GL/GLES/EGL/GLX/WGL Loader-Generator based on the official specs.
 * by David Herberth
 */
#include <glad/glad.h>

// Main X11 headers
#include <X11/Xlib.h>
#include <X11/Xutil.h>

// "OpenGL Extension to the X Window System" - basically makes an OpenGL context I can connect with an XWindow object
#include <GL/glx.h>

#include <stdlib.h>

#include "venus_common.h"
#include "toolkit/theme.h"

Display *g_display = NULL;		// X Display
Window g_root = 0;				// Root window of display
zlog_category_t *g_log = NULL;			// "venus" category for zlog
XVisualInfo *g_visual_info = NULL;

window *current_window;

GLXContext context(window *window) {
	if (current_window == window) {
		return *window->context;
	} else {
		glXMakeCurrent(g_display, window->xwin, *window->context);
		current_window = window;
		return *window->context;
	}
}

int venus_initialize() {

	/*
	 * I've chosen to use zlog as a logger because it is easily configurable, accessible, and very fast.
	 * There should be some way to keep the logger going after venus has been terminated if desired, but I haven't decided how
	 */

	int zlog_init_code = zlog_init("../.zlog.conf");
	if (zlog_init_code) {
		printf("Failed to start zlog\n");
		return VS_FAIL_ZLOG_NOT_LOADED;
	}

	// Grab the "venus" category from the zlog config. If the category doesn't exist the user needs to fix that!
	g_log = zlog_get_category("venus");
	if (!g_log) {
		printf("Failed to get zlog category 'venus'\n");
		zlog_fini();
		return VS_FAIL_ZLOG_MISSING_CATEGORY;
	}

	// Delete the last logfile since we're using a new one now
	remove("latest.log");
	zlog_info(g_log, "Starting...");

	// Initializes the default theme
	set_default_venus_theme(&g_theme);

	// Connect to the X server.
	g_display = XOpenDisplay(NULL);
	if (g_display == NULL) {
		zlog_error(g_log, "Failed to connect to the X Server");
		return VS_FAIL_X_NO_CONNECTION;
	}

	// The root window of the display is basically just a window that occupies the entire frame
	g_root = DefaultRootWindow(g_display);

	zlog_info(g_log, "Connected to display");

	GLint attributes[] = {
		GLX_RGBA,
		GLX_DEPTH_SIZE,
		24,
		GLX_DOUBLEBUFFER,
		None
		};

	// Gets a visual that fits what was specified in the "attributes" field.
	// WARNING This line is causing a memory leak (on my machine, at least). I'm having a lot of trouble figuring out how to stop the leak.
	// TODO For now I really can't worry about it because it's taking too much time and it seems to be completely out of my control.
	g_visual_info = glXChooseVisual(g_display, DefaultScreen(g_display), attributes);


	if (g_visual_info == NULL) {
		zlog_info(g_log, "No appropriate visual found");
		return VS_FAIL_GLX_NO_VISUAL;
	} else {
		zlog_info(g_log, "Visual %p selected", (void*) (g_visual_info->visualid));
	}
}

int destroy_window(window *win) {
	glXMakeCurrent(g_display, None, NULL);
	glXDestroyContext(g_display, *(win->context));
	XDestroyWindow(g_display, win->xwin);
	XFree(win->context);
}

int venus_terminate() {
	XCloseDisplay(g_display);
	XFree(g_visual_info);

	zlog_info(g_log, "End");
	zlog_fini();
}

int create_window(window *win) {
	XSetWindowAttributes set_window_attributes;
	set_window_attributes.colormap = XCreateColormap(g_display, g_root, g_visual_info->visual, AllocNone);
	set_window_attributes.event_mask = ExposureMask | KeyPressMask | PointerMotionMask;

	// Create the window
	win->xwin = XCreateWindow(
		g_display,
		g_root,
		0,
		0,
		1242,
		768,
		0,
		g_visual_info->depth,
		InputOutput,
		g_visual_info->visual,
		CWColormap | CWEventMask,
		&set_window_attributes
	);

	win->context = malloc(sizeof(GLXContext));

	// Now we create a context for GL to work with. This is an incredibly crucial part of setting up the window.
	*(win->context) = glXCreateContext(g_display, g_visual_info, NULL, GL_TRUE);
	context(win);

	if (!GLVersion.major) {
		if (!gladLoadGL()) {
			zlog_fatal(g_log, "Failed to load OpenGL");
			return VS_FAIL_GL_NOT_LOADED;
		}
		zlog_info(g_log, "Loaded OpenGL %i.%i", GLVersion.major, GLVersion.minor);
	}
	context(win);
	return VS_SUCCESS;
}

int set_title(window *win, char* title) {
	XStoreName(g_display, win->xwin, title);
}

int set_background_color(window *win, color color) {
	glClearColor((float) color[0] / 255.0f,(float) color[1] / 255.0f, (float) color[2] / 255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glXSwapBuffers(g_display, win->xwin);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glXSwapBuffers(g_display, win->xwin);
	return VS_SUCCESS;
}

int show(window *win) {
	XMapWindow(g_display, win->xwin);
	return VS_SUCCESS;
}

int hide(window *win) {
	XUnmapWindow(g_display, win->xwin);
	return VS_SUCCESS;
}

int flush() {
	return !XFlush(g_display);
}

int venus_begin_loop() {
	
}

/*#endif*/
