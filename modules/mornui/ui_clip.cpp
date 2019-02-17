#include "ui_clip.h"
#include "core/os/os.h"

UI_Clip::UI_Clip() {
	margin_[0] = 0;
	margin_[1] = 0;
	margin_[2] = 0;
	margin_[3] = 0;
	set_mouse_filter(Control::MOUSE_FILTER_STOP);
}
UI_Clip::~UI_Clip() {
}


void UI_Clip::InitAttribute(Ref<XMLNode> node,ScriptInstance* self) {
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
	}
}

void UI_Clip::SetSkin(const String& v) {
	if (v != "" && skin_ != v) {
		skin_ = v;
		//Ref<AtlasTexture> t = memnew(AtlasTexture);
		//t->set_atlas(ResourceLoader::load(v));
		//t->set_region(Rect2(0,0,64,64));
		texture_ = ResourceLoader::load(v);//t;
		update();
		minimum_size_changed();
		emit_signal("texture_changed");
		_change_notify("texture");
	}
}
void UI_Clip::SetInterval(int v) {
	if (interval_!=v && v>0) {
		interval_ = v;
	}
}

void UI_Clip::SetSizeGrid(const String& v) {
	if (grid_ != v && v != "") {
		grid_ = v;
		Vector<String> list = grid_.split(",");
		int len = list.size();
		if (len > 4) {
			len = 4;
		}
		for (unsigned i = 0; i < len; i++) {
			margin_[i] = list[i].to_int();
		}

	}
}

void UI_Clip::SetIndex(int v) {
	if (index_ != v) {
		index_ = v;
		update();
	}
}

void UI_Clip::SetClipX(int v) {
	if (clipX_ != v) {
		clipX_ = v;
	}
}

void UI_Clip::SetClipY(int v) {
	if (clipY_ != v) {
		clipY_ = v;
	}
}

void UI_Clip::SetAutoPlay(bool v) {
	if (autoplay_ != v) {
		autoplay_ = v;
		set_process(v);
	}
}

void UI_Clip::_notification(int p_what) {
	if (p_what == NOTIFICATION_PROCESS && autoplay_ && interval_>0) {
		uint32_t v = OS::get_singleton()->get_ticks_msec();
		if (ctimer_ == 0) {
			ctimer_ = v;
		}
		else if (v - ctimer_>= interval_){
			ctimer_ = v;
			SetIndex(index_ + 1);
		}
	}
	else if (p_what == NOTIFICATION_DRAW) {

		if (texture_.is_null())
			return;
		
		Rect2 rect = Rect2(Point2(), get_size());
		Rect2 src_rect;
		texture_->get_rect_region(rect, src_rect, rect, src_rect);
		if (index_ < 0) {
			index_ = 0;
			
		}
		else if (index_ >= clipX_ * clipY_)
		{
			index_ = 0;
		}
		
		int w = texture_->get_width() / clipX_;
		int h = texture_->get_height() / clipY_;
		int y = index_ / clipX_;
		int x = index_ % clipX_;

		src_rect.position.x += (x * w);
		src_rect.position.y += (y * h);
		src_rect.size.x = w;
		src_rect.size.y = h;
		
		RID ci = get_canvas_item();
		VS::get_singleton()->canvas_item_add_nine_patch(ci, rect, src_rect, texture_->get_rid(), Vector2(margin_[0], margin_[1]), Vector2(margin_[2], margin_[3]), VS::NinePatchAxisMode(axis_h_), VS::NinePatchAxisMode(axis_v_), true);
	}
}

//void Clip::_bind_methods() {
	//ClassDB::bind_method(D_METHOD("Xml"), &Box::Xml);
//}



