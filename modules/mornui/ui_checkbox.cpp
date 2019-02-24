#include "ui_checkbox.h"
#include "core/os/os.h"

UI_CheckBox::UI_CheckBox() {
	clip_draw_ = false;
	button_draw_ = false;
}
UI_CheckBox::~UI_CheckBox() {
}


void UI_CheckBox::InitAttribute(Ref<XMLNode> node,ScriptInstance* self) {
	UI_Button::InitAttribute(node, self);
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
void UI_CheckBox::_gui_input(Ref<InputEvent> p_event) {
	Ref<InputEventMouseButton> b = p_event;
	if (b.is_valid()) {
		if (b->is_pressed()) {
			if (clipY_ > 1) {
				index_ = index_ == 1 ? 2 : 1;
			}
		}
		update();
	}
}

void UI_CheckBox::_notification(int p_what) {
	if (!checkbox_draw_)return;
	if (p_what == NOTIFICATION_MOUSE_ENTER && index_ != 1) {
		if (clipY_ > 1) {
			index_ = 2;
		}
		update();
	}
	else if (p_what == NOTIFICATION_MOUSE_EXIT && index_ != 1) {
		if (clipY_ > 1) {
			index_ = 0;
		}
		update();
	}
	
	if (p_what == NOTIFICATION_DRAW) {

		RID ci = get_canvas_item();
		Size2 size = get_size();
		int w = 0;
		int h = 0;
		if (!texture_.is_null()) {
			w = texture_->get_width() / clipX_;
			h = texture_->get_height() / clipY_;
			int y = index_ / clipX_;
			int x = index_ % clipX_;

			Rect2 rect = Rect2(Point2(0, (size.height-w)), Point2(w,h));
			Rect2 src_rect;
			texture_->get_rect_region(rect, src_rect, rect, src_rect);
			if (index_ < 0) {
				index_ = 0;

			}
			else if (index_ >= clipX_ * clipY_)
			{
				index_ = 0;
			}

			
			src_rect.position.x += (x * w);
			src_rect.position.y += (y * h);
			src_rect.size.x = w;
			src_rect.size.y = h;

			texture_->draw_rect_region(ci, rect, src_rect);
		}
		if (text_ != "") {
			Color color;
			Ref<Font> font = get_font("font");
			Point2 text_ofs = font->get_string_size(text_);
			int text_clip = text_ofs.width;
			text_ofs.x = (w + 2);
			text_ofs.y = size.height - text_ofs.height + font->get_ascent();
			font->draw(ci, text_ofs.floor(), text_, color, text_clip);
			minsize_.width = text_ofs.x + text_clip;
			minsize_.height = text_ofs.height > h ? text_ofs.height : h;
			if (minsize_.width > size.width || minsize_.height > size.height)minimum_size_changed();
		}
	}
}

Size2 UI_CheckBox::get_minimum_size() const {
	
		return minsize_;

}
void UI_CheckBox::_bind_methods() {
}



