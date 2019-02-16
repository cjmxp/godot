
#ifndef BOX_H
#define BOX_H
#include "modules/pugixml/pugixml_exporter.h"
#include "scene/gui/control.h"

class Box : public Control {

	GDCLASS(Box, Control);
	OBJ_CATEGORY("GUI Nodes");
public:
	Box();
	~Box();
	void Xml(const Variant& self, const String &v);
	void SetXml(Ref<XMLNode> node, ScriptInstance* self);
	virtual void InitAttribute(Ref<XMLNode> node,ScriptInstance* self);
	void InitChilds(Ref<XMLNode> node,ScriptInstance* self);
protected:
	static void _bind_methods();
	Ref<XMLDocument> dom;
};
#endif
