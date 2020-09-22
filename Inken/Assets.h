#pragma once

#include "Mesh.h"
#include "AssetEnums.h"

class Assets {
public:

	static void LoadAll();
	/* Copy cached mesh */
	static void mesh(MODELS, Mesh&);
	/* Get cached texture */
	static Texture texture(TEXTURES);

private:

	static void LoadMeshes();
	static Mesh meshes[MAX_MODELS];
	static const std::string MODEL_FOLDER;
	static const std::string MODEL_PATHS[MAX_MODELS];
	static const std::string MODEL_EXT;
	static bool MODEL_IS_CULL[MAX_MODELS];

	static void LoadTextures();
	static Texture textures[MAX_TEXTURES];
	static const std::string TEXTURE_FOLDER;
	static const std::string TEXTURE_PATHS[MAX_TEXTURES];

};