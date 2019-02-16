#include "box.h"
#include "clip.h"

Box::Box() {
}
Box::~Box() {
}

void Box::Xml(const Variant& self, const String &v) {
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
void Box::SetXml(Ref<XMLNode> node,ScriptInstance* self) {
	InitAttribute(node, self);
	InitChilds(node, self);
}

void Box::InitAttribute(Ref<XMLNode> node,ScriptInstance* self) {
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
			set_mouse_filter(MouseFilter::MOUSE_FILTER_STOP);
		}
		else if (tag == "mousechildren") {
			set_mouse_filter(MouseFilter::MOUSE_FILTER_PASS);
		}
	}
	
}

void Box::InitChilds(Ref<XMLNode> node, ScriptInstance* self) {
	Vector<Variant> array = node->get_children();
	for (unsigned i = 0; i < array.size(); i++) {
		Ref<XMLNode> child = array[i];
		String tag = child->name().to_lower();
		if (tag == "clip") {
			Clip* element = memnew(Clip);
			element->SetXml(child, self);
			add_child(element);
		}
	}
}

void Box::_bind_methods() {
	ClassDB::bind_method(D_METHOD("Xml"), &Box::Xml);
}



