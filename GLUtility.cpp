#include "GLUtility.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>
#include "GL.h"
#include <vtkMath.h>

using namespace std;

GLUtility::GLUtility() { }

GLUtility::~GLUtility() { }

bool GLUtility::ErrorFlag = false;

void GLUtility::CheckOpenGLError(const std::string &task) {
	GLenum err = glGetError();
	// Make sure we get all error flags before we continue, but don't loop more than 100 times
	int i = 0;
	while (err != GL_NO_ERROR && i < 10) {
		ErrorFlag = true;
		const char *errstring = reinterpret_cast<const char*>(gluErrorString(err));
		cerr << task << " FAILED (err[" << (i+1) << "]=" << err << 
			", \"" << (errstring ? errstring : "???") << "\")" << endl;
		//assert(false);
		err = glGetError();
		++i;
		if (i == 10) {
			// This should not happen, but apparently it does
			// glGetError should return 0 if it generates an error itself
			cerr << "GLUtility: 10 consecutive errors reached, are you misssing a glEnd()?";
		}
	}
}

void GLUtility::ClearOpenGLError() {
	// Loop until there are no more errors
	int i = 0;
	while (glGetError() != GL_NO_ERROR && i < 10) { 
		++i;
		if (i == 10) {
			// This should not happen, but apparently it does
			// glGetError should return 0 if it generates an error itself
			cerr << "GLUtility: 10 consecutive errors reached, are you missing a glEnd()?";
		}
	}
	ErrorFlag = false;
}

bool GLUtility::GetErrorFlag() {
	return ErrorFlag;
}
