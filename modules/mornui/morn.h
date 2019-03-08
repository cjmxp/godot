
#ifndef MORN_H
#define MORN_H
#include "mres.h"
#include "mui.h"
#include "scene/resources/texture.h"
#include "scene/main/node.h"
#include "core/io/http_client.h"
#include "scene/resources/font.h"
//#include "scene/resources/dynamic_font.h"

class Morn : public Node {
	GDCLASS(Morn, Node);
public:
	Morn();
	~Morn();
	StringName MUIClass;
	static Morn *get_singleton();
	void Init(const Variant& m);
	Ref<Texture> GetSkin(const String& skin);
	Ref<Font> GetFont(const String& f,int s);
	Ref<MRes> GetRes(const String& skin);
	void LoadRes(Ref<MRes> res);
	void SetUrl(const String& v);
	String GetUrl() { return root_; };
	PoolByteArray HTTPGet(const String& url);
	void OnComplete(Ref<MRes> res);
	void OnError(const String& v);
protected:
	static void _bind_methods();
	Error _parse_url(const String &p_url);
	Error _request();
	bool _handle_response(bool *ret_value);
	void sleep(int msec);
	String root_;
private:
	String url;
	String request_string;
	String request_data;
	int port;
	int body_len;
	volatile int downloaded;
	int redirections;
	int response_code;
	int max_redirects;
	bool use_ssl;
	bool request_sent;
	bool got_response;
	Ref<MUI> mui_;

	Vector<String> headers;
	PoolVector<String> response_headers;
	PoolByteArray body;
	Mutex* mutex_;
	Thread* thread;
	static Morn* singleton;
	bool exit_;
	Vector<String> n_;
	Vector<Ref<MRes> > list_;

	Map<uint32_t, Ref<MRes> > res_;
	Map<uint32_t, Ref<Font> > fonts_;
	//Map<uint32_t, Ref<DynamicFontData>> ttfs_;

	Ref<HTTPClient> client_;
	Node* main_;
	static void _thread_func(void *ud);
	void _thread();
	Ref<MRes> find(const String& v);
	Ref<MRes> pop();
	void push(Ref<MRes> res);

};
#endif
