#pragma once
#include "GLQuery.h"

// Some support for older GLEW versions
#ifndef GLEW_EXT_timer_query
#if defined(_MSC_VER) && _MSC_VER < 1400
typedef __int64 GLint64EXT;
typedef unsigned __int64 GLuint64EXT;
#else
typedef signed long long GLint64EXT;
typedef unsigned long long GLuint64EXT;
#endif
#endif // GLEW_EXT_timer_query

class GLTimerQuery : public GLQuery
{
public:
	static GLTimerQuery *New();
	virtual ~GLTimerQuery();

	GLint64EXT GetResulti64();
	GLuint64EXT GetResultui64();

protected:
	GLTimerQuery();
	
private:
	// Not implemented
	GLTimerQuery(const GLTimerQuery&);
	void operator=(const GLTimerQuery&);
};
