#include "clip.h"


Clip::Clip() {
	margin[0] = 0;
	margin[1] = 0;
	margin[2] = 0;
	margin[3] = 0;
	axis_h = AXIS_STRETCH_MODE_STRETCH;
	axis_v = AXIS_STRETCH_MODE_STRETCH;
	draw_center = true;
}
Clip::~Clip() {
}


void Clip::InitAttribute(Ref<XMLNode> node,ScriptInstance* self) {
	Box::InitAttribute(node, self);
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
			SetInterval(attribute->value().to_float()*0.001f);
		}
	}
}

void Clip::SetSkin(const String& v) {
	if (v != "" && skin_ != v) {
		skin_ = v;
		Ref<AtlasTexture> t = memnew(AtlasTexture);
		t->set_atlas(ResourceLoader::load(v));
		t->set_region(Rect2(0,0,64,64));
		texture = t;
		update();
		minimum_size_changed();
		emit_signal("texture_changed");
		_change_notify("texture");
	}
}
void Clip::SetInterval(float v) {
	if (interval_!=v) {
		interval_ = v;
	}
}

void Clip::SetSizeGrid(const String& v) {
	if (grid_ != v && v != "") {
		grid_ = v;
		Vector<String> list = grid_.split(",");
		int len = list.size();
		if (len > 4) {
			len = 4;
		}
		for (unsigned i = 0; i < len; i++) {
			margin[i] = list[i].to_int();
		}

	}
}


void Clip::SetIndex(int v) {
	if (index_ != v) {
		index_ = v;
		update();
	}
}

void Clip::SetClipX(int v) {
	if (clipX_ != v) {
		clipX_ = v;
	}
}

void Clip::SetClipY(int v) {
	if (clipY_ != v) {
		clipY_ = v;
	}
}

void Clip::_notification(int p_what) {

	if (p_what == NOTIFICATION_DRAW) {

		if (texture.is_null())
			return;
		
		Rect2 rect = Rect2(Point2(), get_size());
		Rect2 src_rect;
		texture->get_rect_region(rect, src_rect, rect, src_rect);
		if (index_ < 0) {
			index_ = 0;
			
		}
		else if (index_ >= clipX_ * clipY_)
		{
			index_ = 0;
		}
		
		int w = src_rect.size.x / clipX_;
		int h = src_rect.size.y / clipY_;
		int y = index_ / clipX_;
		int x = index_ % clipX_;

		src_rect.position.x += (x * w);
		src_rect.position.y += (y * h);
		src_rect.size.x = w;
		src_rect.size.y = h;
		
		RID ci = get_canvas_item();
		VS::get_singleton()->canvas_item_add_nine_patch(ci, rect, src_rect, texture->get_rid(), Vector2(margin[0], margin[1]), Vector2(margin[2], margin[3]), VS::NinePatchAxisMode(axis_h), VS::NinePatchAxisMode(axis_v), draw_center);
	}
}

//void Clip::_bind_methods() {
	//ClassDB::bind_method(D_METHOD("Xml"), &Box::Xml);
//}



