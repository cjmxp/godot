
#ifndef MATLASTEXTURE_H
#define MATLASTEXTURE_H
#include "scene/resources/texture.h"

class MAtlasTexture : public AtlasTexture {
	GDCLASS(MAtlasTexture, AtlasTexture);
public:
	MAtlasTexture();
	~MAtlasTexture();
	uint8_t i;
	uint32_t id;
	String key;
};
#endif
