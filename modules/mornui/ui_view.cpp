#include "ui_view.h"

UI_View::UI_View() {

	set_mouse_filter(Control::MOUSE_FILTER_IGNORE);

}
UI_View::~UI_View() {
}

void UI_View::OnEvent(Ref<InputEvent> e) {
	if (get_script_instance()->has_method("_onEvent")) {
		get_script_instance()->call("_onEvent", e);
	}
}

void UI_View::_bind_methods() {

}



