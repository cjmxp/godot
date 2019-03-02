#include "morn.h"
#include "mui.h"
#include "core/os/file_access.h"

_Morn* _Morn::singleton = nullptr;
_Morn* _Morn::get_singleton() {
	if (singleton == nullptr) {
		singleton = memnew(_Morn);
	}
	return singleton;
}

_Morn::_Morn() {
	mutex_ = Mutex::create();
	thread = Thread::create(_thread_func, this);
}
_Morn::~_Morn() {
	
}

MRes* _Morn::LoadRes(MRes* res) {
	if (find(res->GetPath()) == nullptr) {
		push(res);
	}
}

Ref<Texture> _Morn::GetTexture(const String& skin) {
	Ref<Texture> texture = nullptr;

}

MRes* _Morn::GetRes(const String& v) {
	return find(v);
}

void _Morn::_thread_func(void *ud) {
	_Morn *erp = (_Morn *)ud;
	erp->_thread();
}

void _Morn::_thread() {
	while (true) {
		MRes* res = pop();
		if (res == nullptr) {
			continue;
		}
		if (res->exists()) {
			res->Load();
		}
		else {

		}
	}
}

MRes* _Morn::find(const String& v) {
	MRes* res = nullptr;
	mutex_->lock();
	for (unsigned i = 0; i < list_.size(); i++) {
		if (list_[i]->GetPath() == v) {
			res = list_[i];
			break;
		}
	}
	if (res == nullptr) {
		if (res_.has(v)) {
			res = res_.find(v)->get();
		}
	}
	mutex_->unlock();
	return res;
}

MRes* _Morn::pop() {
	MRes* res = nullptr;
	mutex_->lock();
	if (list_.size() > 0) {
		res = list_[0];
		list_.remove(0);
	}
	mutex_->unlock();
	return res;
}

void _Morn::push(MRes* res) {
	if (res != nullptr) {
		mutex_->lock();
		list_.push_back(res);
		mutex_->unlock();
	}
}

void _Morn::_bind_methods() {
	ClassDB::bind_method(D_METHOD("LoadRes"), &_Morn::LoadRes);
	ClassDB::bind_method(D_METHOD("GetRes"), &_Morn::GetRes);
}
