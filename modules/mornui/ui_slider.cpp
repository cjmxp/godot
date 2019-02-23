#include "ui_slider.h"
#include "core/os/os.h"

UI_Slider::UI_Slider() {
	set_focus_mode(FOCUS_ALL);
	set_mouse_filter(Control::MOUSE_FILTER_STOP);
	set_clip_contents(false);
	but = memnew(UI_Button);
	but->set_mouse_filter(Control::MOUSE_FILTER_PASS);
	add_child(but);
}
UI_Slider::~UI_Slider() {
}

void UI_Slider::SetSkin(const String& v) {
	skin_ = v;
	texture_ = ResourceLoader::load(v);
	but->SetClipY(3);
	but->SetSkin(skin_.replace(".png", "$bar.png"));
	update();
}

void UI_Slider::InitAttribute(Ref<XMLNode> node,ScriptInstance* self) {
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
void UI_Slider::_gui_input(Ref<InputEvent> p_event) {
	Ref<InputEventMouseButton> b = p_event;
	if (b.is_valid()) {
		if (b->is_pressed()) {
			but->SetIndex(2);
			loack_ = true;
			real_t wh = 0.0f;
			real_t xy = 0.0f;
			real_t swh = 0.0f;
			if (hv_) {
				wh = but->get_size().width / 2;
				xy = b->get_position().x;
				swh = get_size().width;
			}
			else {
				wh = but->get_size().height / 2;
				xy = b->get_position().y;
				swh = get_size().height;
			}
			if (xy<=wh) {
				SetValue(0.0f);
				return;
			}
			if (xy >= swh -wh) {
				SetValue(1.0f);
				return;
			}
			SetValue((xy-wh)/(swh - wh*2));
		}
		else {
			loack_ = false;
		}
	}
	if (loack_) {
		Ref<InputEventMouseMotion> mm = p_event;
		if (mm.is_valid()) {
			real_t wh = 0.0f;
			real_t xy = 0.0f;
			real_t swh = 0.0f;
			if (hv_) {
				wh = but->get_size().width / 2;
				xy = mm->get_position().x;
				swh = get_size().width;
			}
			else {
				wh = but->get_size().height / 2;
				xy = mm->get_position().y;
				swh = get_size().height;
			}
			if (xy <= wh) {
				SetValue(0.0f);
				return;
			}
			if (xy >= swh - wh) {
				SetValue(1.0f);
				return;
			}
			SetValue((xy - wh) / (swh - wh * 2));
		}
	}
	
}
void UI_Slider::SetDirection(String v) {
	if (v == "horizontal") {
		hv_ = true;
	}
	else {
		hv_ = false;
	}
}
void UI_Slider::SetValue(float v) {
	if (v < 0)v = 0;
	if (v > 1)v = 1;
	Size2 size = get_size();
	Size2 bsize = but->get_size();
	real_t length = 0.0f;
	real_t x = 0.0f;
	real_t y = 0.0f;
	if (hv_) {
		length = size.width - bsize.width;
		x = length * v;
		y = (size.height - bsize.height)*0.5;
	}
	else {
		length = size.height - bsize.height;
		x = (size.width - bsize.width)*0.5;
		y = length * v; 
	}
	but->set_position(Point2(x, y));
	value_ = v;
}

void UI_Slider::_notification(int p_what) {
	if (!hslider_draw_)return;

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
		if (!init_) {
			if (get_size().width != 0 && but->get_size().width > 0) {
				init_ = true;
				SetValue(value_);
			}
		}
		/*
		RID ci = get_canvas_item();
		Size2 size = get_size();
		Color color;
		Ref<Font> font = get_font("font");
		int text_clip = size.width;
		
	//	but->setpo

		Vector<String> list =  xl_text.split("\n");
		real_t h = font->get_ascent()*list.size();
		real_t of = (size.height - h)*0.5;

		for (unsigned i = 0; i < list.size(); i++) {
			Point2 text_ofs = (size - font->get_string_size(list[i]));
			text_ofs.x *= 0.5;
			text_ofs.y = font->get_ascent() + of;
			of += font->get_ascent()+1;
			font->draw(ci, text_ofs.floor(), list[i], color, text_clip);
		}*/
	}
}

void UI_Slider::_bind_methods() {
}



