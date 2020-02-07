/*
 * Venus Graphics Engine
 * Copyright (C) 2020, Wesley Studt
 */

/**
 * @file window.c
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

Display *g_display = NULL;		// X Display
Window g_root = 0;				// Root window of display
zlog_category_t *g_log = NULL;			// "venus" category for zlog

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
	
	// Connect to the X server.
	g_display = XOpenDisplay(NULL);
	if (g_display == NULL) {
		zlog_error(g_log, "Failed to connect to the X Server");
		return VS_FAIL_X_NO_CONNECTION;
	}
	
	// The root window of the display is basically just a window that occupies the entire frame
	g_root = DefaultRootWindow(g_display);
	
	zlog_info(g_log, "Connected to display");
}

int destroy_window(window *win) {
	glXDestroyContext(g_display, *win->context);
	XDestroyWindow(g_display, win->xwin);
}

int venus_terminate() {
	XCloseDisplay(g_display);
	
	zlog_info(g_log, "End");
	zlog_fini();
}

int create_window(window *win) {
	GLint attributes[] = {
		GLX_RGBA,
		GLX_DEPTH_SIZE,
		24,
		GLX_DOUBLEBUFFER,
		None
		};
	XVisualInfo *visual_info;
	XSetWindowAttributes set_window_attributes;
	
	// Gets a visual that fits what was specified in the "attributes" field.
	visual_info = glXChooseVisual(g_display, 0, attributes);
	if (visual_info == NULL) {
		zlog_info(g_log, "No appropriate visual found");
		return VS_FAIL_GLX_NO_VISUAL;
	} else {
		zlog_info(g_log, "Visual %p selected", (void*) visual_info->visualid);
	}
	
	set_window_attributes.colormap = XCreateColormap(g_display, g_root, visual_info->visual, AllocNone);
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
		visual_info->depth,
		InputOutput,
		visual_info->visual,
		CWColormap | CWEventMask,
		&set_window_attributes
	);
	
	win->context = malloc(sizeof(GLXContext));
	
	// Now we create a context for GL to work with. This is an incredibly crucial part of setting up the window.
	*(win->context) = glXCreateContext(g_display, visual_info, NULL, GL_TRUE);
	glXMakeCurrent(g_display, win->xwin, *(win->context));
	
	if (!GLVersion.major) {
		if (!gladLoadGL()) {
			zlog_fatal(g_log, "Failed to load OpenGL");
			return VS_FAIL_GL_NOT_LOADED;
		}
		zlog_info(g_log, "Loaded OpenGL %i.%i", GLVersion.major, GLVersion.minor);
	}
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
	return XFlush(g_display);
}

void venus_begin_loop() {
	
}

/*#endif*/
