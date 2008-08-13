#include "GLTextureManager.h"

#include <iostream>
#include <cassert>

using namespace std;

// ----------------------------------------------------------------------------
GLTextureManager *GLTextureManager::New() 
{
	if (GLEW_VERSION_1_3) 
	{
		std::cout 
			<< "GLTextureManager: Using OpenGL 1.3 or higher" 
			<< std::endl;
		return new GLTextureManager();
	} 
	else if (GLEW_ARB_multitexture) 
	{
		// TODO: add ARB fallback
		std::cerr 
			<< "GLTextureManager: Falling back to ARB (not implemented!)" 
			<< std::endl;
		return 0;
	} 
	else 
	{
		std::cerr 
			<< "GLTextureManager: Multitexturing not supported!" 
			<< std::endl;
		return 0;
	}
}

// ----------------------------------------------------------------------------
GLTextureManager::GLTextureManager()
{ 
	std::cout << "GLTextureManager: Constructor" << std::endl;

	// Get maximum number of texture units
	// NOTE: this seems to be the right parameter, but I'm not 100% sure... 
	// GL_MAX_TEXTURE_UNITS is stuck at 4 on nvidia, however 
	// GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS might be used as well.
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);

	// Initialize current bindings
	currentBinding.resize(maxTextureUnits, 0);
}

// ----------------------------------------------------------------------------
GLTextureManager::~GLTextureManager() 
{
	std::cout << "GLTextureManager: Destructor" << std::endl;

	// Make sure our textures are no longer bound
	Unbind();

	// Delete all managed textures
	std::map<std::string, GLTexture*>::iterator it = textures.begin();
	while (it != textures.end()) 
	{
		//const string name = it->first;
		GLTexture *tex = it->second;
		assert(tex);
		++it;
		delete tex;
	}
}

// ----------------------------------------------------------------------------
// Add a new sampler with the given name and initial texture, optionally add to store
GLTextureManager::SamplerId GLTextureManager::AddTexture(const string &name, 
														 GLTexture *tex, 
														 bool takeOwnership) 
{
	// Do we already have a sampler with this name?
	SamplerId sampler = GetSampler(name);
	if (sampler == BAD_SAMPLER_ID)
	{
		// Find an unused SamplerId to register a new sampler
		if (!unusedSamplers.empty())
		{
			sampler = unusedSamplers.front();
			unusedSamplers.pop();
		}
		else
		{
			// No unused SamplerIds, add a new one
			sampler = samplers.size();
			samplers.push_back(tex);
		}

		samplersByName[name] = sampler;
	}

	// Assign this texture to the sampler
	// TODO: what to do if this sampler already has a texture?
	// (see also SwapTexture)
	samplers[sampler] = tex;
	
	if (takeOwnership)
	{
		// Add the texture to the list
		textures[name] = tex;
	}

	return sampler;
}

// ----------------------------------------------------------------------------
// Get the sampler with the given name
GLTextureManager::SamplerId GLTextureManager::GetSampler(const std::string &name)
{
	map<std::string, SamplerId>::iterator sit = samplersByName.find(name);
	if (sit != samplersByName.end())
	{
		return sit->second;
	}
	else
	{
		return BAD_SAMPLER_ID;
	}
}

// ----------------------------------------------------------------------------
// Swap the texture assigned to the given sampler, returns the old texture
GLTexture *GLTextureManager::SwapTexture(SamplerId sampler, GLTexture *tex)
{
	assert(sampler >= 0 
		&& static_cast<unsigned int>(sampler) < samplers.size());

	// Replace the texture on this sampler
	GLTexture *oldTex = samplers[sampler];
	samplers[sampler] = tex;

	// TODO: should we also replace any texture in the store with the same name?

	return oldTex;
}

// ----------------------------------------------------------------------------
// Unregister the given name as a sampler
void GLTextureManager::RemoveSampler(const std::string &name)
{
	// Find the sampler
	SamplerId sampler = GetSampler(name);
	if (sampler != BAD_SAMPLER_ID) 
	{
		// TODO: invalidate any programs in cache that use this sampler?

		unusedSamplers.push(sampler);
		samplers[sampler] = 0;
		samplersByName.erase(name);
	}
}

