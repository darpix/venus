;/*
 * Venus Graphics Engine
 * Copyright (C) 2020, Wesley Studt
 */

/**
 * @file vector.h
 */

#ifndef VS_VECTOR_H
#define VS_VECTOR_H

#include <stdlib.h>
#include <string.h>

unsigned vec_size(void *vec) {
	unsigned *size = vec;
	return *(--size);
}

void vec_delete(void *vec) {
	unsigned *source = vec;
	free(--source);
}

#ifdef __cplusplus
extern "C" {
#endif

#define VS_DEFINE_VECTOR(TYPE, NAME)														\
typedef TYPE *NAME;																			\
																							\
NAME create_##NAME(unsigned size) {															\
	char *source = malloc(sizeof(unsigned) + sizeof(TYPE) * size);							\
	*((unsigned*) source) = size;															\
	source += sizeof(unsigned);																\
	return (NAME) source;																	\
}																							\
																							\
void NAME##_resize(NAME vec, unsigned new_size) {											\
	char *source = ((char*) vec - sizeof(unsigned));										\
	source = realloc(source, sizeof(unsigned) + new_size * sizeof(TYPE));					\
}																							\
																							\
void NAME##_add(NAME dest, NAME src0, NAME src1) {											\
	for (unsigned i = 0; i < vec_size(src0); ++i)											\
		dest[i] = src0[i] + src1[i];														\
}																							\
																							\
void NAME##_subtract(NAME dest, NAME src0, NAME src1) {										\
	for (unsigned i = 0; i < vec_size(src0); ++i)											\
		dest[i] = src0[i] + src1[i];														\
}																							\
																							\
void NAME##_cross(NAME dest, NAME src0, NAME src1) {										\
	if (src0 == dest) {																		\
		NAME cross = create_##NAME(3);														\
		memcpy(((char*) cross) - sizeof(unsigned), ((char*) src0) - sizeof(unsigned),		\
				sizeof(unsigned) + sizeof(TYPE) * 3);										\
		dest[0] = (src0[1] * src1[2]) - (src0[2] * src1[1]);								\
		dest[1] = (src0[2] * src1[0]) - (src0[0] * src1[2]);								\
		dest[2] = (src0[0] * src1[1]) - (src0[1] * src1[0]);								\
		vec_delete(dest);																	\
		dest = cross;																		\
		return;																				\
	}																						\
	if (src1 == dest) {																		\
		NAME cross = create_##NAME(3);														\
		memcpy(((char*) cross) - sizeof(unsigned), ((char*) src1) - sizeof(unsigned),		\
				sizeof(unsigned) + sizeof(TYPE) * 3);										\
		dest[0] = (src0[1] * src1[2]) - (src0[2] * src1[1]);								\
		dest[1] = (src0[2] * src1[0]) - (src0[0] * src1[2]);								\
		dest[2] = (src0[0] * src1[1]) - (src0[1] * src1[0]);								\
		vec_delete(dest);																	\
		dest = cross;																		\
		return;																				\
	}																						\
	dest[0] = (src0[1] * src1[2]) - (src0[2] * src1[1]);									\
	dest[1] = (src0[2] * src1[0]) - (src0[0] * src1[2]);									\
	dest[2] = (src0[0] * src1[1]) - (src0[1] * src1[0]);									\
}																							\
																							\
void NAME##_multiply(NAME dest, NAME src, TYPE scalar) {									\
	for (unsigned i = 0; i < vec_size(src); ++i)											\
		dest[i] = dest[i] * scalar;															\
}																							\
																							\
TYPE NAME##_dot(NAME src0, NAME src1) {														\
	TYPE dot = 0;																			\
	for (unsigned i = 0; i < vec_size(src0); ++i)											\
		dot += src0[i] * src1[i];															\
	return dot;																				\
}
#ifdef __cplusplus
}
#endif
#endif
