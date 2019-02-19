
#ifndef UI_LABEL_H
#define UI_LABEL_H
#include "modules/pugixml/pugixml_exporter.h"
#include "ui_clip.h"

class UI_Label : public UI_Clip {

	GDCLASS(UI_Label, UI_Clip);
	OBJ_CATEGORY("GUI Nodes");
public:
	UI_Label();
	~UI_Label();
	void InitAttribute(Ref<XMLNode> node,ScriptInstance* self) override;
protected:
	static void _bind_methods();
	void _notification(int p_what);
	void _gui_input(Ref<InputEvent> p_event) override;
	//virtual void _unhandled_input(Ref<InputEvent> p_event);
	//static void _bind_methods();
	bool label_draw_{ true };
	String xl_text{"button"};
};
#endif
