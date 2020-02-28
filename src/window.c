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

// OpenGL Extension to the X Window System
#include <GL/glx.h>

#include <stdlib.h>

#include "venus_common.h"
#include "engine/graphics.h"
#include "toolkit/theme.h"

int create_window(window *win) {
	
	// Attributes for XVisualInfo
	int attributes[] = {
		GLX_X_RENDERABLE,		True,
		GLX_DRAWABLE_TYPE,		GLX_WINDOW_BIT,
		GLX_RENDER_TYPE,		GLX_RGBA_BIT,
		GLX_X_VISUAL_TYPE,		GLX_TRUE_COLOR,
		GLX_RED_SIZE,			8,
		GLX_GREEN_SIZE,			8,
		GLX_BLUE_SIZE,			8,
		GLX_ALPHA_SIZE,			8,
		GLX_DEPTH_SIZE,			24,
		GLX_STENCIL_SIZE,		8,
		GLX_DOUBLEBUFFER,		True,
		None
	};
	
	GLXFBConfig framebuffer;
	XVisualInfo *visual_info = glx_get_visual(attributes, &framebuffer);
	if (visual_info == NULL) {
		zlog_info(g_log, "No appropriate visual found");
		return VS_FAILURE;
	} else {
		zlog_info(g_log, "Visual %p selected", (void*) visual_info->visualid);
	}
	
	XSetWindowAttributes set_window_attributes;
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
	*(win->context) = glx_make_context(visual_info, framebuffer, NULL, GL_TRUE);
	glx_make_current(win);
	XFree(visual_info);

	if (!GLVersion.major) {
		if (!gladLoadGL()) {
			zlog_fatal(g_log, "Failed to load OpenGL");
			return VS_FAILURE;
		}
		zlog_info(g_log, "Loaded OpenGL %i.%i", GLVersion.major, GLVersion.minor);
	}
	return VS_SUCCESS;
}

int destroy_window(window *win) {
	glXMakeCurrent(g_display, None, NULL);
	glXDestroyContext(g_display, *(win->context));
	XDestroyWindow(g_display, win->xwin);
	XFree(win->context);
}

int set_title(window *win, char *title) {
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

int swap_buffers(window *win) {
	glXSwapBuffers(g_display, win->xwin);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return VS_SUCCESS;
}

/*#endif*/

