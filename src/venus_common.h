/**
 * @file venus_common.h
 * Venus Graphics Engine
 * Copyright (C) 2020, Wesley Studt
 */

#pragma once
#ifndef VS_VENUS_COMMON_H
#define VS_VENUS_COMMON_H

/*
 * https://github.com/HardySimpson/zlog
 * zlog is a reliable, high-performance, thread safe, flexible, clear-model, pure C logging library.
 * by Hardy Simpson
 */

#include <zlog.h>

extern zlog_category_t *g_log;

#define VS_FALSE 				0
#define VS_TRUE 				1

#define VS_SUCCESS 						0x0000
#define VS_FAILURE 						0x0001



#define VS_FAIL_VENUS					0b0010000000000000000000000000000
#define VS_FAIL_VENUS_MATRIX_SIZE		(0x0001 | VS_FAIL_VENUS)



#define VS_FAIL_ZLOG					0b0100000000000000000000000000000
#define VS_FAIL_ZLOG_NOT_LOADED			(0x0001 | VS_FAIL_ZLOG)
#define VS_FAIL_ZLOG_MISSING_CATEGORY	(0x0002 | VS_FAIL_ZLOG)



#define VS_FAIL_X						0b1000000000000000000000000000000
#define VS_FAIL_X_NO_CONNECTION			(0x0001 | VS_FAIL_X)



#define VS_FAIL_GL						0b0001000000000000000000000000000
#define VS_FAIL_GL_NOT_LOADED			(0x0001 | VS_FAIL_GL)



#define VS_FAIL_GLX						0b0000100000000000000000000000000
#define VS_FAIL_GLX_NO_VISUAL			(0x0001 | VS_FAIL_GLX)

#ifdef VS_COMPILE_DO_NOT_PRINT_ERROR
#define vs_err(ERR) return ERR
#else
#define vs_err(ERR) {zlog_error(g_log, #ERR); return ERR;}
#endif

#define vs_has_err(X) !(X)

#endif
