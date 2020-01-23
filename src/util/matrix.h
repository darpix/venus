/*
 * Venus Graphics Engine
 * Copyright (C) 2020, Wesley Studt
 */

/**
 * @file matrix.h
 */

#ifndef VS_MATRIX_H
#define VS_MATRIX_H

#define VS_MATRIX_ROW		0
#define VS_MATRIX_COLUMN	1

#include "vector.h"

typedef struct {
	unsigned rows;
	unsigned columns;
	
	float *data;
} matrix;

matrix create_matrix(unsigned rows, unsigned columns);
void resize_matrix(matrix *matrix0, unsigned new_rows, unsigned new_columns);

int matrix_add(matrix *destination, matrix *source0, matrix *source1);
int matrix_multiply(matrix *destination, matrix *source0, matrix *source1);
int matrix_copy(matrix *destination, matrix *source);
int matrix_subtract(matrix *destination, matrix *source0, matrix *source1);
int matrix_multiply_scalar(matrix *destination, matrix *source, float scalar);
int matrix_transpose(matrix *destination, matrix *source);
int matrix_negate(matrix *destination, matrix *source);
void print_matrix(matrix *mat);

#endif
