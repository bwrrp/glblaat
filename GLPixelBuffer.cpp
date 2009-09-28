#include "GLPixelBuffer.h"

#include "GLUtility.h"

#ifndef NDEBUG
#include <iostream>
#endif

using namespace std;

// ----------------------------------------------------------------------------
GLPixelBuffer *GLPixelBuffer::New() 
{
	// TODO: extension promoted to core in OpenGL 2.1
	if (GLEW_ARB_pixel_buffer_object) 
	{
#ifndef NDEBUG
		cout << "GLPixelBuffer: Using ARB pixel buffers" << endl;
#endif
		return new GLPixelBuffer();
	} 
	else 
	{
#ifndef NDEBUG
		cerr << "GLPixelBuffer: Pixel buffer objects not supported!" << endl;
#endif
		return 0;
	}
}

// ----------------------------------------------------------------------------
GLPixelBuffer::GLPixelBuffer() 
: GLBuffer()
{
}

// ----------------------------------------------------------------------------
GLPixelBuffer::~GLPixelBuffer() 
{
}
