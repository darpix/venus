/** 
 * @file text_field.c
 * Venus Graphics Engine
 * Copyright (C) 2020, Wesley Studt
 */
 
#include "text_field.h"

#include "../../venus_common.h"
#include "../../window.h"
#include "../../engine/graphics.h"

#include "../theme.h"

int call_text_field(unsigned type, window *win, void *widget, void** params, unsigned n_params) {
	if (type == VS_WIDGET_DRAW)
		return g_theme.draw_text_field(widget, win, params, n_params);
	return VS_FAIL_VENUS;
}

vtext_field *create_text_field() {
	vtext_field *field = malloc(sizeof(vtext_field));
	
	field->func = call_text_field;
	
	return field;
}
