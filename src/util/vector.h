/**
 * @file vector.h
 * Venus Graphics Engine
 * Copyright (C) 2020, Wesley Studt
 */

#ifndef VS_VECTOR_H
#define VS_VECTOR_H

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

unsigned vec_size(void *vec);
void vec_delete(void *vec);

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Defines a new vector type and includes function prototypes
 * 
 * In Venus, a vector represents a mathematical vector, not a container like std::vector. The reason for this is that since
 * Venus is a GUI/graphics library, it needs to manipulate points in two-dimensional space and three-dimensional space. This
 * requires functions that can add, multiply, etc., vectors.
 * 
 * @param TYPE The datatype the vector should contain
 * @param NAME The name of the new vector
 */
	
#define VS_INTERNAL_SET_VECTOR(arg0, args...)

#define VS_DEFINE_VECTOR_HEADER(TYPE, NAME)													\
typedef TYPE *NAME;																			\
																							\
NAME create_##NAME(unsigned size);															\
NAME make_##NAME(unsigned size, ...);														\
void NAME##_resize(NAME vec, unsigned new_size);											\
void NAME##_add(NAME dest, NAME src0, NAME src1);											\
void NAME##_subtract(NAME dest, NAME src0, NAME src1);										\
void NAME##_cross(NAME dest, NAME src0, NAME src1);											\
void NAME##_multiply(NAME dest, NAME src, TYPE scalar);										\
TYPE NAME##_dot(NAME src0, NAME src1);														\

/**
 * @brief Provides the source code for a previously defined vector type
 * 
 * The two values need to reflect a vector type defined by VS_DEFINE_VECTOR_HEADER.
 * 
 * @param TYPE The datatype the vector contains
 * @param NAME The name of the vector
 */

#define VS_DEFINE_VECTOR_SOURCE(TYPE, NAME)													\
																							\
NAME create_##NAME(unsigned size) {															\
	char *source = malloc(sizeof(unsigned) + sizeof(TYPE) * size);							\
	*((unsigned*) source) = size;															\
	source += sizeof(unsigned);																\
	return (NAME) source;																	\
}																							\
																							\
NAME make_##NAME(unsigned size, ...) {														\
	NAME vector = create_##NAME(size);														\
																							\
	va_list values;																			\
	va_start(values, size);																	\
	if (sizeof(TYPE) < sizeof(int))															\
		for (unsigned i = 0; i < size; ++i)													\
			vector[i] = (TYPE) va_arg(values, int);											\
	else																					\
		for (unsigned i = 0; i < size; ++i)													\
			vector[i] = va_arg(values, TYPE);												\
	va_end(values);																			\
	return vector;																			\
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
