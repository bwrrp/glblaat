once
#include "GLResource.h"
#include "GL.h"

class GLRendertarget : public GLResource 
{
public:
	friend class GLFramebuffer;

	virtual ~GLRendertarget();

	virtual bool Resize(int width, int height) = 0;

	int GetTimesAttached() { return timesAttached; }

protected:
	int width;
	int height;
	int timesAttached;

	GLRendertarget(int width, int height);

	virtual void AttachToBoundFBO(int attachment);
	virtual void DetachFromBoundFBO(int attachment);

private:
	// Not implemented
	GLRendertarget(const GLRendertarget&);
	void operator=(const GLRendertarget&);
};
