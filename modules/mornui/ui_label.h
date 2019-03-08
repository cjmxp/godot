
#ifndef UI_LABEL_H
#define UI_LABEL_H
#include "ui_clip.h"



class UI_Label : public UI_Clip {

	GDCLASS(UI_Label, UI_Clip);
	OBJ_CATEGORY("GUI Nodes");
public:
	UI_Label();
	~UI_Label();
	virtual Size2 get_minimum_size() const override;
	void InitAttribute(Ref<XMLNode> node, ScriptInstance* self) override;
	String GetAlign();
	void SetAlign(const String& v);
	bool GetAutowrap() { return autowrap_; };
	void SetAutowrap(bool v);
	bool GetClip() { return clip_; };
	void SetClip(bool v);
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
	//virtual void _unhandled_input(Ref<InputEvent> p_event);
	//static void _bind_methods();
	bool label_draw_;
	String text_;
private:
	enum Align {
		ALIGN_LEFT,
		ALIGN_CENTER,
		ALIGN_RIGHT,
		ALIGN_FILL
	};
	Align align_;
	bool autowrap_;
	bool clip_;
	int line_count;
	bool uppercase;
	int total_char_cache;
	int max_lines_visible;
	bool word_cache_dirty;
	int lines_skipped;
	int visible_chars;
	String font_;
	String color_;
	int font_size_;

	Ref<Font> cfont;

	struct WordCache {

		enum {
			CHAR_NEWLINE = -1,
			CHAR_WRAPLINE = -2
		};
		int char_pos; // if -1, then newline
		int word_len;
		int pixel_width;
		int space_count;
		WordCache *next;
		WordCache() {
			char_pos = 0;
			word_len = 0;
			pixel_width = 0;
			next = NULL;
			space_count = 0;
		}
	};
	WordCache *word_cache;
	int get_longest_line_width() const;
	void regenerate_word_cache();
};
#endif
