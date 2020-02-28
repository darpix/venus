/**
 * @file venus.h
 * Venus Graphics Engine
 * Copyright (C) 2020, Wesley Studt
 * 
 * @brief The main interface to Venus
 */

#pragma once
#ifndef VS_VENUS_H
#define VS_VENUS_H

/**
 * @brief Initializes venus and the libraries used
 * 
 * This function does a couple of important things. First it initializes zlog, the logging library I have chosen to use.
 * zlog needs to be started first in order to start logging immediately.
 * As well as starting zlog, it also creates a connection to an X Server.
 * It should be noted that this does not initalize OpenGL.
 * 
 * @return Returns whether it was successful or not
 */
int venus_initialize();

/**
 * @brief Terminates venus and does memory clean up
 * 
 * This stops any X connections, destroys GL contexts, and finally, finishes zlog.
 * 
 * @return Returns whether it was successful or not
 */
int venus_terminate();

/**
 * @brief Flushes all X requests
 * 
 * This is only used to tell X to flush the display. It will likely be removed soon because it won't need to exist.
 * 
 * @return Returns whether it was successful or not
 */
int flush();

/**
 * @brief Starts the event loop for any associated venus windows
 * 
 * @return Returns whether it was successful or not
 */
int venus_begin_loop();

#endif
