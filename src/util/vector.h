;/*
 * Venus Graphics Engine
 * Copyright (C) 2020, Wesley Studt
 */

/**
 * @file vector.h
 */

#ifndef VS_VECTOR_H
#define VS_VECTOR_H

typedef struct {
	unsigned size;
	
	float *data;
} vector;

vector create_vector(unsigned size);
float vector_dot(vector *vec0, vector *vec1);
int vector_multiply(vector *destination, vector *source0, vector *source1);

#endif
