;/** 
 * @file text_field.h
 * Venus Graphics Engine
 * Copyright (C) 2020, Wesley Studt
 */

#ifndef VS_WIDGET_TEXT_FIELD_H
#define VS_WIDGET_TEXT_FIELD_H

#include "../widget.h"

#define VS_TEXT_FIELD_ID	0x0001

typedef struct {
	unsigned n_children;
	void **children;
	
	void *parent;
	unsigned index;
	
	unsigned width;
	unsigned height;
	
	int (*func)(unsigned type, window *win, void *widget, void** params, unsigned n_params);

	char *default_text;
	char *current_text;
} vtext_field;

/**
 * @brief Creates a new text field
 * 
 * @return Returns a new text field
 */
vtext_field *create_text_field();

#endif
