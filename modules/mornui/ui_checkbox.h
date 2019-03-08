
#ifndef UI_CHECKBOX_H
#define UI_CHECKBOX_H
#include "ui_button.h"

class UI_CheckBox  : public UI_Button {

	GDCLASS(UI_CheckBox, UI_Button);
	OBJ_CATEGORY("GUI Nodes");
public:
	UI_CheckBox();
	~UI_CheckBox();
	void InitAttribute(Ref<XMLNode> node,ScriptInstance* self) override;
protected:
	static void _bind_methods();
	void _notification(int p_what);
	void _gui_input(Ref<InputEvent> p_event) override;
	//virtual void _unhandled_input(Ref<InputEvent> p_event);
	bool checkbox_draw_;
};
#endif
