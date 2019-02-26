
#ifndef UI_TEXTINPUT_H
#define UI_TEXTINPUT_H
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
	int get_cursor_position() const;
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
	void set_editable(bool p_editable);
	bool is_editable() const;
	void delete_char();
	bool cursor_get_blink_enabled() const;
	void cursor_set_blink_enabled(const bool p_enabled);
protected:
	static void _bind_methods();
	void _notification(int p_what);
	void _gui_input(Ref<InputEvent> p_event) override;
	//virtual void _unhandled_input(Ref<InputEvent> p_event);
	//static void _bind_methods();
	String xl_text{"input"};
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
	bool editable{ true };
	bool pass{ true };
	bool text_changed_dirty{ false };
	int window_pos{ 0 };
	int cached_width{ 0 };
	int cached_placeholder_width{ 0 };
	int cursor_pos{ 0 };
	bool caret_blink_enabled{ false };
	bool draw_caret{ true };
	bool window_has_focus{ true };
	bool expand_to_text_length{ false };
	int max_length{0}; // 0 for no maximum
	float placeholder_alpha{ 0.6f };
	String ime_text;
	Point2 ime_selection;
	bool clear_button_enabled{ false };
	struct TextOperation {
		int cursor_pos;
		String text;
	};
	List<TextOperation> undo_stack;
	List<TextOperation>::Element *undo_stack_pos;
	String text;
	String placeholder;
	String undo_text;
	String secret_character{"*"};
	uint32_t blink_ctimer_{ 0 };

	void _clear_undo_stack();
	void _clear_redo();
	void _create_undo_state();
	void _reset_caret_blink_timer();
	void _toggle_draw_caret();
	void shift_selection_check_pre(bool);
	void shift_selection_check_post(bool);
	void selection_fill_at_cursor();
	void clear_internal();
	void _text_changed();
	void _emit_text_change();
};
#endif
