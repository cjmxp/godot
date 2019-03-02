#include "morn.h"
#include "mui.h"
#include "core/os/os.h"

_Morn* _Morn::singleton = nullptr;
_Morn* _Morn::get_singleton() {
	if (singleton == nullptr) {
		singleton = memnew(_Morn);
	}
	return singleton;
}

_Morn::_Morn() {
	mutex_ = Mutex::create();
	
}
_Morn::~_Morn() {
}

Error _Morn::_parse_url(const String &p_url) {

	url = p_url;
	use_ssl = false;

	request_string = "";
	port = 80;
	request_sent = false;
	got_response = false;
	body_len = -1;
	body.resize(0);
	downloaded = 0;
	redirections = 0;

	String url_lower = url.to_lower();
	if (url_lower.begins_with("http://")) {
		url = url.substr(7, url.length() - 7);
	}
	else if (url_lower.begins_with("https://")) {
		url = url.substr(8, url.length() - 8);
		use_ssl = true;
		port = 443;
	}
	else {
		ERR_EXPLAIN("Malformed URL");
		ERR_FAIL_V(ERR_INVALID_PARAMETER);
	}

	if (url.length() < 1) {
		ERR_EXPLAIN("URL too short");
		ERR_FAIL_V(ERR_INVALID_PARAMETER);
	}

	int slash_pos = url.find("/");

	if (slash_pos != -1) {
		request_string = url.substr(slash_pos, url.length());
		url = url.substr(0, slash_pos);
	}
	else {
		request_string = "/";
	}

	int colon_pos = url.find(":");
	if (colon_pos != -1) {
		port = url.substr(colon_pos + 1, url.length()).to_int();
		url = url.substr(0, colon_pos);
		ERR_FAIL_COND_V(port < 1 || port > 65535, ERR_INVALID_PARAMETER);
	}

	return OK;
}

bool _Morn::_handle_response(bool *ret_value) {

	if (!client_->has_response()) {
		*ret_value = true;
		return 0;
	}

	got_response = true;
	response_code = client_->get_response_code();
	List<String> rheaders;
	client_->get_response_headers(&rheaders);
	response_headers.resize(0);
	downloaded = 0;
	for (List<String>::Element *E = rheaders.front(); E; E = E->next()) {
		response_headers.push_back(E->get());
	}

	if (response_code == 301 || response_code == 302) {
		// Handle redirect

		if (max_redirects >= 0 && redirections >= max_redirects) {

			*ret_value = true;
			return 0;
		}

		String new_request;

		for (List<String>::Element *E = rheaders.front(); E; E = E->next()) {
			if (E->get().findn("Location: ") != -1) {
				new_request = E->get().substr(9, E->get().length()).strip_edges();
			}
		}

		if (new_request != "") {
			// Process redirect
			client_->close();
			int new_redirs = redirections + 1; // Because _request() will clear it
			Error err;
			if (new_request.begins_with("http")) {
				// New url, request all again
				_parse_url(new_request);
			}
			else {
				request_string = new_request;
			}

			err = _request();
			if (err == OK) {
				request_sent = false;
				got_response = false;
				body_len = -1;
				body.resize(0);
				downloaded = 0;
				redirections = new_redirs;
				*ret_value = false;
				return 1;
			}
		}
	}

	return 2;
}
void _Morn::sleep(int msec) {
	OS::get_singleton()->delay_usec(msec * 1000);
}
Error _Morn::_request() {
	return client_->connect_to_host(url, port, use_ssl, false);
}
PoolByteArray _Morn::HTTPGet(const String& url_) {
	response_code = -1;
	_parse_url(url_);
	client_->set_blocking_mode(false);
	Error err = _request();
	if (err == OK) {
		while (true)
		{
			switch (client_->get_status()) {
			case HTTPClient::STATUS_DISCONNECTED: {
				return PoolByteArray();
			} break;
			case HTTPClient::STATUS_RESOLVING: {
				client_->poll();
				// Must wait
				sleep(200);
				continue;
			} break;
			case HTTPClient::STATUS_CANT_RESOLVE: {
				return PoolByteArray();

			} break;
			case HTTPClient::STATUS_CONNECTING: {
				client_->poll();
				// Must wait
				sleep(200);
				continue;
			} break; // Connecting to IP
			case HTTPClient::STATUS_CANT_CONNECT: {
				return PoolByteArray();

			} break;
			case HTTPClient::STATUS_CONNECTED: {

				if (request_sent) {

					if (!got_response) {

						// No body

						bool ret_value;

						int r = _handle_response(&ret_value);
						if (r == 0) {
							return PoolByteArray();
						}
						else if (r == 1) {
							continue;
						}

						return PoolByteArray();
					}
					if (got_response && body_len < 0) {
						return body;
					}

					return PoolByteArray();
					// Request migh have been done
				}
				else {
					// Did not request yet, do request

					Error err = client_->request(HTTPClient::METHOD_GET, request_string, headers, request_data);
					if (err != OK) {
						return PoolByteArray();
					}

					request_sent = true;
					continue;
				}
			} break; // Connected: break requests only accepted here
			case HTTPClient::STATUS_REQUESTING: {
				// Must wait, still requesting
				client_->poll();
				sleep(200);
				continue;

			} break; // Request in progress
			case HTTPClient::STATUS_BODY: {

				if (!got_response) {

					bool ret_value;
					int r = _handle_response(&ret_value);
					if (r == 0) {
						return PoolByteArray();
					}
					else if (r == 1) {
						continue;
					}

					if (!client_->is_response_chunked() && client_->get_response_body_length() == 0) {
						return PoolByteArray();
					}

					// No body len (-1) if chunked or no content-length header was provided.
					// Change your webserver configuration if you want body len.
					body_len = client_->get_response_body_length();

				}

				client_->poll();

				PoolByteArray chunk = client_->read_response_body_chunk();
				downloaded += chunk.size();

				body.append_array(chunk);
				
				if (body_len >= 0) {

					if (downloaded == body_len) {
						return body;
					}
				}
				else if (client_->get_status() == HTTPClient::STATUS_DISCONNECTED) {
					return body;
				}

				continue;

			} break; // Request resulted in body: break which must be read
			case HTTPClient::STATUS_CONNECTION_ERROR: {
				return PoolByteArray();
			} break;
			case HTTPClient::STATUS_SSL_HANDSHAKE_ERROR: {
				return PoolByteArray();
			} break;
			}
		}
	}
}

