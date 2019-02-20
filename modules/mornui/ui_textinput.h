
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
	void deselect();
	void copy_text();
	void cut_text();
	void paste_text();
	void undo();
	void redo();
	void select(int p_from = 0, int p_to = -1);
	void select_all();
	void selection_delete();
	void delete_text(int p_from_column, int p_to_column);
	void append_at_cursor(String p_text);
	void clear();
protected:
	static void _bind_methods();
	void _notification(int p_what);
	void _gui_input(Ref<InputEvent> p_event) override;
	//virtual void _unhandled_input(Ref<InputEvent> p_event);
	//static void _bind_methods();
	String xl_text{"a\nb\nc\nd"};
	bool textinput_draw_{ true };
	bool select_{ false };
private:
	struct Selection {

		int begin;
		int end;
		int cursor_start;
		bool enabled;
		bool creating;
		bool doubleclick;
		bool drag_attempt;
	} selection;

	Align align{ ALIGN_LEFT };
	bool editable{ false };
	bool pass{ false };
	bool text_changed_dirty;
	int window_pos{ 0 };
	int cached_width{ 0 };
	int cursor_pos{ 0 };
	bool caret_blink_enabled{ false };
	bool draw_caret{ false };
	struct TextOperation {
		int cursor_pos;
		String text;
	};
	List<TextOperation> undo_stack;
	List<TextOperation>::Element *undo_stack_pos;
	String text;
	String undo_text;

	void _clear_undo_stack();
	void _create_undo_state();
	void _reset_caret_blink_timer();
	void shift_selection_check_pre(bool);
	void selection_fill_at_cursor();
	void clear_internal();
};
#endif
