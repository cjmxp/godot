#include "mres.h"

MRes::MRes() {
}
MRes::~MRes() {
	
}
bool MRes::exists() {
	return false;
}
void MRes::SetPath(const String& v) {
	if (path_ == "" && v != "") {
		path_ = v;
	}
}
void MRes::SetKey(const String& v) {
	key_ = v;
}

