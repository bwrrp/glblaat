// Copyright (c) 2009 Stef Busking
// Distributed under the terms of the MIT License.

#pragma once

// Abstract superclass for all GLBlaat resources.
class GLResource 
{
public:
	virtual ~GLResource() {}
protected:
	GLResource() {} // can not instantiate abstract class
};