// ----------------------------------------------------------------------------
// Specify that the given unit is managed elsewhere, but can be used by programs as name
bool GLTextureManager::AddReservedSlot(const string &name, int unit) 
{
	assert(0 <= unit && unit < maxTextureUnits);

	// Register reserved slot
	reserved[name] = unit;

	// Is any texture bound here?
	if (currentBinding[unit]) 
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		currentBinding[unit]->UnbindCurrent();
		currentBinding[unit] = 0;
		glActiveTexture(GL_TEXTURE0);
	}

	// TODO: some more error checking could be useful (aliasing...)

	return true;
}

// ----------------------------------------------------------------------------
// Get a texture from the store
GLTexture *GLTextureManager::GetTexture(const string &name) 
{
	// Do we know this texture?
	map<std::string, GLTexture*>::iterator texit = 
		textures.find(name);

	if (texit == textures.end()) 
	{
#ifndef NDEBUG
		std::cerr 
			<< "GLTextureManager: Error getting texture '" 
			<< name 
			<< "', texture not found" 
			<< std::endl;
#endif
		return 0;
	}
	return texit->second;
}

// ----------------------------------------------------------------------------
// Get a texture from the store
const GLTexture *GLTextureManager::GetTexture(const string &name) const
{
	// Do we know this texture?
	map<std::string, GLTexture*>::const_iterator texit = 
		textures.find(name);

	if (texit == textures.end()) 
	{
#ifndef NDEBUG
		std::cerr 
			<< "GLTextureManager: Error getting texture '" 
			<< name 
			<< "', texture not found" 
			<< std::endl;
#endif
		return 0;
	}
	return texit->second;
}

// ----------------------------------------------------------------------------
// Remove a texture from the store (transfer ownership to caller), or delete a reserved slot
GLTexture *GLTextureManager::RemoveTexture(const string &name) 
{
	// Is this a reserved slot?
	std::map<std::string, int>::iterator rit = reserved.find(name);
	if (rit != reserved.end())
	{
		// Remove the reserved slot
		reserved.erase(name);
		return 0;
	}
	else
	{
		GLTexture *tex = GetTexture(name);
		// Ignore invalid removals
		if (!tex) return 0;

		// If this texture is currently bound, unbind it
		for (int unit = 0; unit < maxTextureUnits; ++unit)
		{
			if (currentBinding[unit] == tex)
			{
				glActiveTexture(GL_TEXTURE0 + unit);
				tex->UnbindCurrent();
				currentBinding[unit] = 0;
			}
		}
		glActiveTexture(GL_TEXTURE0);

		// Remove the texture from the store
		textures.erase(name);
		return tex;
	}
}

// ----------------------------------------------------------------------------
// Delete a texture from the store, or delete a reserved slot
void GLTextureManager::DeleteTexture(const string &name) 
{
	GLTexture *tex = RemoveTexture(name);
	if (tex) 
	{
#ifndef NDEBUG
		/*
		std::cout 
			<< "GLTextureManager: Deleting texture '" 
			<< name << "'" << std::endl;
		*/
#endif
		delete tex;
	} 
}

// ----------------------------------------------------------------------------
// Reset all assignments, except reserved slots
void GLTextureManager::BeginNewPass()
{
	// Essentially, this means we simply unregister all programs
	std::map<GLProgram*, SamplerBindings>::iterator it = bindings.begin();
	while (it != bindings.end())
	{
		GLProgram *prog = it->first;
		++it;
		UnregisterProgram(prog);
	}
}

