;/** 
 * @file widget.h
 * Venus Graphics Engine
 * Copyright (C) 2020, Wesley Studt
 */

#ifndef VS_WIDGET_H
#define VS_WIDGET_H

#define VS_WIDGET_DRAW		0x0001

#include "../window.h"

/*
 * Model for what a widget must look like
 */
typedef struct {
	unsigned n_children;
	void **children;
	
	void *parent;
	unsigned index;
	
	unsigned width;
	unsigned height;
	
	int (*func)(unsigned type, window *win, void *widget, void** params, unsigned n_params);
	
/*
 * The rest of the data needs to go here. The most important data needs to be at the top so that when a cast is performed the
 * only data kept is the most important
 */

} widget_t;

/**
 * @brief Adds a new child widget
 * 
 * @param parent Pointer to parent widget
 * @param widget Pointer to new child widget
 * 
 * @return Returns an error code
 */
int add_widget(void *parent, void *widget);

/**
 * @brief Inserts a new child widget at the desired index
 * 
 * This operation is not recommended as it is quite slow
 * 
 * @param parent Pointer to parent widget
 * @param index Desired index
 * @param widget Pointer to new child widget
 * 
 * @return Returns an error code
 */
int insert_widget(void *parent, unsigned index, void *widget);

/**
 * @brief Sets a child widget at the desired index
 * 
 * @param parent Pointer to parent widget
 * @param index Desired index
 * @param widget Pointer to new child widget
 * 
 * @return Returns an error code
 */
int set_widget(void *parent, unsigned index, void *widget);

/**
 * @brief Removes a widget
 * 
 * @param parent Pointer to parent widget
 * @param widget Pointer to child widget
 * 
 * @return Returns an error code
 */
int remove_widget(void *parent, void *widget);

/**
 * @brief Removes a widget using index
 * 
 * @param parent Pointer to parent widget
 * @param index Index of the widget to be removed
 * 
 * @return Pointer to the widget that has been removed
 */
void *remove_widget_index(void *parent, unsigned index);

/**
 * @brief Gets a widget from index
 * 
 * @param parent Pointer to parent widget
 * @param index Index of the widget
 * 
 * @return Pointer to the widget
 */
void *get_widget(void *parent, unsigned index);

/**
 * @brief Allocates memory for children
 * 
 * If there are no children yet, this function allocates a new block of memory for the children array. If there are already
 * children, then it reallocates the memory to fit the new size.
 * 
 * @param parent Pointer to widget to be allocated
 * @param size Number of children widgets
 * 
 * @return Returns an error code
 */
int allocate_children(void *widget, unsigned size);

#endif
