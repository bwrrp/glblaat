#include "GLOcclusionQuery.h"

#ifndef NDEBUG
#include <iostream>
#endif

// ----------------------------------------------------------------------------
GLOcclusionQuery *GLOcclusionQuery::New() 
{
	if (GLEW_VERSION_1_5) 
	{
#ifndef NDEBUG
		std::cout << "GLOcclusionQuery: Using OpenGL 1.5" << std::endl;
#endif
		return new GLOcclusionQuery();
	} 
	else if (GLEW_ARB_occlusion_query)
	{
		// TODO: add an ARB version of these classes as fallback
#ifndef NDEBUG
		std::cerr << "GLOcclusionQuery: Falling back to ARB (not implemented!)" 
			<< std::endl;
#endif
		return 0;
	}
	else
	{
#ifndef NDEBUG
		std::cerr << "GLOcclusionQuery: Occlusion queries not supported!" 
			<< std::endl;
#endif
		return 0;
	}
}

// ----------------------------------------------------------------------------
GLOcclusionQuery::GLOcclusionQuery() 
: GLQuery(GL_SAMPLES_PASSED)
{
}

// ----------------------------------------------------------------------------
GLOcclusionQuery::~GLOcclusionQuery() 
{
}
