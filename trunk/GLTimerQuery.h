#pragma once
#include "GLQuery.h"

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
