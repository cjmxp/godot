#include "register_types.h"
#include "core/class_db.h"
#include "ui_box.h"
#include "ui_view.h"
#include "ui_tab.h"
#include "ui_list.h"
#include "ui_clip.h"
#include "ui_button.h"
#include "ui_checkbox.h"
#include "ui_label.h"
#include "ui_slider.h"
#include "ui_textinput.h"
#include "ui_scrollbar.h"

void register_mornui_types() {

	ClassDB::register_class<UI_Box>();
	ClassDB::register_class<UI_View>();
	ClassDB::register_class<UI_Tab>();
	ClassDB::register_class<UI_List>();
	ClassDB::register_class<UI_Clip>();
	ClassDB::register_class<UI_Button>();
	ClassDB::register_class<UI_CheckBox>();
	ClassDB::register_class<UI_Label>();
	ClassDB::register_class<UI_Slider>();
	ClassDB::register_class<UI_TextInput>();
	ClassDB::register_class<UI_ScrollBar>();
	
}

void unregister_mornui_types() {
	
}
