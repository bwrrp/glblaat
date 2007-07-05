#include "GLTextureManager.h"

#include <iostream>

using namespace std;

GLTextureManager *GLTextureManager::New() {
	if (GLEW_VERSION_1_3) {
		cout << "GLTextureManager: Using OpenGL 1.3 or higher" << endl;
		return new GLTextureManager();
	} else if (GLEW_ARB_multitexture) {
		// TODO: add ARB fallback
		cerr << "GLTextureManager: Falling back to ARB (not implemented!)" << endl;
		return 0;
	} else {
		cerr << "GLTextureManager: Multitexturing not supported!" << endl;
		return 0;
	}
}

GLTextureManager::GLTextureManager() { 
	cout << "GLTextureManager: Constructor" << endl;
	// Initialize inuse vector
	for (unsigned int i = 0; i < GL_MAX_TEXTURE_UNITS; ++i) {
		inuse.push_back(false);
	}
}

GLTextureManager::~GLTextureManager() { 
	cout << "GLTextureManager: Destructor" << endl;
	// Delete all textures
	map<string, GLTexture*>::iterator i = textures.begin();
	while (i != textures.end()) {
		const string name = i->first;
		GLTexture *tex = i->second;
		++i;
		if (tex) {
			DeleteTexture(name);
		} else {
			RemoveTexture(name);
		}
	}
}

bool GLTextureManager::AddTexture(const string &name, GLTexture *tex) {
	// Find the first free slot (linear search)
	unsigned int i = 0;
	while (i < GL_MAX_TEXTURE_UNITS) {
		if (!inuse[i]) break;
		++i;
	}
	// Any free texture units?
	if (i == GL_MAX_TEXTURE_UNITS) {
		cerr << "GLTextureManager: Error adding texture '" << name << "', no free texture units" << endl;
		return false;
	}
	// Ok, store info
	//cout << "GLTextureManager: Adding texture '" << name << "', unit " << i << endl;
	textures[name] = tex;
	texunits[name] = i;
	texnames[i] = name;
	inuse[i] = true;

	return true;
}

bool GLTextureManager::AddReservedSlot(const string &name, unsigned int slot) {
	// Is this slot currently free?
	if (inuse[slot]) {
		// Fetch the name for the texture currently in this slot
		string name2 = texnames[slot];
		if (!textures[name2]) {
			cerr << "GLTextureManager: Error adding reserved slot '" << name << 
				"', slot " << slot << " is already reserved as '" << name2 << "'" << endl;
			return false;
		}
		// Try to move the texture out of the way
		if (!AddTexture(name2, textures[name2])) {
			return false;
		}
	}
	// Reserve the slot
	textures[name] = 0;
	texunits[name] = slot;
	texnames[slot] = name;
	inuse[slot] = true;

	return true;
}

GLTexture *GLTextureManager::GetTexture(const string &name) {
	// Do we know this texture?
	if (textures.find(name) == textures.end()) {
		cerr << "GLTextureManager: Error getting texture '" << name << "', texture not found" << endl;
		return 0;
	}
	// Reserved slots will just return null here
	return textures[name];
}

GLTexture *GLTextureManager::RemoveTexture(const string &name) {
	// Do we know this texture?
	if (textures.find(name) == textures.end()) {
		cerr << "GLTextureManager: Error removing texture '" << name << "', texture not found" << endl;
		return 0;
	}
	int unit = texunits[name];
	GLTexture *tex = textures[name];
	// Don't unbind reserved slots
	if (tex) {
		//cout << "GLTextureManager: Removing texture '" << name << "', unit " << unit << endl;
		// Unbind the texture
		glActiveTexture(GL_TEXTURE0 + unit);
		tex->UnbindCurrent();
	}
	// Now forget about the thing
	inuse[unit] = false;
	texnames.erase(unit);
	texunits.erase(name);
	textures.erase(name);
	// Release texture to caller
	return tex;
}

void GLTextureManager::DeleteTexture(const string &name) {
	GLTexture *tex = RemoveTexture(name);
	if (tex) {
		cout << "GLTextureManager: Deleting texture '" << name << "'" << endl;
		delete tex;
	} else {
		cerr << "GLTextureManager: Will not delete reserved slot '" << name << "'" << endl;
	}
}

void GLTextureManager::Bind() {
	for (map<string, GLTexture*>::iterator i = textures.begin(); i != textures.end(); ++i) {
		// Get info
		const string &name = i->first;
		GLTexture *tex = i->second;
		int unit = texunits[name];
		// Don't touch the reserved texture units
		if (tex) {
			//cout << "GLTextureManager: Binding texture '" << name << "' to unit " << unit << endl;
			// Set up texture in OpenGL
			glActiveTexture(GL_TEXTURE0 + unit);
			tex->BindToCurrent();
		}
	}
	glActiveTexture(GL_TEXTURE0);
}

void GLTextureManager::Unbind() {
	for (map<string, GLTexture*>::iterator i = textures.begin(); i != textures.end(); ++i) {
		// Get info
		const string &name = i->first;
		GLTexture *tex = i->second;
		int unit = texunits[name];
		// Don't touch the reserved texture units
		if (tex) {
			//cout << "GLTextureManager: Unbinding unit " << unit << "(was texture '" << name << "')" << endl;
			// Unbind the texture
			glActiveTexture(GL_TEXTURE0 + unit);
			tex->UnbindCurrent();
		}
	}
	glActiveTexture(GL_TEXTURE0);
}

void GLTextureManager::SetupProgram(GLProgram &prog) {
	for (map<string, int>::iterator i = texunits.begin(); i != texunits.end(); ++i) {
		bool res = prog.UseTexture(i->first, i->second);
		if (res) {
			//cout << "GLTextureManager: Found texture variable '" << i->first << "' in program" << endl;
		} else {
			//cerr << "GLTextureManager: Texture variable '" << i->first << "' not found in program" << endl;
		}
	}
}
