#include "mui.h"
#include "core/io/file_access_encrypted.h"
#include "core/io/file_access_memory.h"
#include "core/io/image_loader.h"

MUI::MUI() {
}
MUI::~MUI() {
	
}

bool MUI::Load() {
	if (MRes::Load()) {
		FileAccess *file = FileAccess::open("user://" + path_, FileAccess::WRITE);
		if (key_!="") {
			FileAccessEncrypted *fae = memnew(FileAccessEncrypted);
			Error err = fae->open_and_parse_password(file, key_,  FileAccessEncrypted::MODE_WRITE_AES256);
			if (err) {
				memdelete(fae);
				if(file)memdelete(file);
				file = nullptr;
				return false;
			}
			else {
				file = fae;
			}
		}
		if (file) {
			PoolByteArray temp;
			FileAccessMemory* buff = memnew(FileAccessMemory);
			uint16_t count = file->get_16();
			uint32_t size = 0;
			String key = "";
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
				if (ImageLoader::load_image(key, image, buff) != OK) {
					break;
				}
			}

			

			//buff->open_custom();
			
			
			//file->get_buffer(w.ptr(), buff.size());
			buff->close();
			file->close();
			memdelete(file);
			file = nullptr;
			buff = nullptr;
			return true;
		}
	}
	return false;
}
bool MUI::Fill(PoolByteArray db) {
	MRes::Fill(db);
}

