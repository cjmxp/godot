#include "ui_label.h"
#include "core/os/os.h"

UI_Label::UI_Label() {
	set_clip_contents(true);
}
UI_Label::~UI_Label() {
}


void UI_Label::InitAttribute(Ref<XMLNode> node,ScriptInstance* self) {
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

Size2 UI_Label::get_minimum_size() const {

	// don't want to mutable everything
	if (word_cache_dirty)
		const_cast<UI_Label*>(this)->regenerate_word_cache();

	if (autowrap)
		return Size2(1, clip ? 1 : minsize_.height);
	else {
		Size2 ms = minsize_;
		if (clip)
			ms.width = 1;
		return ms;
	}
}

void UI_Label::_gui_input(Ref<InputEvent> p_event) {
	p_event->SetName(get_name());
	OnEvent(p_event);
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
void UI_Label::_notification(int p_what) {
	if (!label_draw_)return;
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
		if (clip) {
			VisualServer::get_singleton()->canvas_item_set_clip(get_canvas_item(), true);
		}
		if (word_cache_dirty)
			regenerate_word_cache();

		RID ci = get_canvas_item();

		Size2 string_size;
		Size2 size = get_size();
		Ref<StyleBox> style = get_stylebox("normal");
		Ref<Font> font = get_font("font");
		Color font_color = get_color("font_color");
		Color font_color_shadow = get_color("font_color_shadow");
		bool use_outline = get_constant("shadow_as_outline");
		Point2 shadow_ofs(get_constant("shadow_offset_x"), get_constant("shadow_offset_y"));
		int line_spacing = get_constant("line_spacing");
		Color font_outline_modulate = get_color("font_outline_modulate");

		//style->draw(ci, Rect2(Point2(0, 0), get_size()));

		VisualServer::get_singleton()->canvas_item_set_distance_field_mode(get_canvas_item(), font.is_valid() && font->is_distance_field_hint());

		int font_h = font->get_height() + line_spacing;

		int lines_visible = (size.y + line_spacing) / font_h;

		// ceiling to ensure autowrapping does not cut text
		int space_w = Math::ceil(font->get_char_size(' ').width);
		int chars_total = 0;

		int vbegin = 0, vsep = 0;

		if (lines_visible > line_count) {
			lines_visible = line_count;
		}

		if (max_lines_visible >= 0 && lines_visible > max_lines_visible) {
			lines_visible = max_lines_visible;
		}


		WordCache *wc = word_cache;
		if (!wc)
			return;

		int line = 0;
		int line_to = lines_skipped + (lines_visible > 0 ? lines_visible : 1);
		FontDrawer drawer(font, font_outline_modulate);
		while (wc) {
			/* handle lines not meant to be drawn quickly */
			if (line >= line_to)
				break;
			if (line < lines_skipped) {

				while (wc && wc->char_pos >= 0)
					wc = wc->next;
				if (wc)
					wc = wc->next;
				line++;
				continue;
			}

			/* handle lines normally */

			if (wc->char_pos < 0) {
				//empty line
				wc = wc->next;
				line++;
				continue;
			}

			WordCache *from = wc;
			WordCache *to = wc;

			int taken = 0;
			int spaces = 0;
			while (to && to->char_pos >= 0) {

				taken += to->pixel_width;
				if (to != from && to->space_count) {
					spaces += to->space_count;
				}
				to = to->next;
			}

			bool can_fill = to && to->char_pos == WordCache::CHAR_WRAPLINE;

			float x_ofs = 0;

			switch (align) {

			case ALIGN_FILL:
			case ALIGN_LEFT: {

				x_ofs = 0;
			} break;
			case ALIGN_CENTER: {

				x_ofs = int(size.width - (taken + spaces * space_w)) / 2;
			} break;
			case ALIGN_RIGHT: {

				x_ofs = int(size.width - style->get_margin(MARGIN_RIGHT) - (taken + spaces * space_w));
			} break;
			}


			float y_ofs = 0;
			y_ofs += (line - lines_skipped) * font_h + font->get_ascent();
			y_ofs += vbegin + line * vsep;

			while (from != to) {

				// draw a word
				int pos = from->char_pos;
				if (from->char_pos < 0) {

					ERR_PRINT("BUG");
					return;
				}
				if (from->space_count) {
					/* spacing */
					x_ofs += space_w * from->space_count;
					if (can_fill && align == ALIGN_FILL && spaces) {

						x_ofs += int((size.width - (taken + space_w * spaces)) / spaces);
					}
				}

				if (font_color_shadow.a > 0) {

					int chars_total_shadow = chars_total; //save chars drawn
					float x_ofs_shadow = x_ofs;
					for (int i = 0; i < from->word_len; i++) {

						if (visible_chars < 0 || chars_total_shadow < visible_chars) {
							CharType c = xl_text[i + pos];
							CharType n = xl_text[i + pos + 1];
							if (uppercase) {
								c = String::char_uppercase(c);
								n = String::char_uppercase(n);
							}

							float move = font->draw_char(ci, Point2(x_ofs_shadow, y_ofs) + shadow_ofs, c, n, font_color_shadow, false);
							if (use_outline) {
								font->draw_char(ci, Point2(x_ofs_shadow, y_ofs) + Vector2(-shadow_ofs.x, shadow_ofs.y), c, n, font_color_shadow, false);
								font->draw_char(ci, Point2(x_ofs_shadow, y_ofs) + Vector2(shadow_ofs.x, -shadow_ofs.y), c, n, font_color_shadow, false);
								font->draw_char(ci, Point2(x_ofs_shadow, y_ofs) + Vector2(-shadow_ofs.x, -shadow_ofs.y), c, n, font_color_shadow, false);
							}
							x_ofs_shadow += move;
							chars_total_shadow++;
						}
					}
				}
				for (int i = 0; i < from->word_len; i++) {

					if (visible_chars < 0 || chars_total < visible_chars) {
						CharType c = xl_text[i + pos];
						CharType n = xl_text[i + pos + 1];
						if (uppercase) {
							c = String::char_uppercase(c);
							n = String::char_uppercase(n);
						}

						x_ofs += drawer.draw_char(ci, Point2(x_ofs, y_ofs), c, n, font_color);
						chars_total++;
					}
				}
				from = from->next;
			}

			wc = to ? to->next : 0;
			line++;
		}
	}
}

int UI_Label::get_longest_line_width() const {

	Ref<Font> font = get_font("font");
	int max_line_width = 0;
	int line_width = 0;

	for (int i = 0; i < xl_text.size(); i++) {

		CharType current = xl_text[i];
		if (uppercase)
			current = String::char_uppercase(current);

		if (current < 32) {

			if (current == '\n') {

				if (line_width > max_line_width)
					max_line_width = line_width;
				line_width = 0;
			}
		}
		else {

			// ceiling to ensure autowrapping does not cut text
			int char_width = Math::ceil(font->get_char_size(current, xl_text[i + 1]).width);
			line_width += char_width;
		}
	}

	if (line_width > max_line_width)
		max_line_width = line_width;

	return max_line_width;
}

void UI_Label::regenerate_word_cache() {

	while (word_cache) {

		WordCache *current = word_cache;
		word_cache = current->next;
		memdelete(current);
	}

	int width = autowrap ? (get_size().width ) : get_longest_line_width();
	Ref<Font> font = get_font("font");

	int current_word_size = 0;
	int word_pos = 0;
	int line_width = 0;
	int space_count = 0;
	// ceiling to ensure autowrapping does not cut text
	int space_width = Math::ceil(font->get_char_size(' ').width);
	int line_spacing = get_constant("line_spacing");
	line_count = 1;
	total_char_cache = 0;

	WordCache *last = NULL;

	for (int i = 0; i <= xl_text.length(); i++) {

		CharType current = i < xl_text.length() ? xl_text[i] : L' '; //always a space at the end, so the algo works

		if (uppercase)
			current = String::char_uppercase(current);

		// ranges taken from http://www.unicodemap.org/
		// if your language is not well supported, consider helping improve
		// the unicode support in Godot.
		bool separatable = (current >= 0x2E08 && current <= 0xFAFF) || (current >= 0xFE30 && current <= 0xFE4F);
		//current>=33 && (current < 65||current >90) && (current<97||current>122) && (current<48||current>57);
		bool insert_newline = false;
		int char_width = 0;

		if (current < 33) {

			if (current_word_size > 0) {
				WordCache *wc = memnew(WordCache);
				if (word_cache) {
					last->next = wc;
				}
				else {
					word_cache = wc;
				}
				last = wc;

				wc->pixel_width = current_word_size;
				wc->char_pos = word_pos;
				wc->word_len = i - word_pos;
				wc->space_count = space_count;
				current_word_size = 0;
				space_count = 0;
			}

			if (current == '\n') {
				insert_newline = true;
			}
			else if (current != ' ') {
				total_char_cache++;
			}

			if (i < xl_text.length() && xl_text[i] == ' ') {
				if (line_width > 0 || last == NULL || last->char_pos != WordCache::CHAR_WRAPLINE) {
					space_count++;
					line_width += space_width;
				}
				else {
					space_count = 0;
				}
			}

		}
		else {
			// latin characters
			if (current_word_size == 0) {
				word_pos = i;
			}
			// ceiling to ensure autowrapping does not cut text
			char_width = Math::ceil(font->get_char_size(current, xl_text[i + 1]).width);
			current_word_size += char_width;
			line_width += char_width;
			total_char_cache++;
		}

		if ((autowrap && (line_width >= width) && ((last && last->char_pos >= 0) || separatable)) || insert_newline) {
			if (separatable) {
				if (current_word_size > 0) {
					WordCache *wc = memnew(WordCache);
					if (word_cache) {
						last->next = wc;
					}
					else {
						word_cache = wc;
					}
					last = wc;

					wc->pixel_width = current_word_size - char_width;
					wc->char_pos = word_pos;
					wc->word_len = i - word_pos;
					wc->space_count = space_count;
					current_word_size = char_width;
					space_count = 0;
					word_pos = i;
				}
			}

			WordCache *wc = memnew(WordCache);
			if (word_cache) {
				last->next = wc;
			}
			else {
				word_cache = wc;
			}
			last = wc;

			wc->pixel_width = 0;
			wc->char_pos = insert_newline ? WordCache::CHAR_NEWLINE : WordCache::CHAR_WRAPLINE;

			line_width = current_word_size;
			line_count++;
			space_count = 0;
		}
	}

	if (!autowrap)
		minsize_.width = width;

	if (max_lines_visible > 0 && line_count > max_lines_visible) {
		minsize_.height = (font->get_height() * max_lines_visible) + (line_spacing * (max_lines_visible - 1));
	}
	else {
		minsize_.height = (font->get_height() * line_count) + (line_spacing * (line_count - 1));
	}

	if (!autowrap || !clip) {
		//helps speed up some labels that may change a lot, as no resizing is requested. Do not change.
		minimum_size_changed();
	}
	word_cache_dirty = false;
}

void UI_Label::_bind_methods() {
}



