
#ifndef MORN_H
#define MORN_H
#include "mres.h"
#include "mui.h"
#include "scene/resources/texture.h"
#include "scene/main/node.h"
#include "core/io/http_client.h"
class Morn : public Node {
	GDCLASS(Morn, Node);
public:
	Morn();
	~Morn();
	static Morn *get_singleton();
	void Init(const Variant& m);
	Ref<Texture> GetSkin(const String& skin);
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
	String root_{ "" };
private:
	String url{ "" };
	String request_string{ "" };
	String request_data{ "" };
	int port{ 80 };
	int body_len{ -1 };
	volatile int downloaded{ 0 };
	int redirections{ 0 };
	int response_code{ 0 };
	int max_redirects{ 8 };
	bool use_ssl{ false };
	bool request_sent{ false };
	bool got_response{ false };
	Ref<MUI> mui_{ NULL };

	Vector<String> headers;
	PoolVector<String> response_headers;
	PoolByteArray body;
	Mutex* mutex_;
	Thread* thread{ NULL };
	static Morn* singleton;
	bool exit_{ false };
	Vector<String> n_;
	Vector<Ref<MRes>> list_;
	Map<String, Ref<MRes>> res_;
	Ref<HTTPClient> client_;
	Node* main_{NULL};
	static void _thread_func(void *ud);
	void _thread();
	Ref<MRes> find(const String& v);
	Ref<MRes> pop();
	void push(Ref<MRes> res);

};
#endif
