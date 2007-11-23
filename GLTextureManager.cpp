#include "GLTextureManager.h"

#include <iostream>
#include <cassert>

using namespace std;

// ----------------------------------------------------------------------------
class GLTextureManager::GLManagedTexture
{
public:
	GLManagedTexture(const std::string &name, GLTexture *texture)
	{
		this->name = name;
		this->texture = texture;
		this->slot = -1;
	}

	string name;
	GLTexture *texture;
	int slot;
};

// ----------------------------------------------------------------------------
GLTextureManager *GLTextureManager::New() 
{
	if (GLEW_VERSION_1_3) 
	{
		cout << "GLTextureManager: Using OpenGL 1.3 or higher" << endl;
		return new GLTextureManager();
	} 
	else if (GLEW_ARB_multitexture) 
	{
		// TODO: add ARB fallback
		cerr << "GLTextureManager: Falling back to ARB (not implemented!)" 
			<< endl;
		return 0;
	} 
	else 
	{
		cerr << "GLTextureManager: Multitexturing not supported!" << endl;
		return 0;
	}
}

// ----------------------------------------------------------------------------
GLTextureManager::GLTextureManager() 
{ 
	cout << "GLTextureManager: Constructor" << endl;
	// Initialize inuse vector
	for (unsigned int i = 0; i < GL_MAX_TEXTURE_UNITS; ++i) 
	{
		inuse.push_back(false);
		assignments.push_back(0);
	}
}

// ----------------------------------------------------------------------------
GLTextureManager::~GLTextureManager() 
{ 
	cout << "GLTextureManager: Destructor" << endl;
	// Delete all textures
	map<string, GLManagedTexture*>::iterator i = textures.begin();
	while (i != textures.end()) 
	{
		const string name = i->first;
		GLManagedTexture *mtex = i->second;
		++i;
		if (mtex->texture) 
		{
			DeleteTexture(name);
		} 
		else 
		{
			RemoveTexture(name);
		}
	}
}

// ----------------------------------------------------------------------------
bool GLTextureManager::AddTexture(const string &name, GLTexture *tex) 
{
	// Create a wrapper
	GLManagedTexture *mtex = new GLManagedTexture(name, tex);
	// Add the texture to the list
	textures[name] = mtex;

	return true;
}

// ----------------------------------------------------------------------------
bool GLTextureManager::AddReservedSlot(const string &name, unsigned int slot) 
{
	assert(0 <= slot && slot < GL_MAX_TEXTURE_UNITS);

	// Is this slot currently free?
	if (inuse[slot]) 
	{
		cerr << "GLTextureManager: Error adding reserved slot '" 
			<< name << "', slot already in use." << endl;
		return false;
	}
		
	// Create a wrapper and reserve the slot
	GLManagedTexture *mtex = new GLManagedTexture(name, 0);
	mtex->slot = slot;
	textures[name] = mtex;
	assignments[slot] = mtex;
	inuse[slot] = true;

	return true;
}

// ----------------------------------------------------------------------------
GLTexture *GLTextureManager::GetTexture(const string &name) 
{
	// Do we know this texture?
	if (textures.find(name) == textures.end()) 
	{
		cerr << "GLTextureManager: Error getting texture '" 
			<< name << "', texture not found" << endl;
		return 0;
	}
	// Reserved slots will just return null here
	return textures[name]->texture;
}

// ----------------------------------------------------------------------------
GLTexture *GLTextureManager::RemoveTexture(const string &name) 
{
	// Do we know this texture?
	if (textures.find(name) == textures.end()) 
	{
		//cerr << "GLTextureManager: Error removing texture '" 
		//	<< name << "', texture not found" << endl;
		return 0;
	}
	GLManagedTexture *mtex = textures[name];
	GLTexture *tex = mtex->texture;
	int slot = mtex->slot;
	// Don't unbind unassigned textures
	if (slot >= 0) 
	{
		// Don't unbind reserved slots
		if (tex)
		{
			// Unbind the texture
			glActiveTexture(GL_TEXTURE0 + slot);
			tex->UnbindCurrent();
		}
		// Free the slot
		assignments[slot] = 0;
		inuse[slot] = false;
	}
	// Now forget about the thing
	textures.erase(name);
	delete mtex;
	// Release texture to caller
	return tex;
}

