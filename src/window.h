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

typedef struct {
	/// The number of child widgets
	unsigned n_children;
	
	/// Child widgets
	void **children;
	
	/// Pointer to the window's unique GLXContext
	__glx_context *context;
	
	/// X window
	__x_win xwin;
} window;

/**
 * @brief Creates a new window
 * 
 * This creates a new X window and binds a GL context to that window.
 * 
 * @param win Pointer to window
 * 
 * @return Returns whether it was successful or not
 */
int create_window(window *win);

/**
 * @brief Destroys a window
 * 
 * You should always destroy any window you create in order to ensure that no memory is leaked.
 * 
 * @param win Pointer to window
 * 
 * @Return Returns whether it was successful or not
 */
int destroy_window(window *win);

/**
 * @brief Sets a window's title
 * 
 * @param win Pointer to window
 * @param title The desired title
 * 
 * @return Returns whether it was successful or not
 */
int set_title(window *win, char* title);

/**
 * @brief Sets a window's color
 * 
 * @param win Pointer to window
 * @param color The window's color. Only the first three values, the red, green, and blue values, are used.
 * 
 * @return Returns whether it was successful or not
 */
int set_background_color(window *win, color color);

/**
 * @brief Shows a window
 * 
 * This requests X to map the window
 * 
 * @param win Pointer to window
 * 
 * @return Returns whether it was successful or not
 */
int show(window *win);

/**
 * @brief Hides a window
 * 
 * This requests X to unmap the window
 * 
 * @param win Pointer to window
 * 
 * @return Returns whether it was successful or not
 */
int hide(window *win);

/**
 * @brief Swaps the framebuffers and clears the draw buffer
 * 
 * TODO This is just a temporary function. I will delete it later because the dev does not need access to the GL buffers
 * 
 * @return Returns whether it was successful or not
 */

int swap_buffers(window *win);

#endif
