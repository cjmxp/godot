
#ifndef UI_TEXTINPUT_H
#define UI_TEXTINPUT_H
#include "modules/pugixml/pugixml_exporter.h"
#include "ui_clip.h"

class UI_TextInput : public UI_Clip {

	GDCLASS(UI_TextInput, UI_Clip);
	OBJ_CATEGORY("GUI Nodes");
public:
	enum Align {

		ALIGN_LEFT,
		ALIGN_CENTER,
		ALIGN_RIGHT,
		ALIGN_FILL
	};

	UI_TextInput();
	~UI_TextInput();
	void InitAttribute(Ref<XMLNode> node,ScriptInstance* self) override;
	void set_cursor_at_pixel_pos(int p_x);
	void set_cursor_position(int p_pos);
	void set_window_pos(int p_pos);
protected:
	static void _bind_methods();
	void _notification(int p_what);
	void _gui_input(Ref<InputEvent> p_event) override;
	//virtual void _unhandled_input(Ref<InputEvent> p_event);
	//static void _bind_methods();
	String xl_text{"a\nb\nc\nd"};
	bool button_draw_{ true };
	bool select_{ false };
private:
	Align align{ ALIGN_LEFT };
	bool editable;
	bool pass;
	bool text_changed_dirty;
	int window_pos{ 0 };
	int cached_width{ 0 };
	int cursor_pos{ 0 };
	String text;
};
#endif
