#include "ui_textinput.h"
#include "core/os/os.h"

UI_TextInput::UI_TextInput() {
	set_focus_mode(FOCUS_ALL);
	set_mouse_filter(Control::MOUSE_FILTER_STOP);
}
UI_TextInput::~UI_TextInput() {
}


void UI_TextInput::InitAttribute(Ref<XMLNode> node,ScriptInstance* self) {
	UI_Clip::InitAttribute(node, self);
	/*Vector<Variant> array = node->get_attributes();
	for(unsigned i = 0;i<array.size();i++){
		Ref<XMLAttribute> attribute = array[i];
		String tag = attribute->name().to_lower();
		if (tag == "skin") {
			SetSkin(attribute->value());
		}
		else if (tag == "sizegrid") {
			SetSizeGrid(attribute->value());
		}
		else if (tag=="clipx") {
			SetClipX(attribute->value().to_int());
		}
		else if (tag == "clipy") {
			SetClipY(attribute->value().to_int());
		}
		else if (tag == "index") {
			SetIndex(attribute->value().to_int());
		}
		else if (tag == "interval") {
			SetInterval(attribute->value().to_int());
		}
		else if (tag == "autoplay") {
			if (attribute->value() == "true") {
				SetAutoPlay(true);
			}
			else {
				SetAutoPlay(false);
			}
		}
	}*/
}
void UI_TextInput::_gui_input(Ref<InputEvent> p_event) {
	Ref<InputEventMouseButton> b = p_event;
	if (b.is_valid()) {
		if (b->is_pressed()) {
			if (clipY_ > 1) {
				index_ = 2;
			}
		}
		else {
			index_ = 1;
		}
		update();
	}
}
void UI_TextInput::_notification(int p_what) {
	if (!button_draw_)return;
	if (p_what == NOTIFICATION_MOUSE_ENTER) {
		if (clipY_ > 1) {
			index_ = 1;
		}
		update();
	}
	else if (p_what == NOTIFICATION_MOUSE_EXIT) {
		if (clipY_ > 1) {
			index_ = 0;
		}
		update();
	}
	if (p_what == NOTIFICATION_DRAW) {

		RID ci = get_canvas_item();
		Size2 size = get_size();
		Color color;
		Ref<Font> font = get_font("font");
		int text_clip = size.width;

		Vector<String> list =  xl_text.split("\n");
		real_t h = font->get_ascent()*list.size();
		real_t of = (size.height - h)*0.5;

		for (unsigned i = 0; i < list.size(); i++) {
			Point2 text_ofs = (size - font->get_string_size(list[i]));
			text_ofs.x *= 0.5;
			text_ofs.y = font->get_ascent() + of;
			of += font->get_ascent()+1;
			font->draw(ci, text_ofs.floor(), list[i], color, text_clip);
		}
	}
}
void UI_TextInput::set_cursor_at_pixel_pos(int p_x) {

	Ref<Font> font = get_font("font");
	int ofs = window_pos;
	Ref<StyleBox> style = get_stylebox("normal");
	int pixel_ofs = 0;
	Size2 size = get_size();

	switch (align) {

		case ALIGN_FILL:
		case ALIGN_LEFT: {

			pixel_ofs = int(style->get_offset().x);
		} break;
		case ALIGN_CENTER: {

			if (window_pos != 0)
				pixel_ofs = int(style->get_offset().x);
			else
				pixel_ofs = int(size.width - (cached_width)) / 2;
		} break;
		case ALIGN_RIGHT: {

			pixel_ofs = int(size.width - style->get_margin(MARGIN_RIGHT) - (cached_width));
		} break;
	}

	while (ofs < text.length()) {

		int char_w = 0;
		if (font != NULL) {
			char_w = font->get_char_size(text[ofs]).width;
		}
		pixel_ofs += char_w;

		if (pixel_ofs > p_x) { //found what we look for
			break;
		}

		ofs++;
	}

	set_cursor_position(ofs);
}
void UI_TextInput::set_cursor_position(int p_pos) {

	if (p_pos > (int)text.length())
		p_pos = text.length();

	if (p_pos < 0)
		p_pos = 0;

	cursor_pos = p_pos;

	if (!is_inside_tree()) {

		window_pos = cursor_pos;
		return;
	}

	Ref<StyleBox> style = get_stylebox("normal");
	Ref<Font> font = get_font("font");

	if (cursor_pos <= window_pos) {
		/* Adjust window if cursor goes too much to the left */
		if (window_pos > 0)
			set_window_pos(window_pos - 1);

	}
	else if (cursor_pos > window_pos) {
		/* Adjust window if cursor goes too much to the right */
		int window_width = get_size().width - style->get_minimum_size().width;

		if (window_width < 0)
			return;
		int wp = window_pos;

		if (font.is_valid()) {

			int accum_width = 0;

			for (int i = cursor_pos; i >= window_pos; i--) {

				if (i >= text.length()) {
					//do not do this, because if the cursor is at the end, its just fine that it takes no space
					//accum_width = font->get_char_size(' ').width; //anything should do
				}
				else {
					accum_width += font->get_char_size(text[i], i + 1 < text.length() ? text[i + 1] : 0).width; //anything should do
				}
				if (accum_width > window_width)
					break;

				wp = i;
			}
		}

		if (wp != window_pos)
			set_window_pos(wp);
	}
	update();
}
void UI_TextInput::set_window_pos(int p_pos) {

	window_pos = p_pos;
	if (window_pos < 0) window_pos = 0;
}
void UI_TextInput::_bind_methods() {
}



