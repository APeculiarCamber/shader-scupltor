#pragma once

/*
** RPI Game Architecture Engine
**
** Portions adapted from:
** Viper Engine - Copyright (C) 2016 Velan Studios - All Rights Reserved
**
** This file is distributed under the MIT License. See LICENSE.txt.
*/

#include "ga_vec2f.h"
#include "ga_vec3f.h"

/*
** Floating point 3x3 matrix.
*/
struct ga_mat3f
{
	float data[3][3];

	/*
	** Build an identity matrix.
	*/
	void make_identity();

	/*
	** Build a translation matrix.
	*/
	void make_translation(const ga_vec2f& __restrict t);

	/*
	** Build a uniform scaling matrix.
	*/
	void make_scaling(float s);

	/*
	** Build a rotation about Z matrix.
	*/
	void make_rotation_z(float angle);

	/*
	** Apply translation to the given matrix.
	*/
	void translate(const ga_vec2f& __restrict t);

	/*
	** Apply uniform scaling to the given matrix.
	*/
	void scale(float s);

	/*
	** Apply rotation about Z to the given matrix.
	*/
	void rotate_z(float angle);

	/*
	** Multiply two matrices and store the result in a third.
	*/
	ga_mat3f operator*(const ga_mat3f& __restrict b) const;

	/*
	** Multiply a matrix by another, storing the result in the first.
	*/
	ga_mat3f& operator*=(const ga_mat3f& __restrict m);

	/*
	** Transform a vector by a matrix.
	*/
	ga_vec3f transform(const ga_vec3f& __restrict in) const;

	/*
	** Transpose a matrix.
	*/
	void transpose();

	/*
	** Invert the given matrix.
	*/
	void invert();

	/*
	** Determine if two matrices are largely equivalent.
	*/
	bool equal(const ga_mat3f& __restrict b);
};
