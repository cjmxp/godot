#include "ui_button.h"
#include "core/os/os.h"

UI_Button::UI_Button() {
	set_focus_mode(FOCUS_ALL);
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
void UI_Button::_notification(int p_what) {
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

void UI_Button::_bind_methods() {
}



