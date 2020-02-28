/** 
 * @file panel.c
 * Venus Graphics Engine
 * Copyright (C) 2020, Wesley Studt
 */
 
#include "panel.h"

#include "../../venus_common.h"
#include "../../window.h"
#include "../../engine/graphics.h"

#include "../theme.h"

int call_panel(unsigned type, window *win, void *widget, void** params, unsigned n_params) {
	if (type == VS_WIDGET_DRAW)
		return g_theme.draw_panel(widget, win, params, n_params);
	return VS_FAIL_VENUS;
}

vpanel *create_panel() {
	vpanel *panel = malloc(sizeof(vpanel));
	
	panel->func = call_panel;
	
	return panel;
}

