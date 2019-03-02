
#ifndef MRES_H
#define MRES_H
#include "core/object.h"
#include "scene/resources/texture.h"

class MRes : public Reference {
	GDCLASS(MRes, Reference);
public:
	MRes();
	~MRes();
	virtual void Load() {};
	virtual void Write() {};
	String GetPath() { return path_; };
	void SetPath(const String& v);
	void SetKey(const String& v);
	bool exists();
protected:
	String path_{ "" };
	String key_{ "" };
};
#endif
