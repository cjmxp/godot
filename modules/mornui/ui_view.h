
#ifndef UI_VIEW_H
#define UI_VIEW_H
#include "modules/pugixml/pugixml_exporter.h"
#include "ui_box.h"

class UI_View : public UI_Box {

	GDCLASS(UI_View, UI_Box);
	OBJ_CATEGORY("GUI Nodes");
public:
	UI_View();
	~UI_View();
	
	void OnEvent(Ref<InputEvent> e) override;
protected:
	static void _bind_methods();
};
#endif
