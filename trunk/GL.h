//////////////////////////////////////////////////////////////////////////////
/// \file GL.h
/// \brief OpenGL header collection.
/// 
/// Include this file to include all standard GL headers.

#pragma once

// GLEW
#include <GL/glew.h>

#ifdef WIN32
// Windows headers, needed for GL
#include <windows.h>
#endif

// OpenGL headers
#include <GL/gl.h>
#include <GL/glu.h>
