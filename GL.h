// Copyright (c) 2009 Stef Busking
// Distributed under the terms of the MIT License.

// Include this file to include all standard GL headers.

#pragma once

// GLEW
#include <GL/glew.h>

#ifdef WIN32
#ifndef GLBLAAT_NOTLEAN
// Prevent macro redefinition errors
#undef WIN32_LEAN_AND_MEAN
#undef VC_EXTRALEAN
#undef NOMINMAX
// No need to include too much
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#define NOMINMAX
#endif // GLBLAAT_NOTLEAN
// Windows headers, needed for GL
#include <windows.h>
#endif // WIN32

// OpenGL headers
#include <GL/gl.h>
#include <GL/glu.h>
