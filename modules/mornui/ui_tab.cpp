#include "ui_tab.h"
#include "ui_button.h"

UI_Tab::UI_Tab() {

	set_mouse_filter(Control::MOUSE_FILTER_STOP);

}
UI_Tab::~UI_Tab() {
}

void UI_Tab::InitAttribute(Ref<XMLNode> node,ScriptInstance* self) {
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
		else if (tag == "labels") {
			SetLabels(attribute->value());
		}
		else if (tag == "selectedindex") {
			SetSelectedIndex(attribute->value().to_int());
		}
		else if (tag == "direction") {
			SetDirection(attribute->value());
		}
		else if (tag == "space") {
			SetSpace(attribute->value().to_int());
		}
	}
	setattribute_ = true;
}
void UI_Tab::SetLabels(const String& v) {
	if (labels_ != v) {
		labels_ = v;
		setattribute_ = true;
		layout_ = true;
		set_process(true);
	}
}
void UI_Tab::SetSizeGrid(const String& v) {
	if (grid_ != v) {
		grid_ = v;
		setattribute_ = true;
	}
}

void UI_Tab::SetSkin(const String& v) {
	if (skin_ != v) {
		skin_ = v;
		setattribute_ = true;
	}
}
void UI_Tab::SetClipX(int v) {
	if (clipX_ != v) {
		clipX_ = v;
		setattribute_ = true;
	}
}
void UI_Tab::SetClipY(int v) {
	if (clipY_ != v) {
		clipY_ = v;
		setattribute_ = true;
	}
}
void UI_Tab::SetSelectedIndex(int v) {
	if (selectedindex_ != v && v >= 0) {
		UI_Button* button = nullptr;
		unsigned count = get_child_count();
		if (count > 0) {
			if (v >= count) v = count - 1;
			if (selectedindex_ !=-1) {
				button = Object::cast_to<UI_Button>(get_child(selectedindex_));
				button->SetSelected(false);
				selectedindex_ = -1;
			}
			if (selectedindex_ == -1) {
				button = Object::cast_to<UI_Button>(get_child(v));
				button->SetSelected(true);
			}
			selectedindex_ = v;
		}
		else {
			selectedindex_ = v;
			setattribute_ = true;
		}
	}
}
void UI_Tab::_notification(int p_what) {
	if (setattribute_ && labels_!="") {
		setattribute_ = false;
		UI_Button* button = nullptr;
		unsigned count = get_child_count();
		for (unsigned i = 0; i < count; i++) {
			button = Object::cast_to<UI_Button>(get_child(i));
			if (button) {
				button->set_visible(false);
				button->SetSelected(false);
			}
		}
		Vector<String> list = labels_.split(",");
		for (unsigned i = 0; i < list.size(); i++) {
			if (get_child_count() <= i) {
				button = memnew(UI_Button);
				button->set_name("ui_table_item_"+ itos(i));
				button->SetTabMode(true);
				add_child(button);
			}
			else {
				button = Object::cast_to<UI_Button>(get_child(i));
			}
			if (button) {
				button->set_visible(true);
				button->SetLabel(list[i]);
				button->SetClipX(clipX_);
				button->SetClipY(clipY_);
				button->SetSizeGrid(grid_);
				button->SetSkin(skin_);
				if (selectedindex_ == i)button->SetSelected(true);
			}
		}
		return;
	}
	if (layout_) {
		UI_Button* button = nullptr;
		unsigned count = get_child_count();
		real_t width = 0.0f;
		real_t height = 0.0f;
		for (unsigned i = 0; i < count; i++) {
			button = Object::cast_to<UI_Button>(get_child(i));
			Size2 size = button->get_size();
			if (size.width == 0 || size.height == 0)return;
			if (hv_) {
				button->set_position(Point2(width,0.0f));
				height = size.height > height ? size.height : height;
				width += size.width + space_;
			}
			else {
				button->set_position(Point2(0.0f, height));
				width = size.width > width ? size.width : width;
				height += size.height + space_;
			}
		}
		if (hv_) {
			width -= space_;
		}
		else {
			height -= space_;
		}
		minsize_.width = width;
		minsize_.height = height;
		minimum_size_changed();
		layout_ = false;
		set_process(false);
	}
}
void UI_Tab::SetDirection(String v) {
	if (v == "horizontal") {
		hv_ = true;
	}
	else {
		hv_ = false;
	}
	layout_ = true;
}

void UI_Tab::SetSpace(int v) {

}

void UI_Tab::OnEvent(Ref<InputEvent> p_event) {

	String name = p_event->GetName();
	if (name.find("ui_table_item_")!=-1) {
		Ref<InputEventMouseButton> b = p_event;
		if (b.is_valid() && b->is_pressed() ) {
			name = name.replace_first("ui_table_item_", "");
			unsigned j = name.to_int();
			SetSelectedIndex(j);
			UI_Box* p = Parent();
			if (p) {
				p_event->SetName(get_name());
				p->OnEvent(p_event);
			}
			return;
		}
	}
	UI_Box* p = Parent();
	if (p)p->OnEvent(p_event);
}

void UI_Tab::_gui_input(Ref<InputEvent> p_event) {
	p_event->SetName(get_name());
	OnEvent(p_event);
}


void UI_Tab::_bind_methods() {

}



