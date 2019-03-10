
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

	String GetText() { return text_; };
	void SetText(const String& v);
	String GetColor() { return color_; };
	void SetColor(const String& v);
	String GetFont() { return font_; };
	void SetFont(const String& v);
	int GetFontSize() { return font_size_; };
	void SetFontSize(int v);
	Ref<Font> get_font(const StringName &p_name, const StringName &p_type = StringName()) const;
protected:
	static void _bind_methods();
	void _notification(int p_what);
	void _gui_input(Ref<InputEvent> p_event) override;
	String xl_text;
	bool textinput_draw_;
	bool select_;
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

	Align align;
	bool editable;
	bool pass;
	bool text_changed_dirty;
	int window_pos;
	int cached_width;
	int cached_placeholder_width;
	int cursor_pos;
	bool caret_blink_enabled;
	bool draw_caret;
	bool window_has_focus;
	bool expand_to_text_length;
	int max_length; // 0 for no maximum
	float placeholder_alpha;
	String ime_text;
	Point2 ime_selection;
	bool clear_button_enabled;
	struct TextOperation {
		int cursor_pos;
		String text;
	};
	List<TextOperation> undo_stack;
	List<TextOperation>::Element *undo_stack_pos;
	String text_;
	String color_;
	String font_;
	int font_size_;
	Ref<Font> cfont;

	String placeholder;
	String undo_text;
	String secret_character;
	uint32_t blink_ctimer_;

	Color selection_color_;
	Color font_color_;
	Color font_color_selected_;
	Color cursor_color_;
	bool focus_;

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
