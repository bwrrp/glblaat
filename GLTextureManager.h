#pragma once
#include "GL.h"
#include "GLTexture.h"
#include "GLProgram.h"
#include <map>
#include <string>
#include <vector>

class GLTextureManager 
{
public:
	static GLTextureManager *New();
	~GLTextureManager();

	bool AddTexture(const std::string &name, GLTexture *tex);
	bool AddReservedSlot(const std::string &name, unsigned int slot);
	GLTexture *GetTexture(const std::string &name);
	const GLTexture *GetTexture(const std::string &name) const;
	GLTexture *RemoveTexture(const std::string &name);
	void DeleteTexture(const std::string &name);

	void BeginNewPass();
	void Bind();
	void Unbind();

	bool SetupProgram(GLProgram &prog);

protected:
	class GLManagedTexture;

	std::map<std::string, GLManagedTexture*> textures;
	std::vector<GLManagedTexture*> assignments;
	std::vector<bool> inuse;

	GLTextureManager();

private:
	// Not implemented
	GLTextureManager(const GLTextureManager&);
	void operator=(const GLTextureManager&);
};
