/**
 * @file graphics.c
 * Venus Graphics Engine
 * Copyright (C) 2020, Wesley Studt
 */

#include "graphics.h"

#include "../venus_common.h"

#include <stdlib.h>
#include <math.h>

#include "../toolkit/widget.h"

#define GLX_CONTEXT_MAJOR_VERSION_ARB       0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB       0x2092

Display *g_display = NULL;		// X Display
Window g_root = 0;				// Root window of display
window *g_current_window = NULL;

int (*g_error_callback)(void *win, unsigned err);

GLXContext glx_make_current(window *window) {
	if (g_current_window == window) {
		return *window->context;
	} else {
		glXMakeCurrent(g_display, window->xwin, *window->context);
		g_current_window = window;
		return *window->context;
	}
}

unsigned gl_create_shader(int shader_type, const char **shader_source) {
	unsigned sh = glCreateShader(shader_type);
	glShaderSource(sh, 1, shader_source, NULL);
	glCompileShader(sh);
	
	int success;
	char log[512];
	glGetShaderiv(sh, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(sh, 512, NULL, log);
		glDeleteShader(sh);
		zlog_error(g_log, "vertex shader compilation failed\n%s", log);
		return 0;
	}
	return sh;
}

unsigned gl_load_buffer(void *data, unsigned bytecount) {
	unsigned buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, bytecount, data, GL_STATIC_DRAW);
	return buffer;
}

int glx_check_support(const char *ext_list, const char *extension) {
	const char *start;
	const char *where;
	const char *terminate;
	
	where = strchr(extension, ' ');
	if (where || *extension == '\0')
		return 0;
	
	for (start = ext_list;;) {
		where = strstr(start, extension);
		
		if (!where)
			break;
		
		terminate = where + strlen(extension);
		
		if (where == start || *(where - 1) == ' ')
			if (*terminate == ' ' || *terminate == '\0')
				return 1;
		start = terminate;
	}
	return 0;
}

XVisualInfo *glx_get_visual(int *attributes, GLXFBConfig *framebuffer) {
	zlog_debug(g_log, "Getting framebuffer via GLX...");
	int glx_version_major;
	int glx_version_minor;
	if (!glXQueryVersion(g_display, &glx_version_major, &glx_version_minor) ||
		((glx_version_major == 1) && (glx_version_minor < 3)) || (glx_version_major < 1)
	) {
		zlog_error(g_log, "Invalid GLX version. (%i,%i)", glx_version_major, glx_version_minor);
		return NULL;
	}
	
	int framebuffer_count;
	GLXFBConfig *framebuffer_configs = glXChooseFBConfig(g_display, DefaultScreen(g_display), attributes, &framebuffer_count);
	
	if (!framebuffer_configs) {
		zlog_error(g_log, "Failed to get a framebuffer configuration with the desired attributes");
		return NULL;
	}
	
	zlog_debug(g_log, "Grabbed matching framebuffer configurations.");
	int best_config = -1;
	int worst_config = -1;
	int best_samples = -1;
	int worst_samples = 999;
	for (int i = 0; i < framebuffer_count; ++i) {
		XVisualInfo *buffer_visual_info = glXGetVisualFromFBConfig(g_display, framebuffer_configs[i]);
		
		if (buffer_visual_info) {
			int sample_buffer;
			int samples;
			
			glXGetFBConfigAttrib(g_display, framebuffer_configs[i], GLX_SAMPLE_BUFFERS, &sample_buffer);
			glXGetFBConfigAttrib(g_display, framebuffer_configs[i], GLX_SAMPLES, &samples);
			
			zlog_debug(g_log, "Matching framebuffer configuration %d, visual ID %p: GLX_SAMPLE_BUFFERS = %d, GLX_SAMPLES = %d",
				i, (void*) buffer_visual_info->visualid, sample_buffer, samples
			);
			
			if (best_config < 0 || (sample_buffer && samples > best_samples)) {
				best_config = i;
				best_samples = samples;
			}
			if (worst_config < 0 || (sample_buffer && samples < worst_samples)) {
				worst_config = i;
				worst_samples = samples;
			}
		}
		
		XFree(buffer_visual_info);
	}
	
	*framebuffer = framebuffer_configs[best_config];
	XFree(framebuffer_configs);
	return glXGetVisualFromFBConfig(g_display, *framebuffer);
}


int g_context_err = 0;
int glx_context_error(Display *display, XErrorEvent *event) {
    g_context_err = 1;
    return 0;
}

GLXContext glx_make_context(XVisualInfo *visual_info, GLXFBConfig framebuffer, GLXContext sharelist, int direct) {
	const char *extensions = glXQueryExtensionsString(g_display, DefaultScreen(g_display));
	PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB =
		(PFNGLXCREATECONTEXTATTRIBSARBPROC) glXGetProcAddressARB((const GLubyte*) "glXCreateContextAttribsARB");
	
	GLXContext context = NULL;
	int (*glx_old_error_handler)(Display*, XErrorEvent*) = XSetErrorHandler(&glx_context_error);
	
	if (!glx_check_support(extensions, "GLX_ARB_create_context") || !glXCreateContextAttribsARB) {
		zlog_info(g_log, "glXCreateContextAttribsARB() not found. Reverting to deprecated GLX context.");
		context = glXCreateNewContext(g_display, framebuffer, GLX_RGBA_TYPE, 0, True);
	} else {
		int context_attribs[] = {
			GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
			GLX_CONTEXT_MINOR_VERSION_ARB, 5,
			//GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
			None
		};
		context = glXCreateContextAttribsARB(g_display, framebuffer, 0, True, context_attribs);
		
		XSync(g_display, False);
		if (!g_context_err && context) {
			zlog_info(g_log, "Created new context");
			return context;
		} else {
			context_attribs[1] = 1;
			context_attribs[3] = 0;
			g_context_err = 0;
			zlog_info(g_log, "Failed to create modern context. Reverting to deprecated GLX context.");
			context = glXCreateContextAttribsARB(g_display, framebuffer, 0, True, context_attribs);
		}
	}
	XSync(g_display, False);
	XSetErrorHandler(glx_old_error_handler);
	
	if (g_context_err || !context) {
		zlog_info(g_log, "Failed to create GLX context");
		return NULL;
	}
	
	if (glXIsDirect(g_display, context)) {
		zlog_info(g_log, "Rendering context directly");
	} else {
		zlog_info(g_log, "Rendering context indirectly");
	}
}

void graph_test(window *win) {
	unsigned int array;
	glGenVertexArrays(1, &array); 
	glBindVertexArray(array);
	
	float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
	};
	
	const char *vsh_src = "#version 450 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
			"}\0";
			
	const char *fsh_src = "#version 450 core\n"
			"out vec4 fragment_color;\n"
			"void main(){\n"
			"	fragment_color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
			"}\0";
	
	unsigned program = glCreateProgram();
	unsigned buffer = gl_load_buffer(vertices, 9 * sizeof(float));
	unsigned vsh = gl_create_shader(GL_VERTEX_SHADER,	&vsh_src);
	unsigned fsh = gl_create_shader(GL_FRAGMENT_SHADER,	&fsh_src);
	glAttachShader(program, vsh);
	glAttachShader(program, fsh);
	glLinkProgram(program);
	glDeleteShader(vsh);
	glDeleteShader(fsh);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glUseProgram(program);
	
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
