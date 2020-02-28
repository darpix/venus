/**
 * @file graphics.h
 * Venus Graphics Engine
 * Copyright (C) 2020, Wesley Studt
 */

#ifndef VS_GRAPHICS_H
#define VS_GRAPHICS_H

#include <glad/glad.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/glx.h>

#include "../window.h"
#include "../util/types.h"

extern Display *g_display;
extern Window 	g_root;
extern window  *g_current_window;

/**
 * @brief Load a shader into OpenGL
 * 
 * @param shader_type The type of shader. Can be GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, or GL_GEOMETRY_SHADER
 * @param shader_source The raw source code to compile
 * 
 * @return Returns the shaderID or 0 if the shader compilation failed.
 */
unsigned gl_create_shader(int shader_type, const char **shader_source);

/**
 * @brief Load a buffer into OpenGL
 * 
 * Loads a stream of information into OpenGL.
 * 
 * @param data Pointer to the start of the data
 * @param bytecount Number of bytes to load
 * 
 * @return Returns the bufferID
 */
unsigned gl_load_buffer(void *data, unsigned bytecount);



/**
 * @brief Check support for a GLX extension
 * 
 * @param ext_list List of GLX extensions
 * @param extension Extension to locate
 * 
 * @return Returns whether or not the extension is supported
 */
int glx_check_support(const char *ext_list, const char *extension);

/**
 * @brief Gets a set of visual info
 * 
 * @param attributes The desired attributes for the new visual
 * @param framebuffer Memory address where the framebuffer configuration will be saved
 * 
 * @return Returns a set of visual info
 */
XVisualInfo *glx_get_visual(int *attributes, GLXFBConfig *framebuffer);

/**
 * @brief Sets the current window
 * 
 * This choose which window OpenGL will render to.
 * 
 * @param window The desired venus window
 * 
 * @return Returns the previous current window
 */
GLXContext glx_make_current(window *window);

/**
 * @brief Creates a new GLXContext
 * 
 * @param visual_info Set of visual info
 * @param framebuffer Framebuffer configuration
 * @param sharelist Share list
 * @param direct Direct
 * 
 * @return Returns a new GLX context
 */
GLXContext glx_make_context(XVisualInfo *visual_info, GLXFBConfig framebuffer, GLXContext sharelist, int direct);

void graph_test(window *win);

#endif
