#include "ui_box.h"
#include "ui_clip.h"
#include "ui_button.h"
#include "ui_checkbox.h"
UI_Box::UI_Box() {

	set_mouse_filter(Control::MOUSE_FILTER_STOP);

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
			self->set(attribute->value(), Variant(this));
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
		}
		else if (tag == "height") {
			Size2 size = get_size();
			size.height = attribute->value().to_float();
			set_size(size);
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
}
void UI_Box::MouseEnabled(bool v) {
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
		else if (tag == "checkbox") {
			UI_CheckBox* element = memnew(UI_CheckBox);
			element->SetXml(child, self);
			add_child(element);
		}
		
	}
}

void UI_Box::_bind_methods() {
	ClassDB::bind_method(D_METHOD("Xml"), &UI_Box::Xml);
	ClassDB::bind_method(D_METHOD("_gui_input"), &UI_Box::_gui_input);
}



