/* 
 * Venus Graphics Engine
 * Copyright (C) 2020, Wesley Studt
 */

/**
 * @file window.h
 */

#pragma once
#ifndef VS_WINDOW_H
#define VS_WINDOW_H

#include "venus.h"

typedef unsigned long __x_win;
typedef struct __GLXcontextRec *__glx_context;

/**
 * @brief Structure that contains the basic building blocks for each venus window.
 * 
 * It carries an X Window and a pointer to it's own separate GLXContext that can be used to draw on the window.
 */

typedef struct {
	
	/// The width of the window
	int width;
	
	/// The height of the window
	int height;
	
	/// Pointer to the window's unique GLXContext
	__glx_context *context;
	
	/// The actual window
	__x_win xwin;
} window;


/**
 * @brief Initializes venus and the libraries used
 * 
 * This function does a couple of important things. First it initializes zlog, the logging library I have chosen to use.
 * zlog needs to be started first in order to start logging immediately.
 * As well as starting zlog, it also creates a connection to an X Server.
 * It should be noted that this does not initalize OpenGL.
 * 
 * @return Returns an error code. Can be VS_FAIL_ZLOG_NOT_LOADED, VS_FAIL_ZLOG_MISSING_CAT, or VS_FAIL_X_NO_CONNECTION
 */
int venus_initialize();

/**
 * @brief Terminates venus and does memory clean up
 * 
 * This stops any X connections, destroys GL contexts, and finally, finishes zlog.
 * 
 * @return Returns an error code
 */
int venus_terminate();

/**
 * @brief Creates a new window
 * 
 * This creates a new X window and binds a GL context to that window.
 * 
 * @return Returns an error code. Can be VS_FAIL_GLX_NO_VISUAL or VS_FAIL_GL_NOT_LOADED
 */
int create_window(window *win);

/**
 * @brief Shows a window
 * 
 * This requests X to "map" the window
 * 
 * @return Returns an error code
 */
int show(window *win);

/**
 * @brief Hides a window
 * 
 * This requests X to "unmap" the window
 * 
 * @return Returns an error code
 */
int hide(window *win);

/**
 * @brief Flushes all X requests
 * 
 * This is only used to tell X to flush the display. It will likely be removed soon because it won't need to exist.
 * 
 * @return Returns an error code
 */
int flush();

#endif
