/**
 * @file window.h 
 * Venus Graphics Engine
 * Copyright (C) 2020, Wesley Studt
 */

#ifndef VS_WINDOW_H
#define VS_WINDOW_H

#include "venus.h"

#include "util/types.h"

typedef unsigned long __x_win;
typedef struct __GLXcontextRec *__glx_context;

/**
 * @brief Structure that contains the basic building blocks for each venus window.
 * 
 * It carries an X Window and a pointer to it's own separate GLXContext that can be used to draw on the window.
 */

typedef struct {/// The number of child widgets
	unsigned n_children;
	
	/// Child widgets
	void **children;
	
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
 * @param win Pointer to window
 * 
 * @return Returns an error code. Can be VS_FAIL_GLX_NO_VISUAL or VS_FAIL_GL_NOT_LOADED
 */
int create_window(window *win);

/**
 * @brief Destroys a window
 * 
 * You should always destroy any window you create in order to ensure that no memory is leaked.
 * 
 * @param win Pointer to window
 * 
 * @Return Returns an error code.
 */
int destroy_window(window *win);

/**
 * @brief Sets a window's title
 * 
 * @param win Pointer to window
 * @param title The desired title
 * 
 * @return Returns an error code
 */
int set_title(window *win, char* title);

/**
 * @brief Sets a window's color
 * 
 * @param win Pointer to window
 * @param color The window's color. Only the first three values, the red, green, and blue values, are used.
 * 
 * @return Returns an error code
 */
int set_background_color(window *win, color color);

/**
 * @brief Shows a window
 * 
 * This requests X to map the window
 * 
 * @param win Pointer to window
 * 
 * @return Returns an error code
 */
int show(window *win);

/**
 * @brief Hides a window
 * 
 * This requests X to unmap the window
 * 
 * @param win Pointer to window
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

/**
 * @brief Starts the event loop for any associated venus windows
 * 
 * @return Returns an error code. The error code probably won't matter, but it will tell you if the program crashed or not
 */
int venus_begin_loop();

#endif
