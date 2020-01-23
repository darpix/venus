/*
 * Venus Graphics Engine
 * Copyright (C) 2020, Wesley Studt
 */

/**
 * @file venus_common.h
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

#define VS_SUCCESS 						0x0001
#define VS_FAILURE 						0x0000



#define VS_FAIL_VENUS					0x3000
#define VS_FAIL_VENUS_MATRIX_SIZE		0x3001 // Matrices that are having operations performed on them must be the same size



#define VS_FAIL_ZLOG					0x4000
#define VS_FAIL_ZLOG_NOT_LOADED			0x4001
#define VS_FAIL_ZLOG_MISSING_CATEGORY	0x4002



#define VS_FAIL_X						0x1000
#define VS_FAIL_X_NO_CONNECTION			0x1001



#define VS_FAIL_GL						0x2000
#define VS_FAIL_GL_NOT_LOADED			0x2001



#define VS_FAIL_GLX						0x5000
#define VS_FAIL_GLX_NO_VISUAL			0x5001

#ifdef VS_COMPILE_DO_NOT_PRINT_ERROR
#define vs_err(ERR) return ERR
#else
#define vs_err(ERR) {zlog_error(g_log, #ERR); return ERR;}
#endif

#endif
