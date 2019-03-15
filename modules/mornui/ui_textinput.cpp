#include "ui_textinput.h"
#include "core/os/os.h"
#include "core/os/keyboard.h"
#include "core/message_queue.h"

UI_TextInput::UI_TextInput() {
	xl_text = "";
	textinput_draw_= true;
	select_= false;
 	align= ALIGN_LEFT;
	editable= true;
	pass= false;
	text_changed_dirty= false;
	window_pos= 0;
	cached_width= 0;
	cached_placeholder_width= 0;
	cursor_pos= 0;
	caret_blink_enabled= false;
	draw_caret= true;
	window_has_focus= true;
	expand_to_text_length= false;
	max_length=0; // 0 for no maximum
	placeholder_alpha= 0.6f;
	clear_button_enabled= false;
	text_="";
	color_= "";
	font_= "";
	font_size_= 12;
	secret_character="*";
	blink_ctimer_ = 0;

	set_focus_mode(FOCUS_ALL);
	set_mouse_filter(Control::MOUSE_FILTER_STOP);
	set_default_cursor_shape(CURSOR_IBEAM);
	undo_stack_pos = NULL;
	_create_undo_state();
	cursor_set_blink_enabled(true);
	set_clip_contents(true);

	selection_color_ = Color(0, 0.4, 0.8f);
	font_color_ = Color(1, 0, 0);
	font_color_selected_ = Color(1, 1, 1);
	cursor_color_ = Color(0, 0, 0);
}
UI_TextInput::~UI_TextInput() {
}

static bool _is_text_char(CharType c) {

	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_';
}

