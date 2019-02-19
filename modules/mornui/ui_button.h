
#ifndef UI_BUTTON_H
#define UI_BUTTON_H
#include "modules/pugixml/pugixml_exporter.h"
#include "ui_clip.h"

class UI_Button : public UI_Clip {

	GDCLASS(UI_Button, UI_Clip);
	OBJ_CATEGORY("GUI Nodes");
public:
	UI_Button();
	~UI_Button();
	void InitAttribute(Ref<XMLNode> node,ScriptInstance* self) override;
protected:
	static void _bind_methods();
	void _notification(int p_what);
	void _gui_input(Ref<InputEvent> p_event) override;
	//virtual void _unhandled_input(Ref<InputEvent> p_event);
	//static void _bind_methods();
	String xl_text{"button"};
	bool button_draw_{ true };
	bool select_{ false };
};
#endif
