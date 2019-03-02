
#ifndef MUI_H
#define MUI_H
#include "mres.h"
#include "scene/resources/texture.h"

class MUI : public MRes {
	GDCLASS(MUI, MRes);
public:
	MUI();
	~MUI();
	virtual bool Load() override;
	virtual bool Fill(PoolByteArray db) override;
	Vector<Ref<Image>> imgs_;
	Map<uint32_t, ImageTexture> map_;
};
#endif
