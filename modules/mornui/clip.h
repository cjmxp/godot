
#ifndef CLIP_H
#define CLIP_H
#include "modules/pugixml/pugixml_exporter.h"
#include "box.h"
#include <stdio.h>

class Clip : public Box {

	GDCLASS(Clip, Box);
	OBJ_CATEGORY("GUI Nodes");
public:
	Clip();
	~Clip();
	const String& GetSkin()const { return skin_; };
	void SetSkin(const String& v);
	float GetInterval() { return interval_; };
	void SetInterval(float v);
	const String& GetSizeGrid() { return grid_; };
	void SetSizeGrid(const String& rect);
	int GetIndex() { return index_; };
	void SetIndex(int);
	int GetClipX() { return clipX_; };
	void SetClipX(int);
	int GetClipY() { return clipY_; };
	void SetClipY(int);
	void InitAttribute(Ref<XMLNode> node,ScriptInstance* self) override;
protected:
	enum AxisStretchMode {
		AXIS_STRETCH_MODE_STRETCH,
		AXIS_STRETCH_MODE_TILE,
		AXIS_STRETCH_MODE_TILE_FIT,
	};
	void _notification(int p_what);
	String skin_;
	String grid_;
	Ref<Texture> texture;
	float interval_{ 0.0f };
	int clipX_{ 1 };
	int clipY_{ 1 };
	int index_{ 0 };
	int margin[4];
	bool draw_center;
	AxisStretchMode axis_h, axis_v;
	//static void _bind_methods();
};
#endif
