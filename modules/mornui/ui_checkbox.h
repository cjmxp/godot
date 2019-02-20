
#ifndef UI_CHECKBOX_H
#define UI_CHECKBOX_H
#include "modules/pugixml/pugixml_exporter.h"
#include "ui_button.h"

class UI_CheckBox  : public UI_Button {

	GDCLASS(UI_CheckBox, UI_Button);
	OBJ_CATEGORY("GUI Nodes");
public:
	UI_CheckBox();
	~UI_CheckBox();
	virtual Size2 get_minimum_size() const;
	void InitAttribute(Ref<XMLNode> node,ScriptInstance* self) override;
protected:
	static void _bind_methods();
	void _notification(int p_what);
	void _gui_input(Ref<InputEvent> p_event) override;
	//virtual void _unhandled_input(Ref<InputEvent> p_event);
	bool checkbox_draw_{ true };
private:
	Size2 minsize_;
};
#endif
