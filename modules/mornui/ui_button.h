
#ifndef UI_BUTTON_H
#define UI_BUTTON_H
#include "ui_clip.h"

class UI_Button : public UI_Clip {

	GDCLASS(UI_Button, UI_Clip);
	OBJ_CATEGORY("GUI Nodes");
public:
	UI_Button();
	~UI_Button();
	void InitAttribute(Ref<XMLNode> node,ScriptInstance* self) override;
	bool GetTabMode() { return tab_mode_; };
	void SetTabMode(bool v);
	bool GetSelected() { return selected_; };
	void SetSelected(bool v);
	String GetLabel() { return text_; };
	void SetLabel(const String& v);
	String GetColor();
	void SetColor(const String& v);
	int GetFontSize() { return font_size_; };
	void SetFontSize(int v);
	String GetFont();
	void SetFont(const String& v);
	Ref<Font> get_font(const StringName &p_name, const StringName &p_type = StringName()) const;
protected:
	static void _bind_methods();
	void _notification(int p_what);
	void _gui_input(Ref<InputEvent> p_event) override;
	String text_;
	bool button_draw_;
	bool tab_mode_;
	bool selected_;
	String color_;
	Color font_color_;
	String font_;
	int font_size_;
	Ref<Font> cfont;
};
#endif
