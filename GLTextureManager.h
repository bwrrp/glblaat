#pragma once
#include "GL.h"
#include "GLTexture.h"
#include "GLProgram.h"
#include <map>
#include <string>
#include <vector>
#include <queue>

class GLTextureManager 
{
public:
	static GLTextureManager *New();
	~GLTextureManager();

	typedef int SamplerId;
	static const SamplerId BAD_SAMPLER_ID = -1;

	// Add a new sampler with the given name and initial texture, optionally add to store
	SamplerId AddTexture(const std::string &name, GLTexture *tex, bool takeOwnership = true);
	// Get the sampler with the given name
	SamplerId GetSampler(const std::string &name);
	// Swap the texture assigned to the given sampler, returns the old texture
	GLTexture *SwapTexture(SamplerId sampler, GLTexture *tex);
	// Unregister the given name as a sampler
	void RemoveSampler(const std::string &name);

	// Specify that the given unit is managed elsewhere, but can be used by programs as name
	bool AddReservedSlot(const std::string &name, int unit);

	// Get a texture from the store
	GLTexture *GetTexture(const std::string &name);
	const GLTexture *GetTexture(const std::string &name) const;
	// Remove a texture from the store (transfer ownership to caller), or delete a reserved slot
	GLTexture *RemoveTexture(const std::string &name);
	// Delete a texture from the store, or delete a reserved slot
	void DeleteTexture(const std::string &name);

	// Set the specified program as current, analyze required textures if not cached
	bool SetupProgram(GLProgram *prog, bool updateIfKnown = false);
	// Remove the specified program from the cache
	void UnregisterProgram(GLProgram *prog);
	// Unregister all programs
	void BeginNewPass();

	// Bind textures required for the current program
	void Bind();
	// Unbind all currently bound textures
	void Unbind();

protected:
	// Maximum number of texture units allowed by the system
	int maxTextureUnits;

	// Slot to SamplerId mapping for each program
	typedef std::vector<SamplerId> SamplerBindings;
	std::map<GLProgram*, SamplerBindings> bindings;
	GLProgram *currentProgram;

	// SamplerId to texture / name mapping
	std::vector<GLTexture*> samplers;
	
	// Name to SamplerId mapping
	std::map<std::string, SamplerId> samplersByName;

	// SamplerIds for deleted samplers, to be re-used
	std::queue<SamplerId> unusedSamplers;

	// Texture store
	std::map<std::string, GLTexture*> textures;

	// Reserved texture units (these do not exist in the texture store)
	std::map<std::string, int> reserved; // name to unit

	// Current bindings
	std::vector<GLTexture*> currentBinding;

	// Old storage vars:
	//std::map<std::string, GLManagedTexture*> textures;
	//std::vector<GLManagedTexture*> assignments;
	//std::vector<bool> inuse;

	GLTextureManager();

private:
	// Not implemented
	GLTextureManager(const GLTextureManager&);
	void operator=(const GLTextureManager&);
};
