#include "mui.h"
#include "morn.h"
#include "core/io/file_access_encrypted.h"
#include "core/io/file_access_memory.h"
#include "core/io/image_loader.h"

MUI::MUI() {
}
MUI::~MUI() {
	//map_.clear();
	//imgs_.clear();
}

bool MUI::Load() {
	if (MRes::Load()) {
		Error err;
		FileAccess *file = FileAccess::open("user://" + path_, FileAccess::READ, &err);
		if (key_!="") {
			FileAccessEncrypted *fae = memnew(FileAccessEncrypted);
			err = fae->open_and_parse_password(file, key_, FileAccessEncrypted::MODE_READ);
			if (err) {
				memdelete(fae);
				if(file)memdelete(file);
				file = NULL;
				return false;
			}
			else {
				file = fae;
			}
		}
		if (file) {
			PoolByteArray temp;
			FileAccessMemory* buff = memnew(FileAccessMemory);
			uint16_t count = 0;
			uint32_t size = 0;
			String key = "";
			uint32_t id = 0;
			Rect2 region;

			count = file->get_16();
			for (unsigned i = 0; i < count; i++) {
				Ref <MAtlasTexture> texture = memnew(MAtlasTexture);
				texture->i = file->get_8();
				texture->key = file->get_line();
				id = texture->key.hash();
				region.position.x = file->get_16();
				region.position.y = file->get_16();
				region.size.x = file->get_16();
				region.size.y = file->get_16();
				texture->set_region(region);
				map_.insert(id, texture);
			}

			count = file->get_8();
			for (unsigned i = 0; i < count; i++) {
				key = file->get_line();
				size = file->get_32();
				temp.resize(size);
				PoolByteArray::Write w = temp.write();
				file->get_buffer(w.ptr(), size);
				PoolByteArray::Read r = temp.read();
				buff->open_custom(r.ptr(), size);
				Ref<Image> image = memnew(Image);
				buff->seek(0);
				if (ImageLoader::load_image("img."+ key, image, buff) != OK) {
					break;
				}
				imgs_.push_back(image);
			}
			
			buff->close();
			file->close();
			memdelete(file);
			file = NULL;
			buff = NULL;
			return true;
		}
	}
	return false;
}

bool MUI::Fill(PoolByteArray db) {
	MRes::Fill(db);
	return Load();
}

void MUI::Init() {
	for (Map<uint32_t, Ref<MAtlasTexture>>::Element *E = map_.front(); E; E = E->next()) {
		if (imgs_.size() > E->value()->i) {
			Ref<ImageTexture> texture = memnew(ImageTexture);
			texture->create_from_image(imgs_[E->value()->i], Texture::FLAG_REPEAT);
			E->value()->set_atlas(texture);
		}
		else {
			return;
		}
	}
	ready = true;
}

Ref<Texture> MUI::GetSkin(const String& v) {
	if (!ready)return NULL;
	Map<uint32_t, Ref<MAtlasTexture>>::Element* E = map_.find(v.hash());
	if (E != NULL) {
		return E->value();
	}
	return NULL;
}