// ----------------------------------------------------------------------------
// Bind all textures required by the currently active program
void GLTextureManager::Bind() 
{
	// Make sure we have a current program
	assert(currentProgram);
	SamplerBindings &binding = bindings[currentProgram];

	for (int unit = 0; unit < maxTextureUnits; ++unit) 
	{
		SamplerId samplerId = binding[unit];
		// Ignore unused / reserved units
		if (samplerId >= 0)
		{
			// Get the texture
			GLTexture *tex = samplers[samplerId];
			// This could be NULL due to programmer error 
			// (removing a sampler without resetting programs that use it)
			//assert(tex);

			// Check if anything is bound to this unit
			GLTexture *oldTex = currentBinding[unit];
			if (oldTex == tex) continue;

			// Set up texture in OpenGL
			glActiveTexture(GL_TEXTURE0 + unit);
			if (oldTex) oldTex->UnbindCurrent();
			tex->BindToCurrent();

			// Update current binding
			currentBinding[unit] = tex;
		}
	}
	glActiveTexture(GL_TEXTURE0);
}

// ----------------------------------------------------------------------------
// Unbind all currently bound textures
void GLTextureManager::Unbind() 
{
	for (int unit = 0; unit < maxTextureUnits; ++unit) 
	{
		GLTexture *tex = currentBinding[unit];
		if (tex)
		{
			glActiveTexture(GL_TEXTURE0 + unit);
			tex->UnbindCurrent();
			currentBinding[unit] = 0;
		}
	}
	glActiveTexture(GL_TEXTURE0);
}

// ----------------------------------------------------------------------------
bool GLTextureManager::SetupProgram(GLProgram *prog, bool updateIfKnown) 
{
	// Set current program
	currentProgram = prog;

	// Do we know this program?
	map<GLProgram*, SamplerBindings>::iterator it = 
		bindings.find(prog);
	if (it == bindings.end() || updateIfKnown)
	{
		// Add new (or get and clear) bindings for this program
		SamplerBindings &binding = bindings[prog];
		binding.clear();
		binding.resize(maxTextureUnits, -1);

		// For finding available texture units
		unsigned int nextFreeUnit = 0;
		std::vector<bool> inUse;
		inUse.resize(maxTextureUnits, false);
		// Mark all reserved slots
		for (std::map<std::string, int>::const_iterator rit = 
			reserved.begin(); rit != reserved.end(); ++rit)
		{
			inUse[rit->second] = true;
		}

		// Get the uniforms required for this program
		std::vector<GLUniformInfo> uniforms = prog->GetActiveUniforms();
		for (std::vector<GLUniformInfo>::iterator it = uniforms.begin();
			it != uniforms.end(); ++it)
		{
			// Is this a sampler?
			if (it->type == GL_SAMPLER_1D 
				|| it->type == GL_SAMPLER_2D 
				|| it->type == GL_SAMPLER_3D 
				|| it->type == GL_SAMPLER_CUBE 
				|| it->type == GL_SAMPLER_1D_SHADOW 
				|| it->type == GL_SAMPLER_2D_SHADOW
				|| it->type == GL_SAMPLER_2D_RECT_ARB
				|| it->type == GL_SAMPLER_2D_RECT_SHADOW_ARB)
			{
				// Is this a reserved texture unit?
				std::map<std::string, int>::const_iterator rit = 
					reserved.find(it->name);
				if (rit != reserved.end())
				{
					// Reserved unit, only set location in program
					prog->UseTexture(it->name, rit->second);
				}
				else
				{
					// Find the matching SamplerId
					SamplerId sampler = GetSampler(it->name);
					if (sampler == BAD_SAMPLER_ID)
					{
						std::cerr 
							<< "GLTextureManager: Program requires unknown sampler '" 
							<< it->name << "'" << std::endl;
						return false;
					}

					// Find the next free texture unit
					while (nextFreeUnit != maxTextureUnits 
						&& inUse[nextFreeUnit]) 
					{
						++nextFreeUnit;
					}
					if (nextFreeUnit == maxTextureUnits)
					{
						// We ran out of texture units!
						std::cerr 
							<< "GLProgram: ran out of available texture units!" 
							<< std::endl;
						return false;
					}

					// Assign sampler to unit
					binding[nextFreeUnit] = sampler;
					inUse[nextFreeUnit] = true;
					// Pass the unit id to program
					prog->UseTexture(it->name, nextFreeUnit);
				}
			}
		}
	}

	return true;
}

// ----------------------------------------------------------------------------
// Remove bindings for the given program
void GLTextureManager::UnregisterProgram(GLProgram *prog)
{
	bindings.erase(prog);
}
