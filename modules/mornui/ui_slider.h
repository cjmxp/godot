
#ifndef UI_SLIDER_H
#define UI_SLIDER_H
#include "ui_clip.h"
#include "ui_button.h"
class UI_Slider : public UI_Clip {

	GDCLASS(UI_Slider, UI_Clip);
	OBJ_CATEGORY("GUI Nodes");
public:
	UI_Slider();
	~UI_Slider();
	void InitAttribute(Ref<XMLNode> node,ScriptInstance* self) override;
	virtual void SetSkin(const String& v)override;
	float GetValue() { return value_; };
	void SetValue(float v);
	void SetDirection(String v);
	void OnEvent(Ref<InputEvent> e) override;
protected:
	static void _bind_methods();
	void _notification(int p_what);
	void _gui_input(Ref<InputEvent> p_event) override;
	//virtual void _unhandled_input(Ref<InputEvent> p_event);
	//static void _bind_methods();
	bool hslider_draw_{true};
private:
	UI_Button* bar_{ NULL };
	float value_{ 0.0 };
	bool loack_{ false };
	bool hv_{ true };
	bool layout_{ false };
};
#endif
