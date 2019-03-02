
#ifndef MORN_H
#define MORN_H
#include "core/object.h"
#include "mres.h"
//#include "core/os/thread.h"
#include "scene/resources/texture.h"

class _Morn : public Object {
	GDCLASS(_Morn, Object);
public:
	_Morn();
	~_Morn();
	static _Morn *get_singleton();
	Ref<Texture> GetTexture(const String& skin);
	MRes* GetRes(const String& skin);
	MRes* LoadRes(MRes* res);
protected:
	static void _bind_methods();
private:
	Mutex* mutex_;
	Thread* thread{ nullptr };
	static _Morn* singleton;
	static void _thread_func(void *ud);
	void _thread();
	bool exit_{ false };
	Vector<String> n_;
	Vector<MRes*> list_;
	MRes* find(const String& v);
	MRes* pop();
	void push(MRes* res);
	Map<String, MRes*> res_;
	
};
#endif
