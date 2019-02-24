
#ifndef UI_TAB_H
#define UI_TAB_H
#include "modules/pugixml/pugixml_exporter.h"
#include "ui_box.h"

class UI_Tab : public UI_Box {

	GDCLASS(UI_Tab, UI_Box);
	OBJ_CATEGORY("GUI Nodes");
public:
	UI_Tab();
	~UI_Tab();
	void InitAttribute(Ref<XMLNode> node,ScriptInstance* self) override;
	void SetLabels(const String& v);
	String GetLabels() { return labels_; };
	const String& GetSizeGrid() { return grid_; };
	void SetSizeGrid(const String& rect);
	const String& GetSkin()const { return skin_; };
	void SetSkin(const String& v);
	int GetClipX() { return clipX_; };
	void SetClipX(int);
	int GetClipY() { return clipY_; };
	void SetClipY(int);
	int GetSelectedIndex() { return selectedindex_; }
	void SetSelectedIndex(int v);
	void SetDirection(String v);
	void SetSpace(int v);
	void OnEvent(Ref<InputEvent> e) override;
protected:
	static void _bind_methods();
	void _notification(int p_what);
	void _gui_input(Ref<InputEvent> p_event) override;
	String labels_{""};
	String grid_{ "" };
	String skin_{ "" };
	int clipX_{ 1 };
	int clipY_{ 3 };
	bool setattribute_{ false };
	bool layout_{ false };
	int selectedindex_{ -1 };
	bool hv_{ true };
	int space_{ 1 };
};
#endif
