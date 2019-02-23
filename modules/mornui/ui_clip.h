
#ifndef UI_CLIP_H
#define UI_CLIP_H
#include "modules/pugixml/pugixml_exporter.h"
#include "ui_box.h"

class UI_Clip : public UI_Box {

	GDCLASS(UI_Clip, UI_Box);
	OBJ_CATEGORY("GUI Nodes");
public:
	UI_Clip();
	~UI_Clip();
	const String& GetSkin()const { return skin_; };
	virtual void SetSkin(const String& v);
	virtual Size2 get_minimum_size() const;
	int GetInterval() { return interval_; };
	void SetInterval(int v);
	const String& GetSizeGrid() { return grid_; };
	void SetSizeGrid(const String& rect);
	int GetIndex() { return index_; };
	void SetIndex(int);
	int GetClipX() { return clipX_; };
	void SetClipX(int);
	int GetClipY() { return clipY_; };
	void SetClipY(int);
	bool GetAutoPlay() { return autoplay_; };
	void SetAutoPlay(bool);
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
	Ref<Texture> texture_;
	int interval_{ 128 };
	int clipX_{ 1 };
	int clipY_{ 1 };
	int index_{ 0 };
	int margin_[4];
	bool autoplay_{false};
	uint32_t ctimer_{0};
	AxisStretchMode axis_h_{ AXIS_STRETCH_MODE_STRETCH };
	AxisStretchMode axis_v_{ AXIS_STRETCH_MODE_STRETCH };
	bool clip_draw_{ true };
	Size2 minsize_;
	//static void _bind_methods();
};
#endif
