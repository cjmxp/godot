
#ifndef MATLASTEXTURE_H
#define MATLASTEXTURE_H
#include "scene/resources/texture.h"

class MAtlasTexture : public AtlasTexture {
	GDCLASS(MAtlasTexture, AtlasTexture);
public:
	MAtlasTexture();
	~MAtlasTexture();
	uint8_t i{ 0 };
	uint32_t id{ 0 };
	String key{ "" };
};
#endif
