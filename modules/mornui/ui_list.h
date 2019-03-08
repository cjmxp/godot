
#ifndef UI_LIST_H
#define UI_LIST_H
#include "ui_box.h"
#include "ui_scrollbar.h"
class UI_List : public UI_Box {

	GDCLASS(UI_List, UI_Box);
	OBJ_CATEGORY("GUI Nodes");
public:
	UI_List();
	~UI_List();
	void InitAttribute(Ref<XMLNode> node,ScriptInstance* self) override;
	void InitChilds(Ref<XMLNode> node, ScriptInstance* self) override;
	int GetSelectedIndex() { return selectedindex_; }
	void SetSelectedIndex(int v);
	int GetSpaceX() { return spacex_; };
	void SetSpaceX(int v);
	int GetSpaceY() { return spacey_; };
	void SetSpaceY(int v);
	int GetRepeatX() { return repeatx_; };
	void SetRepeatX(int v);
	int GetRepeatY() { return repeaty_; };
	void SetRepeatY(int v);
	void OnEvent(Ref<InputEvent> e) override;
	void SetData(const Variant& arr);
protected:
	static void _bind_methods();
	void _notification(int p_what);
	void _gui_input(Ref<InputEvent> p_event) override;
	int selectedindex_;
	int spacex_;
	int spacey_;
	int repeatx_;
	int repeaty_;
	UI_Box* box_;
	UI_Box* content_;
	Ref<XMLNode> render_;
	Vector<Variant> data_;
	UI_ScrollBar* scrollbar_;
private:
	void scroll(String v);
	Point2 node_size;
};
#endif
