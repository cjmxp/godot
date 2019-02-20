#include "ui_textinput.h"
#include "core/os/os.h"
#include "core/os/keyboard.h"
#include "core/message_queue.h"

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
	if (b.is_valid() && b->get_button_index() == BUTTON_LEFT) {
		_reset_caret_blink_timer();
		if (b->is_pressed()) {
			shift_selection_check_pre(b->get_shift());
			set_cursor_at_pixel_pos(b->get_position().x);
			if (b->get_shift()) {

				selection_fill_at_cursor();
				selection.creating = true;

			}
			else {

				if (b->is_doubleclick()) {

					selection.enabled = true;
					selection.begin = 0;
					selection.end = text.length();
					selection.doubleclick = true;
				}

				selection.drag_attempt = false;

				if ((cursor_pos < selection.begin) || (cursor_pos > selection.end) || !selection.enabled) {

					deselect();
					selection.cursor_start = cursor_pos;
					selection.creating = true;
				}
				else if (selection.enabled) {

					selection.drag_attempt = true;
				}
			}

		}
		else {
			if ((!selection.creating) && (!selection.doubleclick)) {
				deselect();
			}
			selection.creating = false;
			selection.doubleclick = false;

			if (OS::get_singleton()->has_virtual_keyboard())
				OS::get_singleton()->show_virtual_keyboard(text, get_global_rect());
		}

		update();
	}

	Ref<InputEventMouseMotion> m = p_event;
	if (m.is_valid()) {

		if (m->get_button_mask() & BUTTON_LEFT) {

			if (selection.creating) {
				set_cursor_at_pixel_pos(m->get_position().x);
				selection_fill_at_cursor();
			}
		}
	}

	Ref<InputEventKey> k = p_event;
	if (k.is_valid()) {
		if (!k->is_pressed())
			return;

		unsigned int code = k->get_scancode();

		if (k->get_command()) {

			bool handled = true;

			switch (code) {

			case (KEY_X): { // CUT

				if (editable) {
					cut_text();
				}

			} break;

			case (KEY_C): { // COPY

				copy_text();

			} break;

			case (KEY_V): { // PASTE

				if (editable) {

					paste_text();
				}

			} break;

			case (KEY_Z): { // undo / redo
				if (editable) {
					if (k->get_shift()) {
						redo();
					}
					else {
						undo();
					}
				}
			} break;

			case (KEY_U): { // Delete from start to cursor

				if (editable) {

					deselect();
					text = text.substr(cursor_pos, text.length() - cursor_pos);

					Ref<Font> font = get_font("font");

					cached_width = 0;
					if (font != NULL) {
						for (int i = 0; i < text.length(); i++)
							cached_width += font->get_char_size(text[i]).width;
					}

					set_cursor_position(0);
					_text_changed();
				}

			} break;

			case (KEY_Y): { // PASTE (Yank for unix users)

				if (editable) {

					paste_text();
				}

			} break;
			case (KEY_K): { // Delete from cursor_pos to end

				if (editable) {

					deselect();
					text = text.substr(0, cursor_pos);
					_text_changed();
				}

			} break;
			case (KEY_A): { //Select All
				select();
			} break;
#ifdef APPLE_STYLE_KEYS
			case (KEY_LEFT): { // Go to start of text - like HOME key
				set_cursor_position(0);
			} break;
			case (KEY_RIGHT): { // Go to end of text - like END key
				set_cursor_position(text.length());
			} break;
#endif
			default: {
				handled = false;
			}
			}

			if (handled) {
				accept_event();
				return;
			}
		}

	}
}
void UI_TextInput::_notification(int p_what) {
	if (!textinput_draw_)return;
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
void UI_TextInput::selection_fill_at_cursor() {

	selection.begin = cursor_pos;
	selection.end = selection.cursor_start;

	if (selection.end < selection.begin) {
		int aux = selection.end;
		selection.end = selection.begin;
		selection.begin = aux;
	}

	selection.enabled = (selection.begin != selection.end);
}

void UI_TextInput::shift_selection_check_pre(bool p_shift) {

	if (!selection.enabled && p_shift) {
		selection.cursor_start = cursor_pos;
	}
	if (!p_shift)
		deselect();
}

void UI_TextInput::select(int p_from, int p_to) {

	if (p_from == 0 && p_to == 0) {
		deselect();
		return;
	}

	int len = text.length();
	if (p_from < 0)
		p_from = 0;
	if (p_from > len)
		p_from = len;
	if (p_to < 0 || p_to > len)
		p_to = len;

	if (p_from >= p_to)
		return;

	selection.enabled = true;
	selection.begin = p_from;
	selection.end = p_to;
	selection.creating = false;
	selection.doubleclick = false;
	update();
}

void UI_TextInput::select_all() {

	if (!text.length())
		return;

	selection.begin = 0;
	selection.end = text.length();
	selection.enabled = true;
	update();
}


void UI_TextInput::selection_delete() {

	if (selection.enabled)
		delete_text(selection.begin, selection.end);

	deselect();
}

void UI_TextInput::deselect() {

	selection.begin = 0;
	selection.end = 0;
	selection.cursor_start = 0;
	selection.enabled = false;
	selection.creating = false;
	selection.doubleclick = false;
	update();
}

void UI_TextInput::undo() {
	if (undo_stack_pos == NULL) {
		if (undo_stack.size() <= 1) {
			return;
		}
		undo_stack_pos = undo_stack.back();
	}
	else if (undo_stack_pos == undo_stack.front()) {
		return;
	}
	undo_stack_pos = undo_stack_pos->prev();
	TextOperation op = undo_stack_pos->get();
	text = op.text;
	set_cursor_position(op.cursor_pos);
	_emit_text_change();
}

void UI_TextInput::redo() {
	if (undo_stack_pos == NULL) {
		return;
	}
	if (undo_stack_pos == undo_stack.back()) {
		return;
	}
	undo_stack_pos = undo_stack_pos->next();
	TextOperation op = undo_stack_pos->get();
	text = op.text;
	set_cursor_position(op.cursor_pos);
	_emit_text_change();
}

void UI_TextInput::cut_text() {

	if (selection.enabled && !pass) {
		OS::get_singleton()->set_clipboard(text.substr(selection.begin, selection.end - selection.begin));
		selection_delete();
	}
}

void UI_TextInput::copy_text() {

	if (selection.enabled && !pass) {
		OS::get_singleton()->set_clipboard(text.substr(selection.begin, selection.end - selection.begin));
	}
}

void UI_TextInput::paste_text() {

	String paste_buffer = OS::get_singleton()->get_clipboard();

	if (paste_buffer != "") {

		if (selection.enabled) selection_delete();
		append_at_cursor(paste_buffer);

		if (!text_changed_dirty) {
			if (is_inside_tree()) {
				MessageQueue::get_singleton()->push_call(this, "_text_changed");
			}
			text_changed_dirty = true;
		}
	}
}

void UI_TextInput::delete_text(int p_from_column, int p_to_column) {

	if (text.size() > 0) {
		Ref<Font> font = get_font("font");
		if (font != NULL) {
			for (int i = p_from_column; i < p_to_column; i++)
				cached_width -= font->get_char_size(text[i]).width;
		}
	}
	else {
		cached_width = 0;
	}

	text.erase(p_from_column, p_to_column - p_from_column);
	cursor_pos -= CLAMP(cursor_pos - p_from_column, 0, p_to_column - p_from_column);

	if (cursor_pos >= text.length()) {

		cursor_pos = text.length();
	}
	if (window_pos > cursor_pos) {

		window_pos = cursor_pos;
	}

	if (!text_changed_dirty) {
		if (is_inside_tree()) {
			MessageQueue::get_singleton()->push_call(this, "_text_changed");
		}
		text_changed_dirty = true;
	}
}

void UI_TextInput::clear() {

	clear_internal();
	_text_changed();
}
void UI_TextInput::clear_internal() {

	_clear_undo_stack();
	cached_width = 0;
	cursor_pos = 0;
	window_pos = 0;
	undo_text = "";
	text = "";
	update();
}

void UI_TextInput::_clear_undo_stack() {
	undo_stack.clear();
	undo_stack_pos = NULL;
	_create_undo_state();
}

void UI_TextInput::_create_undo_state() {
	TextOperation op;
	op.text = text;
	op.cursor_pos = cursor_pos;
	undo_stack.push_back(op);
}

void UI_TextInput::_reset_caret_blink_timer() {
	if (caret_blink_enabled) {
		draw_caret = true;
		update();
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

void UI_TextInput::append_at_cursor(String p_text) {

	if ((max_length <= 0) || (text.length() + p_text.length() <= max_length)) {

		Ref<Font> font = get_font("font");
		if (font != NULL) {
			for (int i = 0; i < p_text.length(); i++)
				cached_width += font->get_char_size(p_text[i]).width;
		}
		else {
			cached_width = 0;
		}

		String pre = text.substr(0, cursor_pos);
		String post = text.substr(cursor_pos, text.length() - cursor_pos);
		text = pre + p_text + post;
		set_cursor_position(cursor_pos + p_text.length());
	}
}

void UI_TextInput::set_window_pos(int p_pos) {

	window_pos = p_pos;
	if (window_pos < 0) window_pos = 0;
}
void UI_TextInput::_bind_methods() {
}



