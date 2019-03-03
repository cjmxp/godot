
#ifndef MUI_H
#define MUI_H
#include "mres.h"
#include "matlastexture.h"

class MUI : public MRes {
	GDCLASS(MUI, MRes);
public:
	MUI();
	~MUI();
	Ref<Texture> GetSkin(const String& v);
	virtual bool Load() override;
	virtual bool Fill(PoolByteArray db) override;
	virtual void Init() override;
	Vector<Ref<Image>> imgs_;
	Map<uint32_t, Ref<MAtlasTexture>> map_;

};
#endif
