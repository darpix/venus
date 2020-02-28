/** 
 * @file default_theme.c
 * @brief The default theme for Venus and a good template to design new themes.
 * 
 * Venus Graphics Engine
 * Copyright (C) 2020, Wesley Studt
 */

#include "default_theme.h"

#include "../window.h"
#include "theme.h"

#include "widgets/text_field.h"
#include "widgets/panel.h"

vtheme g_theme;

void set_default_venus_theme(vtheme *theme) {
	theme->draw_text_field = draw_text_field_default;
	theme->draw_panel = draw_panel_default;
}

int draw_text_field_default(window *win, void *text_field, void **params, unsigned n_params) {
	vtext_field *t_f = (vtext_field*) text_field;
	char *string = t_f->current_text;
	
	
}

int draw_panel_default(window *win, void *panel, void **params, unsigned n_params) {
	
}
