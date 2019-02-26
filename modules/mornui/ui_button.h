
#ifndef UI_BUTTON_H
#define UI_BUTTON_H
#include "ui_clip.h"

class UI_Button : public UI_Clip {

	GDCLASS(UI_Button, UI_Clip);
	OBJ_CATEGORY("GUI Nodes");
public:
	UI_Button();
	~UI_Button();
	void InitAttribute(Ref<XMLNode> node,ScriptInstance* self) override;
	bool GetTabMode() { return tab_mode_; };
	void SetTabMode(bool v);
	bool GetSelected() { return selected_; };
	void SetSelected(bool v);
	String GetLabel() { return text_; };
	void SetLabel(const String& v);
protected:
	static void _bind_methods();
	void _notification(int p_what);
	void _gui_input(Ref<InputEvent> p_event) override;
	//virtual void _unhandled_input(Ref<InputEvent> p_event);
	//static void _bind_methods();
	String text_{""};
	bool button_draw_{ true };
	bool tab_mode_ { false };
	bool selected_{false};

};
#endif
