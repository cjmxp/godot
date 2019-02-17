#include "ui_button.h"
#include "core/os/os.h"

UI_Button::UI_Button() {
	set_focus_mode(FOCUS_ALL);
}
UI_Button::~UI_Button() {
}


void UI_Button::InitAttribute(Ref<XMLNode> node,ScriptInstance* self) {
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
void UI_Button::_gui_input(Ref<InputEvent> p_event) {

	/*if (status.disabled) // no interaction with disabled button
		return;*/
	Ref<InputEventMouseButton> b = p_event;
	if (b.is_valid()) {
		printf("%d \n", b->is_pressed());
	}
	Ref<InputEventMouseMotion> mm = p_event;

	if (mm.is_valid()) {
		
	}
}
void UI_Button::_notification(int p_what) {
	if (p_what == NOTIFICATION_MOUSE_ENTER) {
		printf("%s \n", "hovering = true");
		//status.hovering = true;
		//update();
	}
	else if (p_what == NOTIFICATION_MOUSE_EXIT) {
		//status.hovering = false;
		printf("%s \n", "hovering = false");
		//update();
	}
	/*Clip::_notification(p_what);
	if (p_what == NOTIFICATION_DRAW) {

		RID ci = get_canvas_item();
		Size2 size = get_size();
		Color color;
		Color color_icon(1, 1, 1, 1);

		Ref<StyleBox> style = get_stylebox("normal");

		switch (get_draw_mode()) {

		case DRAW_NORMAL: {

			style = get_stylebox("normal");
			if (!flat)
				style->draw(ci, Rect2(Point2(0, 0), size));
			color = get_color("font_color");
			if (has_color("icon_color_normal"))
				color_icon = get_color("icon_color_normal");
		} break;
		case DRAW_HOVER_PRESSED: {
			if (has_stylebox("hover_pressed") && has_stylebox_override("hover_pressed")) {
				style = get_stylebox("hover_pressed");
				if (!flat)
					style->draw(ci, Rect2(Point2(0, 0), size));
				if (has_color("font_color_hover_pressed"))
					color = get_color("font_color_hover_pressed");
				else
					color = get_color("font_color");
				if (has_color("icon_color_hover_pressed"))
					color_icon = get_color("icon_color_hover_pressed");

				break;
			}
		}
		case DRAW_PRESSED: {

			style = get_stylebox("pressed");
			if (!flat)
				style->draw(ci, Rect2(Point2(0, 0), size));
			if (has_color("font_color_pressed"))
				color = get_color("font_color_pressed");
			else
				color = get_color("font_color");
			if (has_color("icon_color_pressed"))
				color_icon = get_color("icon_color_pressed");

		} break;
		case DRAW_HOVER: {

			style = get_stylebox("hover");
			if (!flat)
				style->draw(ci, Rect2(Point2(0, 0), size));
			color = get_color("font_color_hover");
			if (has_color("icon_color_hover"))
				color_icon = get_color("icon_color_hover");

		} break;
		case DRAW_DISABLED: {

			style = get_stylebox("disabled");
			if (!flat)
				style->draw(ci, Rect2(Point2(0, 0), size));
			color = get_color("font_color_disabled");
			if (has_color("icon_color_disabled"))
				color_icon = get_color("icon_color_disabled");

		} break;
		}

		if (has_focus()) {

			Ref<StyleBox> style = get_stylebox("focus");
			style->draw(ci, Rect2(Point2(), size));
		}

		Ref<Font> font = get_font("font");
		

		Point2 icon_ofs = (!_icon.is_null()) ? Point2(_icon->get_width() + get_constant("hseparation"), 0) : Point2();
		int text_clip = size.width - style->get_minimum_size().width - icon_ofs.width;
		Point2 text_ofs = (size - style->get_minimum_size() - icon_ofs - font->get_string_size(xl_text) - Point2(_internal_margin[MARGIN_RIGHT] - _internal_margin[MARGIN_LEFT], 0)) / 2.0;

		switch (align) {
		case ALIGN_LEFT: {
			text_ofs.x = style->get_margin(MARGIN_LEFT) + icon_ofs.x + _internal_margin[MARGIN_LEFT] + get_constant("hseparation");
			text_ofs.y += style->get_offset().y;
		} break;
		case ALIGN_CENTER: {
			if (text_ofs.x < 0)
				text_ofs.x = 0;
			text_ofs += icon_ofs;
			text_ofs += style->get_offset();
		} break;
		case ALIGN_RIGHT: {
			if (_internal_margin[MARGIN_RIGHT] > 0) {
				text_ofs.x = size.x - style->get_margin(MARGIN_RIGHT) - font->get_string_size(xl_text).x - _internal_margin[MARGIN_RIGHT] - get_constant("hseparation");
			}
			else {
				text_ofs.x = size.x - style->get_margin(MARGIN_RIGHT) - font->get_string_size(xl_text).x;
			}
			text_ofs.y += style->get_offset().y;
		} break;
		}

		text_ofs.y += font->get_ascent();
		font->draw(ci, text_ofs.floor(), xl_text, color, clip_text ? text_clip : -1);
	}*/
}

void UI_Button::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_gui_input"), &UI_Button::_gui_input);
}