void _Morn::SetUrl(const String& v) {
	root_ = v;
}

void _Morn::LoadRes(Ref<MRes> res) {
	if (client_.is_null()) {
		client_.instance();
	}
	if (res.is_null())return;
	if (find(res->GetPath()) == nullptr) {
		push(res);
		if (thread == nullptr) {
			thread = Thread::create(_thread_func, this);
		}
	}
}

Ref<Texture> _Morn::GetTexture(const String& skin) {
	Ref<Texture> texture = nullptr;
	return texture;
}

Ref<MRes> _Morn::GetRes(const String& v) {
	return find(v);
}

void _Morn::_thread_func(void *ud) {
	_Morn *erp = (_Morn *)ud;
	erp->_thread();
}

void _Morn::_thread() {
	while (true) {
		Ref<MRes> res = pop();
		if (res.is_null()) {
			sleep(1000);
			continue;
		}
		if (!res->Load()) {
			PoolByteArray db = HTTPGet(res->GetUrl(GetUrl()));
			if (db.size() > 0) {
				res->Fill(db);
			}
		}
	}
}

Ref<MRes> _Morn::find(const String& v) {
	Ref<MRes> res = nullptr;
	mutex_->lock();
	for (unsigned i = 0; i < list_.size(); i++) {
		Ref<MRes> item = list_[i];
		if (item->GetPath() == v) {
			res = list_[i];
			break;
		}
	}
	if (res.is_null()) {
		if (res_.has(v)) {
			res = res_.find(v)->get();
		}
	}
	mutex_->unlock();
	return res;
}

Ref<MRes> _Morn::pop() {
	Ref<MRes> res = nullptr;
	mutex_->lock();
	if (list_.size() > 0) {
		res = list_[0];
		list_.remove(0);
	}
	mutex_->unlock();
	return res;
}

void _Morn::push(Ref<MRes> res) {
	if (res != nullptr) {
		mutex_->lock();
		list_.push_back(res);
		mutex_->unlock();
	}
}

void _Morn::_bind_methods() {
	ClassDB::bind_method(D_METHOD("LoadRes"), &_Morn::LoadRes);
	ClassDB::bind_method(D_METHOD("GetRes"), &_Morn::GetRes);
	ClassDB::bind_method(D_METHOD("GetUrl"), &_Morn::GetUrl);
	ClassDB::bind_method(D_METHOD("SetUrl"), &_Morn::SetUrl);
}
