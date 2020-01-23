;/*
 * Venus Graphics Engine
 * Copyright (C) 2020, Wesley Studt
 */

/**
 * @file matrix.c
 */

#include "matrix.h"

#include "../venus_common.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

int check_matrix_size(matrix *mat0, matrix *mat1) {
	if (mat0->rows == mat1->rows && mat0->columns == mat1->columns)
		return VS_SUCCESS;
	return VS_FAILURE;
}

matrix create_matrix(unsigned rows, unsigned columns) {
	matrix mat;
	mat.rows = rows;
	mat.columns = columns;
	mat.data = malloc(sizeof(float) * rows * columns);
	
	return mat;
}

void resize_matrix(matrix *matrix0, unsigned new_rows, unsigned new_columns) {
	matrix0->data = realloc(matrix0->data, sizeof(float) * new_rows * new_columns);
}

int format_matrix(matrix *mat, matrix *format) {
	if (mat->data == NULL) {
		*mat = create_matrix(format->rows, format->columns);
		return VS_SUCCESS;
	}
	return check_matrix_size(mat, format);
}

int matrix_copy(matrix *destination, matrix *source) {
	if (!format_matrix(destination, source))
		vs_err(VS_FAIL_VENUS_MATRIX_SIZE);
	
	for (unsigned i = 0; i < source->rows * source->columns; ++i)
		destination->data[i] = source->data[i];
	return VS_SUCCESS;
}

int matrix_negate(matrix *destination, matrix *source) {
	if (!format_matrix(destination, source))
		vs_err(VS_FAIL_VENUS_MATRIX_SIZE);
	
	for (unsigned i = 0; i < destination->rows * destination->columns; ++i)
		destination->data[i] = -source->data[i];
	return VS_SUCCESS;
}

int matrix_add(matrix *destination, matrix *source0, matrix *source1) {
	if (!check_matrix_size(source0, source1))
		vs_err(VS_FAIL_VENUS_MATRIX_SIZE);
	if (!format_matrix(destination, source0))
		vs_err(VS_FAIL_VENUS_MATRIX_SIZE);
		
	for (unsigned i = 0; i < destination->rows * destination->columns; ++i)
		destination->data[i] = source0->data[i] + source1->data[i];
	return VS_SUCCESS;
}

int matrix_subtract(matrix *destination, matrix *source0, matrix *source1) {
	if (!check_matrix_size(source0, source1))
		vs_err(VS_FAIL_VENUS_MATRIX_SIZE);
	if (!format_matrix(destination, source0))
		vs_err(VS_FAIL_VENUS_MATRIX_SIZE);
	
	for (unsigned i = 0; i < destination->rows * destination->columns; ++i)
		destination->data[i] = source0->data[i] - source1->data[i];
	return VS_SUCCESS;
}

int matrix_multiply_scalar(matrix *destination, matrix *source, float scalar) {
	if (!format_matrix(destination, source))
		vs_err(VS_FAIL_VENUS_MATRIX_SIZE);
	
	for (unsigned i = 0; i < destination->rows * destination->columns; ++i)
		destination->data[i] = source->data[i] * scalar;
	return VS_SUCCESS;
}

int matrix_transpose(matrix *destination, matrix *source) {
	if (destination == source) {
		
		unsigned rows = destination->rows;
		destination->rows = destination->columns;
		destination->columns = rows;
		
	} else {
		if (destination->data == NULL) {
			*destination = create_matrix(source->columns, source->rows);
		} else {
			if (!(destination->rows == source->columns && destination->columns == source->rows))
				vs_err(VS_FAIL_VENUS_MATRIX_SIZE);
		}
		
		memcpy(destination->data, source->data, sizeof(float) * source->columns * source->rows);
		return VS_SUCCESS;
	}
}

void print_matrix(matrix *mat) {
	for (int i = 0; i < mat->rows; ++i) {
		for (int j = 0; j < mat->columns; ++j) {
			printf("%i   ", (int) mat->data[i * mat->columns + j]);
		}
		printf("\n");
	}
	fflush(stdout);
}
