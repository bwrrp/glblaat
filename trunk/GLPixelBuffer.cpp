#include "GLPixelBuffer.h"

#include "GLUtility.h"
#include <iostream>

using namespace std;

// ----------------------------------------------------------------------------
GLPixelBuffer *GLPixelBuffer::New() 
{
	// TODO: extension promoted to core in OpenGL 2.1
	if (GLEW_ARB_pixel_buffer_object) 
	{
		cout << "GLPixelBuffer: Using ARB pixel buffers" << endl;
		return new GLPixelBuffer();
	} 
	else 
	{
		cerr << "GLPixelBuffer: Pixel buffer objects not supported!" << endl;
		return 0;
	}
}

// ----------------------------------------------------------------------------
GLPixelBuffer::GLPixelBuffer() 
: GLBuffer()
{
	//cout << "GLPixelBuffer: Constructor" << endl;
}

// ----------------------------------------------------------------------------
GLPixelBuffer::~GLPixelBuffer() 
{
	//cout << "GLPixelBuffer: Destructor" << endl;
}
