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
