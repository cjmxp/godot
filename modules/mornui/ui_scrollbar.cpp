#include "ui_scrollbar.h"

UI_ScrollBar::UI_ScrollBar() {
	up_= NULL;
	slider_= NULL;
	down_= NULL;
	grid_= "";
	skin_= "";
	clipX_= 1;
	clipY_= 3;
	hv_= true;
	value_= 0.0;
	set_mouse_filter(Control::MOUSE_FILTER_STOP);
	slider_ = memnew(UI_Slider);
	slider_->set_name("ui_scrollbar_slider");
	add_child(slider_);
	up_ = memnew(UI_Button);
	up_->set_name("ui_scrollbar_up");
	up_->SetClipY(3);
	add_child(up_);
    down_ = memnew(UI_Button);
	down_->SetClipY(3);
	down_->set_name("ui_scrollbar_down");
	add_child(down_);
}
UI_ScrollBar::~UI_ScrollBar() {
}

void UI_ScrollBar::InitAttribute(Ref<XMLNode> node,ScriptInstance* self) {
	UI_Box::InitAttribute(node, self);
	Vector<Variant> array = node->get_attributes();
	for(unsigned i = 0;i<array.size();i++){
		Ref<XMLAttribute> attribute = array[i];
		String tag = attribute->name().to_lower();
		if (tag == "skin") {
			SetSkin(attribute->value());
		}
		else if (tag == "sizegrid") {
			SetSizeGrid(attribute->value());
		}
		else if (tag == "clipx") {
			SetClipX(attribute->value().to_int());
		}
		else if (tag == "clipy") {
			SetClipY(attribute->value().to_int());
		}
		else if (tag == "direction") {
			SetDirection(attribute->value());
		}
		else if (tag == "value") {
			SetValue(attribute->value().to_float());
		}
	}
	setattribute_ = true;
}

void UI_ScrollBar::_notification(int p_what) {

	if (setattribute_){
		slider_->SetSkin(skin_);
		slider_->SetClipX(clipX_);
		slider_->SetClipY(clipY_);
		slider_->SetSizeGrid(grid_);
		slider_->SetDirection(hv_ ? "horizontal" : "vertical");
		slider_->SetValue(value_);
		up_->SetSkin(skin_.replace_first(".png", "$up.png"));
		down_->SetSkin(skin_.replace_first(".png", "$down.png"));
		setattribute_ = false;
		if(!layout_)set_process(false);
		return;
	}
	if (layout_) {
		Size2 size = up_->get_size();
		Size2 size2 = size;
		if (size.width <= 0 || size.height <= 0)return;
		size = down_->get_size();
		if (size.width <= 0 || size.height <= 0)return;
		size = slider_->get_size();
		if (size.width <= 0 || size.height <= 0)return;
		size = get_size();
		if (hv_) {
			size2 = up_->get_size();
			up_->set_position(Point2(0, (size.height-size2.height)/2));
			size2 = down_->get_size();
			down_->set_position(Point2(size.width- size2.width, (size.height - size2.height) / 2));
			size2 = up_->get_size();
			slider_->set_position(Point2(size2.width, 0));
			slider_->set_size(Size2(size.width - (up_->get_size().width + down_->get_size().width), size.height));
			slider_->SetValue(value_);
		}
		else {
			size2 = up_->get_size();
			up_->set_position(Point2((size.width - size2.width) / 2, 0));
			size2 = down_->get_size();
			down_->set_position(Point2((size.width - size2.width) / 2, size.height - size2.height));
			size2 = up_->get_size();
			slider_->set_position(Point2(0, size2.height));
			slider_->set_size(Size2( size.width, size.height - (up_->get_size().height + down_->get_size().height)));
			slider_->SetValue(value_);
		}
		layout_ = false;
		set_process(false);
	}
}

void UI_ScrollBar::OnEvent(Ref<InputEvent> p_event) {
	String name = p_event->GetName();
	UI_Box* p = Parent();
	if (name=="ui_scrollbar_slider" || name=="ui_scrollbar_up" || name == "ui_scrollbar_down") {
		value_ = slider_->GetValue();
		String key = get_name();
		if (name == "ui_scrollbar_up") {
			
			p_event->SetName(key + "_up");
		}
		else if(name == "ui_scrollbar_down")
		{
			p_event->SetName(key + "_down");
		}
		else if(name == "ui_scrollbar_slider") {
			p_event->SetName(key);
		}
		if (p)p->OnEvent(p_event);
		return;
	}
	if (p)p->OnEvent(p_event);
}

void UI_ScrollBar::_gui_input(Ref<InputEvent> p_event) {
	p_event->SetName(get_name());
	OnEvent(p_event);
}

String UI_ScrollBar::GetDirection() {
	if (hv_) {
		return "horizontal";
	}
	return "vertical";
}

void UI_ScrollBar::SetDirection(const String& v) {
	if (v == "horizontal") {
		hv_ = true;
	}
	else {
		hv_ = false;
	}
	layout_ = true;
	set_process(true);
}

void UI_ScrollBar::SetSizeGrid(const String& v) {
	if (grid_ != v) {
		grid_ = v;
		setattribute_ = true;
	}
}

void UI_ScrollBar::SetSkin(const String& v) {
	if (skin_ != v) {
		skin_ = v;
		setattribute_ = true;
		layout_ = true;
		set_process(true);
	}
}

void UI_ScrollBar::SetClipX(int v) {
	if (clipX_ != v) {
		clipX_ = v;
		setattribute_ = true;
	}
}

void UI_ScrollBar::SetClipY(int v) {
	if (clipY_ != v) {
		clipY_ = v;
		setattribute_ = true;
	}
}

void UI_ScrollBar::SetValue(float v) {
	if (v < 0.0)v = 0.0;
	if (v > 1.0)v = 1.0;
	if (value_ != v) {
		value_ = v;
		setattribute_ = true;
		set_process(true);
	}
}

void UI_ScrollBar::_bind_methods() {
	ClassDB::bind_method(D_METHOD("GetSkin"), &UI_ScrollBar::GetSkin);
	ClassDB::bind_method(D_METHOD("SetSkin"), &UI_ScrollBar::SetSkin);
	ClassDB::bind_method(D_METHOD("GetSizeGrid"), &UI_ScrollBar::GetSizeGrid);
	ClassDB::bind_method(D_METHOD("SetSizeGrid"), &UI_ScrollBar::SetSizeGrid);
	ClassDB::bind_method(D_METHOD("GetClipX"), &UI_ScrollBar::GetClipX);
	ClassDB::bind_method(D_METHOD("SetClipX"), &UI_ScrollBar::SetClipX);
	ClassDB::bind_method(D_METHOD("GetClipY"), &UI_ScrollBar::GetClipY);
	ClassDB::bind_method(D_METHOD("SetClipY"), &UI_ScrollBar::SetClipY);
	ClassDB::bind_method(D_METHOD("GetDirection"), &UI_ScrollBar::GetDirection);
	ClassDB::bind_method(D_METHOD("SetDirection"), &UI_ScrollBar::SetDirection);
	ClassDB::bind_method(D_METHOD("GetValue"), &UI_ScrollBar::GetValue);
	ClassDB::bind_method(D_METHOD("SetValue"), &UI_ScrollBar::SetValue);
	

}



