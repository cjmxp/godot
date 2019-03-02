#include "mres.h"

MRes::MRes() {
}
MRes::~MRes() {
	
}
bool MRes::Load() {
	if (FileAccess::exists("user://" + path_)) {
		return true;
	}
	return false;
}
bool MRes::Fill(PoolByteArray db) {
	Error err;
	FileAccess *file = FileAccess::open("user://" + path_, FileAccess::WRITE, &err);
	if (err) {
		if (file) {
			memdelete(file);
			file = nullptr;
		}
		return false;
	}
	if (file) {
		PoolByteArray::Read r = db.read();
		file->store_buffer(r.ptr(), db.size());
		file->flush();
		file->close();
		memdelete(file);
		file = nullptr;
		return true;
	}
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

