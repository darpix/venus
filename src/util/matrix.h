/** 
 * @file matrix.h
 * Venus Graphics Engine
 * Copyright (C) 2020, Wesley Studt
 */

#ifndef VS_MATRIX_H
#define VS_MATRIX_H

#define VS_MATRIX_ROW		0
#define VS_MATRIX_COLUMN	1

#include <stdlib.h>
#include <string.h>

#include <stdio.h>
unsigned matrix_rows(void *mat);
unsigned matrix_columns(void *mat);
unsigned matrix_size(void *mat);
void matrix_delete(void *mat);

#define VS_DEFINE_MATRIX(TYPE, NAME)														\
typedef TYPE *NAME;																			\
																							\
NAME create_##NAME(unsigned rows, unsigned columns) {										\
	char *source = malloc(sizeof(unsigned) * 2 + rows * columns * sizeof(TYPE));			\
	*((unsigned*) source) = rows;															\
	*((unsigned*) source + 1) = columns;													\
	source += sizeof(unsigned) * 2;															\
	return (NAME) source;																	\
}																							\
																							\
void NAME##_resize(NAME mat, unsigned new_rows, unsigned new_columns) {						\
	char *source = ((char*) mat - sizeof(unsigned) * 2);									\
	source = realloc(source, sizeof(unsigned) * 2 + new_rows * new_columns * sizeof(TYPE));	\
}																							\
																							\
void NAME##_add(NAME dest, NAME src0, NAME src1) {											\
	unsigned size = matrix_size(src0);														\
	for (unsigned i = 0; i < size; ++i)														\
		dest[i] = src0[i] + src1[i];														\
}																							\
																							\
void NAME##_subtract(NAME dest, NAME src0, NAME src1) {										\
	unsigned size = matrix_size(src0);														\
	for (unsigned i = 0; i < size; ++i)														\
		dest[i] = src0[i] - src1[i];														\
}																							\
																							\
void NAME##_negate(NAME dest, NAME src) {													\
	unsigned size = matrix_size(src);														\
	for (unsigned i = 0; i < size; ++i)														\
		dest[i] = -(src[i]);																\
}																							\
																							\
void NAME##_scalar_multiply(NAME dest, NAME src, TYPE scalar) {								\
	unsigned size = matrix_size(src);														\
	for (unsigned i = 0; i < size; ++i)														\
		dest[i] = src[i] * scalar;															\
}																							\
																							\
void NAME##_transpose(NAME dest, NAME src) {												\
	unsigned rows = matrix_rows(src);														\
	unsigned columns = matrix_columns(src);													\
	if (dest == src) {																		\
		for (unsigned r = 0; r < rows; ++r)	{												\
			for (unsigned c = 0; c < columns; ++c) {										\
				TYPE tmp = src[r * columns + c];											\
				src[r * columns + c] = src[c * rows + r];									\
				src[c * rows + r] = tmp;													\
			}																				\
		}																					\
	} else {																				\
		for (unsigned r = 0; r < rows; ++r)													\
			for (unsigned c = 0; c < columns; ++c)											\
				dest[r * columns + c] = src[c * rows + r];									\
	}																						\
}																							\
																							\
void NAME##_multiply(NAME dest, NAME src0, NAME src1) {										\
	unsigned little = matrix_rows(src0);													\
	unsigned size = matrix_columns(src0);													\
	if (little > size) {																	\
		NAME##_multiply(dest, src1, src0);													\
		return;																				\
	}																						\
	for (unsigned r = 0; r < little; ++r)	{												\
		for (unsigned c = 0; c < little; ++c) {												\
			TYPE dot = 0;																	\
			for (unsigned i = 0; i < size; ++i) {											\
				dot += src0[r * size + i] * src1[i * size + c];								\
			}																				\
			dest[r * little + c] = dot;														\
		}																					\
	}																						\
}																							\
																							\
TYPE NAME##_determinant(NAME matrix) {														\
	unsigned size = matrix_rows(matrix);													\
	unsigned columns = matrix_columns(matrix);												\
	if (size != columns)																	\
		return 0;																			\
	printf("size is %i\n", size);															\
	if (size == 2)																			\
		return (matrix[0] * matrix[3]) - (matrix[1] * matrix[2]);							\
	zlog_info(g_log, "creating submatrix");													\
	NAME submatrix = create_##NAME(size - 1, size - 1);										\
	TYPE determinant = 0;																	\
	printf("got here\n");																	\
	for (unsigned a = 0; a < size; ++a) {													\
		for (unsigned b = 0; b < size - 1; ++b) {											\
			for (unsigned c = 1; c < size; ++c) {											\
				if (b < a)																	\
					submatrix[b * (size - 1) + (c - 1)] = matrix[c * size + b];				\
				else																		\
					submatrix[b * (size - 1) + (c - 1)] = matrix[c * size + b + 1];			\
			}																				\
		}																					\
		if (a % 2 == 0)																		\
			determinant += NAME##_determinant(submatrix);									\
		else																				\
			determinant -= NAME##_determinant(submatrix);									\
	}																						\
	zlog_info(g_log, "freeing submatrix");													\
	matrix_delete(submatrix);																\
	return determinant;																		\
}																							\
																							\
void NAME##_set_blank(NAME matrix) {														\
	unsigned size = matrix_size(matrix);													\
	for (unsigned i = 0; i < size; ++i)														\
		matrix[i] = 0;																		\
}																							\
																							\
void NAME##_set_identity(NAME matrix) {														\
	unsigned rows = matrix_rows(matrix);													\
	unsigned columns = matrix_columns(matrix);												\
	for (unsigned r = 0; r < rows; ++r)														\
		for (unsigned c = 0; c < columns; ++c)												\
			if (c == r)																		\
				matrix[r * columns + c] = 1;												\
			else																			\
				matrix[r * columns + c] = 0;												\
}

#endif
