//////////////////////////////////////////////////////////////////////////////
/// \file GL.h
/// \brief OpenGL header collection.
/// 
/// Include this file to include all standard GL headers.

#pragma once

// GLEW
#include <GL/glew.h>

#ifdef WIN32
#ifndef GLBLAAT_NOTLEAN
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
