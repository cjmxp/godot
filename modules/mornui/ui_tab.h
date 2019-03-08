
#ifndef UI_TAB_H
#define UI_TAB_H
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
	String GetDirection();
	void SetDirection(const String& v);
	int GetSpace() { return space_; };
	void SetSpace(int v);
	void OnEvent(Ref<InputEvent> e) override;
	String GetColor() { return color_; };
	void SetColor(const String& v);
	String GetFont() { return font_; };
	void SetFont(const String& v);
protected:
	static void _bind_methods();
	void _notification(int p_what);
	void _gui_input(Ref<InputEvent> p_event) override;
	String labels_;
	String grid_;
	String skin_;
	String font_;
	String color_;
	int clipX_;
	int clipY_;
	int selectedindex_;
	bool hv_;
	int space_;
};
#endif
