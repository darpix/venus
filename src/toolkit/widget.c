/** 
 * @file widget.c
 * Venus Graphics Engine
 * Copyright (C) 2020, Wesley Studt
 */

#include "widget.h"

#include <stdlib.h>
#include <string.h>

#include "../venus_common.h"

int add_widget(void *parent, void *widget) {
	widget_t *p = (widget_t*) parent;
	widget_t *w = (widget_t*) widget;
	
	allocate_children(p, p->n_children + 1);
	set_widget(p, p->n_children, w);
	p->n_children++;
	return VS_SUCCESS;
}

int insert_widget(void *parent, unsigned index, void *widget) {
	widget_t *p = (widget_t*) parent;
	widget_t *w = (widget_t*) widget;
	
	allocate_children(p, p->n_children + 1);
	memcpy(p->children[index + 1], p->children[index], p->n_children - (index + 1));
	set_widget(p, index, w);
	p->n_children++;
	return VS_SUCCESS;
}

int set_widget(void *parent, unsigned index, void *widget) {
	widget_t *p = (widget_t*) parent;
	widget_t *w = (widget_t*) widget;
	
	set_widget(p, index, w);
	return VS_SUCCESS;
}

int remove_widget(void *parent, void *widget) {
	widget_t *p = (widget_t*) parent;
	widget_t *w = (widget_t*) widget;
	
	remove_widget_index(p, w->index);
	return VS_SUCCESS;
}

void *remove_widget_index(void *parent, unsigned index) {
	widget_t *p = (widget_t*) parent;
	widget_t *w = get_widget(p, index);
	
	memcpy(p->children[index], p->children[index + 1], p->n_children - (index + 1));
	allocate_children(p, p->n_children - 1);
	p->n_children--;
	return w;
}

void *get_widget(void *parent, unsigned index) {
	widget_t *p = (widget_t*) parent;
	return p->children + index;
}

int allocate_children(void *widget, unsigned size) {
	widget_t *w = (widget_t*) widget;
	w->children = realloc(w->children, sizeof(w) * size);
	return VS_SUCCESS;
}
