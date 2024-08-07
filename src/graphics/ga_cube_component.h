#pragma once

/*
** RPI Game Architecture Engine
**
** Portions adapted from:
** Viper Engine - Copyright (C) 2016 Velan Studios - All Rights Reserved
**
** This file is distributed under the MIT License. See LICENSE.txt.
*/

#include <string>
#include <cstdint>
#include <memory>
#include "ga_mat4f.h"
#include "ga_material.h"

/*
** Renderable basic textured cubed.
*/
class ga_cube_component
{
public:
	ga_cube_component(std::string& source_vs, std::string& source_fs, std::unique_ptr<ga_material>&& bp);
	virtual ~ga_cube_component();

	std::unique_ptr<ga_material> _material;
	ga_mat4f _transform;
	uint32_t _vao;
	uint32_t _vbos[5];
	uint32_t _index_count;
};
