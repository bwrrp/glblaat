// Copyright (c) 2009 Stef Busking
// Distributed under the terms of the MIT License.

#pragma once

#include <string>

struct GLAttributeInfo
{
	int size;
	unsigned int type;
	std::string name;
	int index;
};

struct GLUniformInfo
{
 	int size;
 	unsigned int type; 
	std::string name;
};
