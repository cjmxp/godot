
#ifndef UI_BOX_H
#define UI_BOX_H
#include "modules/pugixml/pugixml_exporter.h"
#include "scene/gui/control.h"

class UI_Box : public Control {

	GDCLASS(UI_Box, Control);
	OBJ_CATEGORY("GUI Nodes");
public:
	UI_Box();
	~UI_Box();
	void Xml(const Variant& self, const String &v);
	void SetXml(Ref<XMLNode> node, ScriptInstance* self);
	virtual void InitAttribute(Ref<XMLNode> node,ScriptInstance* self);
	void MouseEnabled(bool v);
	void InitChilds(Ref<XMLNode> node,ScriptInstance* self);
	virtual Size2 get_minimum_size() const;
protected:
	static void _bind_methods();
	virtual void _gui_input(Ref<InputEvent> p_event);
	Ref<XMLDocument> dom;
	Size2 minsize_;
};
#endif
