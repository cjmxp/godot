#include "ui_button.h"
#include "core/os/os.h"

UI_Button::UI_Button() {
	set_focus_mode(FOCUS_ALL);
	set_mouse_filter(Control::MOUSE_FILTER_STOP);
	set_clip_contents(true);
}
UI_Button::~UI_Button() {
}


void UI_Button::InitAttribute(Ref<XMLNode> node,ScriptInstance* self) {
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
void UI_Button::_gui_input(Ref<InputEvent> p_event) {
	Ref<InputEventMouseButton> b = p_event;
	if (b.is_valid()) {
		if (b->is_pressed() && index_ != 1) {
			if (clipY_ > 1) {
				if (tab_mode_) {
					index_ = index_ == 1 ? 2 : 1;
					selected_ = !selected_;
				}
				else {
					index_ = 1;
				}
				update();
			}
		}
		else if(!tab_mode_) {
			index_ = 2;
			update();
		}
		
	}
}
void UI_Button::_notification(int p_what) {
	if (!button_draw_)return;
	if (p_what == NOTIFICATION_MOUSE_ENTER && (!tab_mode_ || !selected_)) {
		if (clipY_ > 1) {
			index_ = 2;
			update();
		}
	}
	else if (p_what == NOTIFICATION_MOUSE_EXIT && (!tab_mode_ || !selected_)) {
		if (clipY_ > 1) {
			index_ = 0;
			update();
		}
	}
	if (p_what == NOTIFICATION_DRAW && text_!="") {

		RID ci = get_canvas_item();
		Size2 size = get_size();
		Color color;
		Ref<Font> font = get_font("font");
		int text_clip = size.width;

		if (tab_mode_)text_clip = -1;

		Vector<String> list = text_.split("\n");
		real_t h = font->get_ascent()*list.size();
		real_t of = (size.height - h)*0.5;
		real_t char_w = 0.0f;
		for (unsigned i = 0; i < list.size(); i++) {
			Size2 char_size = font->get_string_size(list[i]);
			Point2 text_ofs = (size - char_size);
			if (char_size.width > char_w)char_w = char_size.width;
			text_ofs.x *= 0.5;
			text_ofs.y = font->get_ascent() + of;
			of += font->get_ascent()+1;
			font->draw(ci, text_ofs.floor(), list[i], color, text_clip);
		}
		if (tab_mode_) {
			minsize_.width = char_w > minsize_.width ? char_w : minsize_.width;
			minsize_.height = h > minsize_.height ? h : minsize_.height;
			minimum_size_changed();
		}
	}
}

void UI_Button::SetLabel(const String& v) {
	if (text_!=v) {
		text_ = v;
		update();
	}
}

void UI_Button::SetSelected(bool v) {
	if (tab_mode_ && selected_!=v) {
		selected_ = v;
		index_ = v ? 1 : 0;
		update();
	}
}
void UI_Button::SetTabMode(bool v) {
	if (tab_mode_ != v) {
		tab_mode_ = v;
		if (!tab_mode_) {
			selected_ = false;
			index_ = 0;
			update();
		}
	}
}
void UI_Button::_bind_methods() {
}



