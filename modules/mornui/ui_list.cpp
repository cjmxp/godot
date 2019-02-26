#include "ui_list.h"

#include "ui_tab.h"
#include "ui_clip.h"
#include "ui_button.h"
#include "ui_checkbox.h"
#include "ui_label.h"
#include "ui_textinput.h"
#include "ui_slider.h"
#include "ui_scrollbar.h"

UI_List::UI_List() {

	set_mouse_filter(Control::MOUSE_FILTER_IGNORE);
	content_ = memnew(UI_Box);
	content_->set_clip_contents(true);
	content_->set_mouse_filter(MOUSE_FILTER_IGNORE);
	add_child(content_);
	box_ = memnew(UI_Box);
	box_->set_name("ui_list_box");
	box_->set_mouse_filter(MOUSE_FILTER_IGNORE);
	content_->add_child(box_);
	
}

UI_List::~UI_List() {
}

void UI_List::InitAttribute(Ref<XMLNode> node,ScriptInstance* self) {
	UI_Box::InitAttribute(node, self);
	Vector<Variant> array = node->get_attributes();
	for(unsigned i = 0;i<array.size();i++){
		Ref<XMLAttribute> attribute = array[i];
		String tag = attribute->name().to_lower();
		if (tag == "repeatx") {
			SetRepeatX(attribute->value().to_int());
		}
		else if (tag == "repeaty") {
			SetRepeatY(attribute->value().to_int());
		}
		else if (tag == "spacex") {
			SetSpaceX(attribute->value().to_int());
		}
		else if (tag == "spacey") {
			SetSpaceY(attribute->value().to_int());
		}
		/*
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
		*/
	}
}

void UI_List::InitChilds(Ref<XMLNode> node, ScriptInstance* self) {
	Vector<Variant> array = node->get_children();
	for (unsigned i = 0; i < array.size(); i++) {
		Ref<XMLNode> child = array[i];
		Ref<XMLAttribute> attribute = child->attribute("name");
		if (attribute->value() == "render") {
			render_ = child;
		}
		else {
			String tag = child->name().to_lower();
			if (tag == "box") {
				UI_Box* element = memnew(UI_Box);
				element->SetXml(child, self);
				add_child(element);
			}
			else if (tag == "tab") {
				UI_Tab* element = memnew(UI_Tab);
				element->SetXml(child, self);
				add_child(element);
			}
			else if (tag == "list") {
				UI_List* element = memnew(UI_List);
				element->SetXml(child, self);
				add_child(element);
			}
			else if (tag == "clip") {
				UI_Clip* element = memnew(UI_Clip);
				element->SetXml(child, self);
				add_child(element);
			}
			else if (tag == "button") {
				UI_Button* element = memnew(UI_Button);
				element->SetXml(child, self);
				add_child(element);
			}
			else if (tag == "checkbox" || tag == "radiobutton") {
				UI_CheckBox* element = memnew(UI_CheckBox);
				element->SetXml(child, self);
				add_child(element);
			}
			else if (tag == "label") {
				UI_Label* element = memnew(UI_Label);
				element->SetXml(child, self);
				add_child(element);
			}
			else if (tag == "textinput") {
				UI_TextInput* element = memnew(UI_TextInput);
				element->SetXml(child, self);
				add_child(element);
			}
			else if (tag == "hslider") {
				UI_Slider* element = memnew(UI_Slider);
				element->SetDirection("horizontal");
				element->SetXml(child, self);
				add_child(element);
			}
			else if (tag == "vslider") {
				UI_Slider* element = memnew(UI_Slider);
				element->SetDirection("vertical");
				element->SetXml(child, self);
				add_child(element);
			}
			else if (tag == "hscrollbar") {
				UI_ScrollBar* element = memnew(UI_ScrollBar);
				element->SetDirection("horizontal");
				element->SetXml(child, self);
				add_child(element);
			}
			else if (tag == "vscrollbar") {
				UI_ScrollBar* element = memnew(UI_ScrollBar);
				element->SetDirection("vertical");
				element->SetXml(child, self);
				add_child(element);
			}
		}
	}
}

void UI_List::SetSelectedIndex(int v) {
	if (selectedindex_ != v && v >= 0) {

	}
	/*if (selectedindex_ != v && v >= 0) {
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
	}*/
}

void UI_List::_notification(int p_what) {
	if (setattribute_) {
		content_->set_size(get_size());
		unsigned count = box_->get_child_count();
		UI_Box* node = nullptr;
		for (unsigned i = 0; i < count; i++) {
			node = Object::cast_to<UI_Box>(box_->get_child(i));
			if (node) {
				node->set_visible(false);
			}
		}
		int len = data_.size();
		for (unsigned i = 0; i < len; i++) {
			if (box_->get_child_count() <= i) {
				node = memnew(UI_Box);
				node->SetXml(render_, nullptr);
				node->set_name("ui_list_item_" + itos(i));
				box_->add_child(node);
			}
			else {
				node = Object::cast_to<UI_Button>(box_->get_child(i));
			}
			node->set_visible(true);
			node->SetDataSource(data_[i]);
		}
		setattribute_ = false;
		return;
	}
	if (layout_) {
		int count = box_->get_child_count();
		int rx = 0;
		int ry = 0;
		real_t x = 0;
		real_t y = 0;
		UI_Box* node = nullptr;
		Size2 size;
		for (unsigned i = 0; i < count; i++) {
			node = Object::cast_to<UI_Box>(box_->get_child(i));
			size = node->get_size();
			rx = i % repeatx_;
			ry = i / repeatx_;
			x = size.width * rx + rx * spacex_;
			y = size.height * ry + ry * spacey_;
			node->set_position(Point2(x, y));
		}
		layout_ = false;
		set_process(false);
	}
}

void UI_List::OnEvent(Ref<InputEvent> p_event) {
/*
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
	if (p)p->OnEvent(p_event);*/
}

void UI_List::_gui_input(Ref<InputEvent> p_event) {
	p_event->SetName(get_name());
	OnEvent(p_event);
}


void UI_List::SetSpaceX(int v) {
	if (spacex_ != v) {
		spacex_ = v;
		layout_ = true;
		set_process(true);
	}
}

void UI_List::SetSpaceY(int v) {
	if (spacey_ != v) {
		spacey_ = v;
		layout_ = true;
		set_process(true);
	}
}

void UI_List::SetRepeatX(int v) {
	if (repeatx_ != v) {
		repeatx_ = v;
		setattribute_ = true;
	}
}

void UI_List::SetRepeatY(int v) {
	if (repeaty_ != v) {
		repeaty_ = v;
		setattribute_ = true;
	}
}

void UI_List::SetData(const Variant& arr) {
	if (arr.is_array()) {
		data_ = arr;
		setattribute_ = true;
		layout_ = true;
		set_process(true);
	}
}

void UI_List::_bind_methods() {
	ClassDB::bind_method(D_METHOD("SetData"), &UI_List::SetData);
}



