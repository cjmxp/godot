
#ifndef UI_LABEL_H
#define UI_LABEL_H
#include "modules/pugixml/pugixml_exporter.h"
#include "ui_clip.h"

class UI_Label : public UI_Clip {

	GDCLASS(UI_Label, UI_Clip);
	OBJ_CATEGORY("GUI Nodes");
public:
	UI_Label();
	~UI_Label();
	void InitAttribute(Ref<XMLNode> node,ScriptInstance* self) override;
protected:
	static void _bind_methods();
	void _notification(int p_what);
	void _gui_input(Ref<InputEvent> p_event) override;
	//virtual void _unhandled_input(Ref<InputEvent> p_event);
	//static void _bind_methods();
	bool label_draw_{ true };
	String xl_text{"buttonasdfasdfasdfasdfdasdsafasfasdsdafsdsd"};
private:
	enum Align {
		ALIGN_LEFT,
		ALIGN_CENTER,
		ALIGN_RIGHT,
		ALIGN_FILL
	};
	Align align{ ALIGN_LEFT };
	bool autowrap{ false };
	bool clip{ false };
	int line_count{ 0 };
	bool uppercase{ false };
	int total_char_cache{ 0 };
	Size2 minsize;
	int max_lines_visible{ -1 };
	bool word_cache_dirty{ true };
	int lines_skipped{ 0 };
	int visible_chars{ -1 };


	struct WordCache {

		enum {
			CHAR_NEWLINE = -1,
			CHAR_WRAPLINE = -2
		};
		int char_pos; // if -1, then newline
		int word_len;
		int pixel_width;
		int space_count;
		WordCache *next{ NULL };
		WordCache() {
			char_pos = 0;
			word_len = 0;
			pixel_width = 0;
			next = 0;
			space_count = 0;
		}
	};
	WordCache *word_cache{ NULL };
	int get_longest_line_width() const;
	void regenerate_word_cache();
};
#endif
