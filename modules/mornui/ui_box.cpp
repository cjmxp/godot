#include "ui_box.h"
#include "ui_tab.h"
#include "ui_list.h"
#include "ui_clip.h"
#include "ui_button.h"
#include "ui_checkbox.h"
#include "ui_label.h"
#include "ui_textinput.h"
#include "ui_slider.h"
#include "ui_scrollbar.h"
UI_Box::UI_Box() {

	set_mouse_filter(Control::MOUSE_FILTER_IGNORE);

}
UI_Box::~UI_Box() {
}

void UI_Box::Xml(const Variant& self, const String &v) {
	if(dom==nullptr){
		dom.instance();
	}
	if (dom->load_string(v)==OK) {
		Object* obj = self;
		if (obj != nullptr) {
			Ref<XMLNode> node = dom->first_child();
			ScriptInstance* script = obj->get_script_instance();
			InitAttribute(node, script);
			InitChilds(node, script);
		}
	}
}
void UI_Box::SetXml(Ref<XMLNode> node,ScriptInstance* self) {
	InitAttribute(node, self);
	InitChilds(node, self);
}

void UI_Box::InitAttribute(Ref<XMLNode> node,ScriptInstance* self) {
	Vector<Variant> array = node->get_attributes();
	for(unsigned i = 0;i<array.size();i++){
		Ref<XMLAttribute> attribute = array[i];
		String tag = attribute->name().to_lower();
		if (tag == "var") {
			if(self!=nullptr)self->set(attribute->value(), Variant(this));
		}
		else if (tag=="name") {
			set_name(attribute->value());
		}
		else if (tag == "x") {
			Point2 p = get_position();
			p.x = attribute->value().to_float();
			set_position(p);
		}
		else if (tag == "y") {
			Point2 p = get_position();
			p.y = attribute->value().to_float();
			set_position(p);
		}
		else if (tag == "width") {
			Size2 size = get_size();
			size.width = attribute->value().to_float();
			set_size(size);
			setattribute_ = true;
		}
		else if (tag == "height") {
			Size2 size = get_size();
			size.height = attribute->value().to_float();
			set_size(size);
			setattribute_ = true;
		}
		else if (tag == "scalex") {
			Vector2 scale = get_scale();
			scale.x = attribute->value().to_float();
			set_scale(scale);
		}
		else if (tag == "scaley") {
			Vector2 scale = get_scale();
			scale.y = attribute->value().to_float();
			set_scale(scale);
		}
		else if (tag == "rotation") {
			float rotation = attribute->value().to_float();
			set_rotation(rotation);
		}
		else if (tag == "visible") {
			if (attribute->value() == "true") {
				set_visible(true);
			}
			else {
				set_visible(false);
			}
		}
		else if (tag == "mouseenabled") {
			if (attribute->value() == "true") {
				MouseEnabled(true);
			}
			else {
				MouseEnabled(false);
			}
		}
	}
}
void UI_Box::_gui_input(Ref<InputEvent> p_event) {
	p_event->SetName(get_name());
	OnEvent(p_event);
}

Size2 UI_Box::get_minimum_size() const {
	return minsize_;
}

UI_Box* UI_Box::Parent() {
	if (!get_parent())return nullptr;
	return Object::cast_to<UI_Box>(get_parent());
}
void UI_Box::OnEvent(Ref<InputEvent> e) {
	UI_Box* p = Parent();
	if (p)p->OnEvent(e);
}
void UI_Box::MouseEnabled(bool v) {
	setattribute_ = true;
	/*if (v) {
		set_mouse_filter(Control::MOUSE_FILTER_STOP);
	}
	else {
		set_mouse_filter(Control::MOUSE_FILTER_IGNORE);
	}*/
}

void UI_Box::InitChilds(Ref<XMLNode> node, ScriptInstance* self) {
	Vector<Variant> array = node->get_children();
	for (unsigned i = 0; i < array.size(); i++) {
		Ref<XMLNode> child = array[i];
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
		else if (tag  == "button") {
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

void UI_Box::SetDataSource(const Variant& db) {

}

void UI_Box::_bind_methods() {
	ClassDB::bind_method(D_METHOD("Xml"), &UI_Box::Xml);
	ClassDB::bind_method(D_METHOD("_gui_input"), &UI_Box::_gui_input);
}



