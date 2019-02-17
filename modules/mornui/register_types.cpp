#include "register_types.h"
#include "core/class_db.h"
#include "ui_box.h"
#include "ui_clip.h"
#include "ui_button.h"
void register_mornui_types() {
	ClassDB::register_class<UI_Box>();
	ClassDB::register_class<UI_Clip>();
	ClassDB::register_class<UI_Button>();
}

void unregister_mornui_types() {
	
}
