#include "GLOcclusionQuery.h"

#include <iostream>

using namespace std;

// ----------------------------------------------------------------------------
GLOcclusionQuery *GLOcclusionQuery::New() 
{
	if (GLEW_VERSION_1_5) 
	{
		cout << "GLOcclusionQuery: Using OpenGL 1.5" << endl;
		return new GLOcclusionQuery();
	} 
	else if (GLEW_ARB_occlusion_query)
	{
		// TODO: add an ARB version of these classes as fallback
		cerr << "GLOcclusionQuery: Falling back to ARB (not implemented!)" << endl;
		return 0;
	}
	else
	{
		cerr << "GLOcclusionQuery: Occlusion queries not supported!" << endl;
		return 0;
	}
}

// ----------------------------------------------------------------------------
GLOcclusionQuery::GLOcclusionQuery() 
: GLQuery(GL_SAMPLES_PASSED)
{
	//cout << "GLOcclusionQuery: Constructor" << endl;
}

// ----------------------------------------------------------------------------
GLOcclusionQuery::~GLOcclusionQuery() 
{
	//cout << "GLOcclusionQuery: Destructor" << endl;
}
