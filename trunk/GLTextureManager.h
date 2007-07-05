#pragma once
#include "GL.h"
#include "GLTexture.h"
#include "GLProgram.h"
#include <map>
#include <string>
#include <vector>

class GLTextureManager {
public:
	static GLTextureManager *New();
	~GLTextureManager();

	bool AddTexture(const std::string &name, GLTexture *tex);
	bool AddReservedSlot(const std::string &name, unsigned int slot);
	GLTexture *GetTexture(const std::string &name);
	GLTexture *RemoveTexture(const std::string &name);
	void DeleteTexture(const std::string &name);

	void Bind();
	void Unbind();

	void SetupProgram(GLProgram &prog);

private:
	std::map<std::string, GLTexture*> textures;
	std::map<std::string, int> texunits;
	std::map<int, std::string> texnames; // inverse map of texunits
	std::vector<bool> inuse;

	GLTextureManager();
};
