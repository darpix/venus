/** 
 * @file theme.h
 * @brief Venus theme
 * 
 * Venus Graphics Engine
 * Copyright (C) 2020, Wesley Studt
 */

#include "../window.h"

#include "default_theme.h"

typedef struct {
	int (*draw_text_field)(window *win, void *text_field, void **params, unsigned n_params);
	int (*draw_panel)(window *win, void *panel, void **params, unsigned n_params);
} vtheme;

void set_default_venus_theme(vtheme *theme);
extern vtheme g_theme;