void UI_TextInput::InitAttribute(Ref<XMLNode> node,ScriptInstance* self) {
	UI_Clip::InitAttribute(node, self);
	Vector<Variant> array = node->get_attributes();
	for(unsigned i = 0;i<array.size();i++){
		Ref<XMLAttribute> attribute = array[i];
		String tag = attribute->name().to_lower();
		if (tag == "text") {
			SetText(attribute->value());
		}
		else if (tag == "font") {
			SetFont(attribute->value());
		}
		else if (tag == "color") {
			SetColor(attribute->value());
		}
		else if (tag == "size") {
			SetFontSize(attribute->value().to_int());
		}
	}
	setattribute_ = true;
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
					selection.end = text_.length();
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
			p_event->SetTarget(get_name());
			OnEvent(p_event);
		}
		else {
			if ((!selection.creating) && (!selection.doubleclick)) {
				deselect();
			}
			selection.creating = false;
			selection.doubleclick = false;

			if (OS::get_singleton()->has_virtual_keyboard())
				OS::get_singleton()->show_virtual_keyboard(text_, get_global_rect());
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
					text_ = text_.substr(cursor_pos, text_.length() - cursor_pos);

					Ref<Font> font = get_font("font");

					cached_width = 0;
					if (font != NULL) {
						for (int i = 0; i < text_.length(); i++)
							cached_width += font->get_char_size(text_[i]).width;
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
					text_ = text_.substr(0, cursor_pos);
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
				set_cursor_position(text_.length());
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
		_reset_caret_blink_timer();
		if (!k->get_metakey()) {

			bool handled = true;
			switch (code) {

			case KEY_KP_ENTER:
			case KEY_ENTER: {

				//emit_signal("text_entered", text);
				if (OS::get_singleton()->has_virtual_keyboard())
					OS::get_singleton()->hide_virtual_keyboard();

				return;
			} break;

			case KEY_BACKSPACE: {

				if (!editable)
					break;

				if (selection.enabled) {
					selection_delete();
					break;
				}

#ifdef APPLE_STYLE_KEYS
				if (k->get_alt()) {
#else
				if (k->get_alt()) {
					handled = false;
					break;
				}
				else if (k->get_command()) {
#endif
					int cc = cursor_pos;
					bool prev_char = false;

					while (cc > 0) {
						bool ischar = _is_text_char(text_[cc - 1]);

						if (prev_char && !ischar)
							break;

						prev_char = ischar;
						cc--;
					}

					delete_text(cc, cursor_pos);

					set_cursor_position(cc);

				}
				else {
					delete_char();
				}

				} break;
			case KEY_KP_4: {
				if (k->get_unicode() != 0) {
					handled = false;
					break;
				}
				// numlock disabled. fallthrough to key_left
			}
			case KEY_LEFT: {

#ifndef APPLE_STYLE_KEYS
				if (!k->get_alt())
#endif
					shift_selection_check_pre(k->get_shift());

#ifdef APPLE_STYLE_KEYS
				if (k->get_command()) {
					set_cursor_position(0);
				}
				else if (k->get_alt()) {

#else
				if (k->get_alt()) {
					handled = false;
					break;
				}
				else if (k->get_command()) {
#endif
					bool prev_char = false;
					int cc = cursor_pos;

					while (cc > 0) {
						bool ischar = _is_text_char(text_[cc - 1]);

						if (prev_char && !ischar)
							break;

						prev_char = ischar;
						cc--;
					}

					set_cursor_position(cc);

				}
				else {
					set_cursor_position(get_cursor_position() - 1);
				}

				shift_selection_check_post(k->get_shift());

				} break;
			case KEY_KP_6: {
				if (k->get_unicode() != 0) {
					handled = false;
					break;
				}
				// numlock disabled. fallthrough to key_right
			}
			case KEY_RIGHT: {

				shift_selection_check_pre(k->get_shift());

#ifdef APPLE_STYLE_KEYS
				if (k->get_command()) {
					set_cursor_position(text_.length());
				}
				else if (k->get_alt()) {
#else
				if (k->get_alt()) {
					handled = false;
					break;
				}
				else if (k->get_command()) {
#endif
					bool prev_char = false;
					int cc = cursor_pos;

					while (cc < text_.length()) {
						bool ischar = _is_text_char(text_[cc]);

						if (prev_char && !ischar)
							break;

						prev_char = ischar;
						cc++;
					}

					set_cursor_position(cc);

				}
				else {
					set_cursor_position(get_cursor_position() + 1);
				}

				shift_selection_check_post(k->get_shift());

				} break;
			case KEY_UP: {

				shift_selection_check_pre(k->get_shift());
				if (get_cursor_position() == 0) handled = false;
				set_cursor_position(0);
				shift_selection_check_post(k->get_shift());
			} break;
			case KEY_DOWN: {

				shift_selection_check_pre(k->get_shift());
				if (get_cursor_position() == text_.length()) handled = false;
				set_cursor_position(text_.length());
				shift_selection_check_post(k->get_shift());
			} break;
			case KEY_DELETE: {

				if (!editable)
					break;

				if (k->get_shift() && !k->get_command() && !k->get_alt()) {
					cut_text();
					break;
				}

				if (selection.enabled) {
					selection_delete();
					break;
				}

				int text_len = text_.length();

				if (cursor_pos == text_len)
					break; // nothing to do

#ifdef APPLE_STYLE_KEYS
				if (k->get_alt()) {
#else
				if (k->get_alt()) {
					handled = false;
					break;
				}
				else if (k->get_command()) {
#endif
					int cc = cursor_pos;

					bool prev_char = false;

					while (cc < text_.length()) {

						bool ischar = _is_text_char(text_[cc]);

						if (prev_char && !ischar)
							break;
						prev_char = ischar;
						cc++;
					}

					delete_text(cursor_pos, cc);

				}
				else {
					set_cursor_position(cursor_pos + 1);
					delete_char();
				}

				} break;
			case KEY_KP_7: {
				if (k->get_unicode() != 0) {
					handled = false;
					break;
				}
				// numlock disabled. fallthrough to key_home
			}
			case KEY_HOME: {

				shift_selection_check_pre(k->get_shift());
				set_cursor_position(0);
				shift_selection_check_post(k->get_shift());
			} break;
			case KEY_KP_1: {
				if (k->get_unicode() != 0) {
					handled = false;
					break;
				}
				// numlock disabled. fallthrough to key_end
			}
			case KEY_END: {

				shift_selection_check_pre(k->get_shift());
				set_cursor_position(text_.length());
				shift_selection_check_post(k->get_shift());
			} break;

			default: {

				handled = false;
			} break;
			}

			if (handled) {
				accept_event();
			}
			else if (!k->get_alt() && !k->get_command()) {
				if (k->get_unicode() >= 32 && k->get_scancode() != KEY_DELETE) {

					if (editable) {
						selection_delete();
						CharType ucodestr[2] = { (CharType)k->get_unicode(), 0 };
						append_at_cursor(ucodestr);
						_text_changed();
						accept_event();
					}

				}
				else {
					return;
				}
			}

			update();
		}
		p_event->SetTarget(get_name());
		OnEvent(p_event);
		return;
	}
}
void UI_TextInput::_notification(int p_what) {
	if (!textinput_draw_)return;
	if (p_what == NOTIFICATION_PROCESS && caret_blink_enabled) {
		focus_ = has_focus();
		uint32_t v = OS::get_singleton()->get_ticks_msec();
		if (blink_ctimer_ == 0) {
			blink_ctimer_ = v;
		}
		else if (v - blink_ctimer_ >= 600) {
			blink_ctimer_ = v;
			_toggle_draw_caret();
		}
		if (selection.enabled && !focus_) {
			deselect();
		}
	}
	else if (p_what == NOTIFICATION_MOUSE_ENTER) {
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
	switch (p_what) {
	case NOTIFICATION_RESIZED: {

		if (expand_to_text_length) {
			window_pos = 0; //force scroll back since it's expanding to text length
		}
		set_cursor_position(get_cursor_position());

	} break;
	case MainLoop::NOTIFICATION_WM_FOCUS_IN: {
		window_has_focus = true;
		draw_caret = true;
		update();
	} break;
	case MainLoop::NOTIFICATION_WM_FOCUS_OUT: {
		window_has_focus = false;
		draw_caret = false;
		update();
	} break;
	case NOTIFICATION_DRAW: {
		focus_ = has_focus();
		if (!focus_ || !window_has_focus) {
			draw_caret = false;
		}
		int width, height;

		Size2 size = get_size();
		width = size.width;
		height = size.height;

		RID ci = get_canvas_item();

			
		float disabled_alpha = 1.0; // used to set the disabled input text color
		if (!is_editable()) {
			disabled_alpha = .5;
			draw_caret = false;
		}

		Ref<Font> font = get_font("font");

		if (focus_) {
			index_ = 3;
		}

		int x_ofs = 0;
		bool using_placeholder = text_.empty();
		int cached_text_width = using_placeholder ? cached_placeholder_width : cached_width;
		switch (align) {

			case ALIGN_FILL:
			case ALIGN_LEFT: {

				x_ofs = 2;
			} break;
			case ALIGN_CENTER: {

				if (window_pos != 0)
					x_ofs = 2;
				else
					x_ofs = MAX(2, int(size.width - (cached_text_width)) / 2);
			} break;
			case ALIGN_RIGHT: {

				x_ofs = MAX(2, int(size.width - 2 - (cached_text_width)));
			} break;
		}
		int ofs_max = width - 2;
		int char_ofs = window_pos;

		int y_area = height;
		int y_ofs = (y_area - font->get_height()) / 2;

		int font_ascent = font->get_ascent();

		Color font_color = font_color_;
	

		const String &t = using_placeholder ? placeholder : text_;
		// draw placeholder color
		if (using_placeholder)
			font_color.a *= placeholder_alpha;
		font_color.a *= disabled_alpha;

		int caret_height = font->get_height() > y_area ? y_area : font->get_height();
		FontDrawer drawer(font, Color(1, 1, 1));
		while (true) {
			//end of string, break!
			if (char_ofs >= t.length())
				break;
			if (char_ofs == cursor_pos) {
				if (ime_text.length() > 0) {
					int ofs = 0;
					while (true) {
						if (ofs >= ime_text.length())
							break;

						CharType cchar = (pass && !text_.empty()) ? secret_character[0] : ime_text[ofs];
						CharType next = (pass && !text_.empty()) ? secret_character[0] : ime_text[ofs + 1];
						int im_char_width = font->get_char_size(cchar, next).width;

						if ((x_ofs + im_char_width) > ofs_max)
							break;

						bool selected = ofs >= ime_selection.x && ofs < ime_selection.x + ime_selection.y;
						if (selected) {
							VisualServer::get_singleton()->canvas_item_add_rect(ci, Rect2(Point2(x_ofs, y_ofs + caret_height), Size2(im_char_width, 3)), font_color);
						}
						else {
							VisualServer::get_singleton()->canvas_item_add_rect(ci, Rect2(Point2(x_ofs, y_ofs + caret_height), Size2(im_char_width, 1)), font_color);
						}

						drawer.draw_char(ci, Point2(x_ofs, y_ofs + font_ascent), cchar, next, font_color);

						x_ofs += im_char_width;
						ofs++;
					}
				}
			}

			CharType cchar = (pass && !text_.empty()) ? secret_character[0] : t[char_ofs];
			CharType next = (pass && !text_.empty()) ? secret_character[0] : t[char_ofs + 1];
			int char_width = font->get_char_size(cchar, next).width;
			// end of widget, break!
			if ((x_ofs + char_width) > ofs_max)
				break;

			bool selected = selection.enabled && char_ofs >= selection.begin && char_ofs < selection.end;

			if (selected)
				VisualServer::get_singleton()->canvas_item_add_rect(ci, Rect2(Point2(x_ofs, y_ofs), Size2(char_width, caret_height)), selection_color_);

			int yofs = y_ofs + (caret_height - font->get_height()) / 2;
			drawer.draw_char(ci, Point2(x_ofs, yofs + font_ascent), cchar, next, selected ? font_color_selected_ : font_color);

			if (char_ofs == cursor_pos && draw_caret) {
				if (ime_text.length() == 0) {

					VisualServer::get_singleton()->canvas_item_add_rect(ci, Rect2(Point2(x_ofs, y_ofs), Size2(1, caret_height)), cursor_color_);

				}
			}

			x_ofs += char_width;
			char_ofs++;
		}

		if (char_ofs == cursor_pos) {
			if (ime_text.length() > 0) {
				int ofs = 0;
				while (true) {
					if (ofs >= ime_text.length())
						break;

					CharType cchar = (pass && !text_.empty()) ? secret_character[0] : ime_text[ofs];
					CharType next = (pass && !text_.empty()) ? secret_character[0] : ime_text[ofs + 1];
					int im_char_width = font->get_char_size(cchar, next).width;

					if ((x_ofs + im_char_width) > ofs_max)
						break;

					bool selected = ofs >= ime_selection.x && ofs < ime_selection.x + ime_selection.y;
					if (selected) {
						VisualServer::get_singleton()->canvas_item_add_rect(ci, Rect2(Point2(x_ofs, y_ofs + caret_height), Size2(im_char_width, 3)), font_color);
					}
					else {
						VisualServer::get_singleton()->canvas_item_add_rect(ci, Rect2(Point2(x_ofs, y_ofs + caret_height), Size2(im_char_width, 1)), font_color);
					}

					drawer.draw_char(ci, Point2(x_ofs, y_ofs + font_ascent), cchar, next, font_color);

					x_ofs += im_char_width;
					ofs++;
				}
			}
		}

		if (char_ofs == cursor_pos && draw_caret) { //may be at the end
			if (ime_text.length() == 0) {

				VisualServer::get_singleton()->canvas_item_add_rect(ci, Rect2(Point2(x_ofs, y_ofs), Size2(1, caret_height)), cursor_color_);

			}
		}

		if (focus_) {

			OS::get_singleton()->set_ime_active(true);
			OS::get_singleton()->set_ime_position(get_global_position() + Point2(using_placeholder ? 0 : x_ofs, y_ofs));
		}

	}break;
	case NOTIFICATION_FOCUS_ENTER: {

		if (!caret_blink_enabled) {
			draw_caret = true;
		}

		OS::get_singleton()->set_ime_active(true);
		Point2 cursor_pos = Point2(get_cursor_position(), 1) * get_minimum_size().height;
		OS::get_singleton()->set_ime_position(get_global_position() + cursor_pos);

		if (OS::get_singleton()->has_virtual_keyboard())
			OS::get_singleton()->show_virtual_keyboard(text_, get_global_rect());

	} break;
	case NOTIFICATION_FOCUS_EXIT: {

		OS::get_singleton()->set_ime_position(Point2());
		OS::get_singleton()->set_ime_active(false);
		ime_text = "";
		ime_selection = Point2();

		if (OS::get_singleton()->has_virtual_keyboard())
			OS::get_singleton()->hide_virtual_keyboard();

	} break;
	case MainLoop::NOTIFICATION_OS_IME_UPDATE: {
		focus_ = has_focus();
		if (focus_) {
			ime_text = OS::get_singleton()->get_ime_text();
			ime_selection = OS::get_singleton()->get_ime_selection();
			update();
		}
	} break;
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

void UI_TextInput::shift_selection_check_post(bool p_shift) {

	if (p_shift)
		selection_fill_at_cursor();
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

	int len = text_.length();
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

	if (!text_.length())
		return;

	selection.begin = 0;
	selection.end = text_.length();
	selection.enabled = true;
	update();
}

void UI_TextInput::delete_char() {

	if ((text_.length() <= 0) || (cursor_pos == 0)) return;

	Ref<Font> font = get_font("font");
	if (font != NULL) {
		cached_width -= font->get_char_size(text_[cursor_pos - 1]).width;
	}

	text_.erase(cursor_pos - 1, 1);

	set_cursor_position(get_cursor_position() - 1);

	_text_changed();
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
	text_ = op.text;
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
	text_ = op.text;
	set_cursor_position(op.cursor_pos);
	_emit_text_change();
}

void UI_TextInput::cut_text() {

	if (selection.enabled && !pass) {
		OS::get_singleton()->set_clipboard(text_.substr(selection.begin, selection.end - selection.begin));
		selection_delete();
	}
}

void UI_TextInput::copy_text() {

	if (selection.enabled && !pass) {
		OS::get_singleton()->set_clipboard(text_.substr(selection.begin, selection.end - selection.begin));
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

	if (text_.size() > 0) {
		Ref<Font> font = get_font("font");
		if (font != NULL) {
			for (int i = p_from_column; i < p_to_column; i++)
				cached_width -= font->get_char_size(text_[i]).width;
		}
	}
	else {
		cached_width = 0;
	}

	text_.erase(p_from_column, p_to_column - p_from_column);
	cursor_pos -= CLAMP(cursor_pos - p_from_column, 0, p_to_column - p_from_column);

	if (cursor_pos >= text_.length()) {

		cursor_pos = text_.length();
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

void UI_TextInput::_text_changed() {

	if (expand_to_text_length)
		minimum_size_changed();

	_emit_text_change();
	_clear_redo();
}

void UI_TextInput::_emit_text_change() {
	//emit_signal("text_changed", text);
	_change_notify("text");
	text_changed_dirty = false;
}

void UI_TextInput::_clear_redo() {
	_create_undo_state();
	if (undo_stack_pos == NULL) {
		return;
	}

	undo_stack_pos = undo_stack_pos->next();
	while (undo_stack_pos) {
		List<TextOperation>::Element *elem = undo_stack_pos;
		undo_stack_pos = undo_stack_pos->next();
		undo_stack.erase(elem);
	}
	_create_undo_state();
}

void UI_TextInput::clear_internal() {

	_clear_undo_stack();
	cached_width = 0;
	cursor_pos = 0;
	window_pos = 0;
	undo_text = "";
	text_ = "";
	update();
}

void UI_TextInput::_clear_undo_stack() {
	undo_stack.clear();
	undo_stack_pos = NULL;
	_create_undo_state();
}

void UI_TextInput::_create_undo_state() {
	TextOperation op;
	op.text = text_;
	op.cursor_pos = cursor_pos;
	undo_stack.push_back(op);
}

void UI_TextInput::_reset_caret_blink_timer() {
	if (caret_blink_enabled) {
		draw_caret = true;
		update();
	}
}

void UI_TextInput::_toggle_draw_caret() {
	draw_caret = !draw_caret;
	if (is_visible_in_tree() && has_focus() && window_has_focus) {
		update();
	}
}

bool UI_TextInput::cursor_get_blink_enabled() const {
	return caret_blink_enabled;
}

void UI_TextInput::cursor_set_blink_enabled(const bool p_enabled) {
	caret_blink_enabled = p_enabled;
	set_process(p_enabled);
	draw_caret = true;
}

void UI_TextInput::set_editable(bool p_editable) {
	editable = p_editable;
	update();
}

bool UI_TextInput::is_editable() const {

	return editable;
}

int UI_TextInput::get_cursor_position() const {

	return cursor_pos;
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

	while (ofs < text_.length()) {

		int char_w = 0;
		if (font != NULL) {
			char_w = font->get_char_size(text_[ofs]).width;
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

	if (p_pos > (int)text_.length())
		p_pos = text_.length();

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

				if (i >= text_.length()) {
					//do not do this, because if the cursor is at the end, its just fine that it takes no space
					//accum_width = font->get_char_size(' ').width; //anything should do
				}
				else {
					accum_width += font->get_char_size(text_[i], i + 1 < text_.length() ? text_[i + 1] : 0).width; //anything should do
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

	if ((max_length <= 0) || (text_.length() + p_text.length() <= max_length)) {

		Ref<Font> font = get_font("font");
		if (font != NULL) {
			for (int i = 0; i < p_text.length(); i++)
				cached_width += font->get_char_size(p_text[i]).width;
		}
		else {
			cached_width = 0;
		}

		String pre = text_.substr(0, cursor_pos);
		String post = text_.substr(cursor_pos, text_.length() - cursor_pos);
		text_ = pre + p_text + post;
		set_cursor_position(cursor_pos + p_text.length());
	}
}

void UI_TextInput::set_window_pos(int p_pos) {

	window_pos = p_pos;
	if (window_pos < 0) window_pos = 0;
}

Ref<Font> UI_TextInput::get_font(const StringName &p_name, const StringName &p_type) const {
	if (cfont.is_null()) {
		return  Control::get_font(p_name);
	}
	return cfont;
}

void UI_TextInput::SetText(const String& v) {
	if (text_ != v && v != "") {
		text_ = v;
		clear_internal();
		update();
		cursor_pos = 0;
		window_pos = 0;
	}
}

void UI_TextInput::SetColor(const String& v) {
	if (color_ != v && v != "") {
		color_ = v;
		font_color_ = Color::hex_rgba(v.hex_to_int());
		update();
	}
}
void UI_TextInput::SetFontSize(int v) {
	if (font_size_ != v && v > 0 && font_ != "") {
		cfont = Morn::get_singleton()->GetFont(font_, font_size_);
		update();
	}
}
void UI_TextInput::SetFont(const String& v) {
	if (font_ != v && v != "") {
		font_ = v;
		cfont = Morn::get_singleton()->GetFont(font_, font_size_);
		update();
	}
}

void UI_TextInput::_bind_methods() {
	ClassDB::bind_method(D_METHOD("GetText"), &UI_TextInput::GetText);
	ClassDB::bind_method(D_METHOD("SetText"), &UI_TextInput::SetText);
	//ClassDB::bind_method(D_METHOD("GetAlign"), &UI_TextInput::GetAlign);
	//ClassDB::bind_method(D_METHOD("SetAlign"), &UI_TextInput::SetAlign);
	ClassDB::bind_method(D_METHOD("GetColor"), &UI_TextInput::GetColor);
	ClassDB::bind_method(D_METHOD("SetColor"), &UI_TextInput::SetColor);
	ClassDB::bind_method(D_METHOD("GetFont"), &UI_TextInput::GetFont);
	ClassDB::bind_method(D_METHOD("SetFont"), &UI_TextInput::SetFont);
	ClassDB::bind_method(D_METHOD("GetFontSize"), &UI_TextInput::GetFontSize);
	ClassDB::bind_method(D_METHOD("SetFontSize"), &UI_TextInput::SetFontSize);
}



