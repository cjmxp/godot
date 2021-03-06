
#ifndef UI_SCROLLBAR_H
#define UI_SCROLLBAR_H
#include "ui_box.h"
#include "ui_button.h"
#include "ui_slider.h"
class UI_ScrollBar : public UI_Box {

	GDCLASS(UI_ScrollBar, UI_Box);
	OBJ_CATEGORY("GUI Nodes");
public:
	UI_ScrollBar();
	~UI_ScrollBar();
	void InitAttribute(Ref<XMLNode> node,ScriptInstance* self) override;
	
	const String& GetSizeGrid() { return grid_; };
	void SetSizeGrid(const String& rect);
	const String& GetSkin()const { return skin_; };
	void SetSkin(const String& v);
	int GetClipX() { return clipX_; };
	void SetClipX(int);
	int GetClipY() { return clipY_; };
	void SetClipY(int);
	String GetDirection();
	void SetDirection(const String& v);
	float GetValue() { return value_; };
	void SetValue(float v);
	void OnEvent(Ref<InputEvent> e) override;
protected:
	static void _bind_methods();
	void _notification(int p_what);
	void _gui_input(Ref<InputEvent> p_event) override;
	UI_Button* up_;
	UI_Slider* slider_;
	UI_Button* down_;
	String grid_;
	String skin_;
	int clipX_;
	int clipY_;
	bool hv_;
	float value_;
};
#endif
