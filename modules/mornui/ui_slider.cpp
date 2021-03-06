#include "ui_slider.h"
#include "core/os/os.h"

UI_Slider::UI_Slider() {
	hslider_draw_=true;
	bar_= NULL;
	value_= 0.0;
	loack_= false;
	hv_= true;
	layout_= false;
	set_focus_mode(FOCUS_ALL);
	set_mouse_filter(Control::MOUSE_FILTER_STOP);
	set_clip_contents(false);
	bar_ = memnew(UI_Button);
	bar_->set_mouse_filter(Control::MOUSE_FILTER_PASS);
	bar_->set_name("ui_slider_bar");
	add_child(bar_);
}
UI_Slider::~UI_Slider() {
}

void UI_Slider::SetSkin(const String& v) {
	if (skin_!=v && v!="") {
		skin_ = v;
		texture_ = Morn::get_singleton()->GetSkin(v);
		bar_->SetClipY(3);
		bar_->SetSkin(skin_.replace(".png", "$bar.png"));
		layout_ = true;
		set_process(true);
		update();
	}
	
}

void UI_Slider::InitAttribute(Ref<XMLNode> node,ScriptInstance* self) {
	UI_Clip::InitAttribute(node, self);
	Vector<Variant> array = node->get_attributes();
	for(unsigned i = 0;i<array.size();i++){
		Ref<XMLAttribute> attribute = array[i];
		String tag = attribute->name().to_lower();
		if (tag == "value") {
			SetValue(attribute->value().to_float());
		}
	
	}
}
void UI_Slider::OnEvent(Ref<InputEvent> e) {
	if (e->GetTarget() == "ui_slider_bar")return;
	UI_Box* p = Parent();
	if (p) {
		e->SetTarget(get_name());
		p->OnEvent(e);
	}
}

void UI_Slider::_gui_input(Ref<InputEvent> p_event) {
	p_event->SetTarget(get_name());
	Ref<InputEventMouseButton> b = p_event;
	if (b.is_valid()) {
		if (b->is_pressed()) {
			bar_->SetIndex(1);
			loack_ = true;
			real_t wh = 0.0f;
			real_t xy = 0.0f;
			real_t swh = 0.0f;
			if (hv_) {
				wh = bar_->get_size().width / 2;
				xy = b->get_position().x;
				swh = get_size().width;
			}
			else {
				wh = bar_->get_size().height / 2;
				xy = b->get_position().y;
				swh = get_size().height;
			}
			if (xy<=wh) {
				SetValue(0.0f);
				OnEvent(p_event);
				return;
			}
			if (xy >= swh -wh) {
				SetValue(1.0f);
				OnEvent(p_event);
				return;
			}
			SetValue((xy-wh)/(swh - wh*2));
			OnEvent(p_event);
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
				wh = bar_->get_size().width / 2;
				xy = mm->get_position().x;
				swh = get_size().width;
			}
			else {
				wh = bar_->get_size().height / 2;
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
			OnEvent(p_event);
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
	if (v < 0.0)v = 0.0;
	if (v > 1.0)v = 1.0;
	Size2 size = get_size();
	Size2 bsize = bar_->get_size();
	real_t length = 0.0f;
	real_t x = 0.0f;
	real_t y = 0.0f;
	if(size.width>0 && size.height>0 && bsize.width>0 && bsize.height>0) {
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
		bar_->set_position(Point2(x, y));
	}
	value_ = v;
}

void UI_Slider::_notification(int p_what) {
	if (!hslider_draw_)return;
	if (layout_) {
		Size2 size = get_size();
		Size2 bsize = bar_->get_size();
		if (size.width > 0 && size.height>0 && bsize.width > 0 && bsize.height > 0) {
			SetValue(value_);
			layout_ = false;
			set_process(false);
		}
	}
	if (p_what == NOTIFICATION_MOUSE_ENTER) {
		if (clipY_ > 1) {
			index_ = 1;
			update();
		}
	}
	else if (p_what == NOTIFICATION_MOUSE_EXIT) {
		if (clipY_ > 1) {
			index_ = 0;
			update();
		}
	}
}

void UI_Slider::_bind_methods() {
	ClassDB::bind_method(D_METHOD("GetValue"), &UI_Slider::GetValue);
	ClassDB::bind_method(D_METHOD("SetValue"), &UI_Slider::SetValue);
}



