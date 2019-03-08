
#ifndef MRES_H
#define MRES_H
#include "core/object.h"
#include "core/reference.h"
#include "core/os/file_access.h"

class MRes : public Reference {
	GDCLASS(MRes, Reference);
public:
	MRes();
	~MRes();
	virtual bool Load();
	virtual bool Fill(PoolByteArray db);
	virtual void Init();
	String GetUrl(const String& root) { return root+"/"+ path_; };
	String GetPath() { return path_; };
	void SetPath(const String& v);
	void SetKey(const String& v);
protected:
	String path_;
	String key_;
	bool ready;
	static void _bind_methods();
};
#endif