// ----------------------------------------------------------------------------
void GLTextureManager::DeleteTexture(const string &name) 
{
	GLTexture *tex = RemoveTexture(name);
	if (tex) 
	{
		//cout << "GLTextureManager: Deleting texture '" 
		//	<< name << "'" << endl;
		delete tex;
	} 
}

// ----------------------------------------------------------------------------
// Reset all assignments, except reserved slots
void GLTextureManager::BeginNewPass()
{
	for (unsigned int slot = 0; slot < GL_MAX_TEXTURE_UNITS; ++slot) 
	{
		GLManagedTexture *mtex = assignments[slot];
		// Ignore unused slots
		if (inuse[slot])
		{
			assert(mtex);
			// Leave the reserved slots in place until they are explicitly removed
			if (mtex->texture)
			{
				inuse[slot] = false;
				assignments[slot] = 0;
				mtex->slot = -1;
			}
		}
	}
}

// ----------------------------------------------------------------------------
// Bind all textures which have been assigned a slot
void GLTextureManager::Bind() 
{
	for (unsigned int slot = 0; slot < GL_MAX_TEXTURE_UNITS; ++slot) 
	{
		// Ignore unused slots
		if (inuse[slot])
		{
			// Get info
			GLManagedTexture *mtex = assignments[slot];
			assert(mtex);
			GLTexture *tex = mtex->texture;
			// Don't touch the reserved texture units
			if (tex) 
			{
				// Set up texture in OpenGL
				glActiveTexture(GL_TEXTURE0 + slot);
				tex->BindToCurrent();
			}
		}
	}
	glActiveTexture(GL_TEXTURE0);
}

// ----------------------------------------------------------------------------
void GLTextureManager::Unbind() 
{
	for (unsigned int slot = 0; slot < GL_MAX_TEXTURE_UNITS; ++slot) 
	{
		// Ignore unused slots
		if (inuse[slot])
		{
			// Get info
			GLManagedTexture *mtex = assignments[slot];
			assert(mtex);
			GLTexture *tex = mtex->texture;
			// Don't touch the reserved texture units
			if (tex) 
			{
				// Unbind the texture
				glActiveTexture(GL_TEXTURE0 + slot);
				tex->UnbindCurrent();
			}
		}
	}
	glActiveTexture(GL_TEXTURE0);
}

// ----------------------------------------------------------------------------
bool GLTextureManager::SetupProgram(GLProgram &prog) 
{
	// Find the first free texture unit, if any
	int firstfree = 0;
	while (firstfree < GL_MAX_TEXTURE_UNITS)
	{
		if (!inuse[firstfree]) break;
		++firstfree;
	}
	if (firstfree == GL_MAX_TEXTURE_UNITS)
	{
		// No free texture units
		firstfree = -1;
	}

	// Iterate over all our textures
	for (map<string, GLManagedTexture*>::iterator i = textures.begin(); 
		i != textures.end(); ++i) 
	{
		// Get info
		string name = i->first;
		GLManagedTexture *mtex = i->second;
		
		// Does this texture already have a slot?
		int slot = mtex->slot;
		if (slot < 0)
		{
			slot = firstfree;
		}

		// Does the program need the texture?
		if (prog.UseTexture(name, (slot >= 0 ? slot : 0)))
		{
			// Texture was used by program
			if (slot < 0) {
				// Oops, the slot wasn't valid
				cerr << "GLTextureManager: Error setting up program, " 
					<< "no free slots for texture '" << name << "'" << endl;
				return false;
			}

			// Store slot assignment
			mtex->slot = slot;
			assignments[slot] = mtex;
			inuse[slot] = true;

			// Find next free slot, if any
			while (firstfree < GL_MAX_TEXTURE_UNITS && firstfree >= 0)
			{
				if (!inuse[firstfree]) break;
				++firstfree;
			}
			if (firstfree == GL_MAX_TEXTURE_UNITS)
			{
				// No free texture units
				firstfree = -1;
			}
		} 
	}

	return true;
}
