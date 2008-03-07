#pragma once
#include "GLQuery.h"

class GLOcclusionQuery : public GLQuery
{
public:
	static GLOcclusionQuery *New();
	virtual ~GLOcclusionQuery();

protected:
	GLOcclusionQuery();
	
private:
	// Not implemented
	GLOcclusionQuery(const GLOcclusionQuery&);
	void operator=(const GLOcclusionQuery&);
};
