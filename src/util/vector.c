/*
 * Venus Graphics Engine
 * Copyright (C) 2020, Wesley Studt
 */

/**
 * @file vector.c
 */

#include "vector.h"

#include <stdlib.h>

vector create_vector(unsigned size) {
	vector vec;
	vec.size = size;
	vec.data = malloc(sizeof(float) * size);
}

float vector_dot(vector *vec0, vector *vec1) {
	float out = 0;
	for (unsigned i = 0; i < vec0->size; ++i)
		out += vec0->data[i] * vec1->data[i];
	
	return out;
}

int vector_multiply(vector *destination, vector *source0, vector *source1) {
	
}
