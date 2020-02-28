;/** 
 * @file panel.h
 * Venus Graphics Engine
 * Copyright (C) 2020, Wesley Studt
 */

#ifndef VS_WIDGET_TEXT_FIELD_H
#define VS_WIDGET_TEXT_FIELD_H

#include "../widget.h"

#define VS_PANEL_ID			0x0002

typedef struct {
	unsigned n_children;
	void **children;
	
	void *parent;
	unsigned index;
	
	unsigned width;
	unsigned height;
	
	int (*func)(unsigned type, window *win, void *widget, void** params, unsigned n_params);
} vpanel;

/**
 * @brief Creates a new panel
 * 
 * @return Returns a new panel
 */
vpanel *create_panel();

#endif

