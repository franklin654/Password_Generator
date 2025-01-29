#include "key.h"

unsigned char* key_generator(char * msg) {
    assert((msg != nullptr));
	unsigned char* buffer = (unsigned char*)malloc(SHA256_DIGEST_LENGTH);
	SHA256(reinterpret_cast<unsigned char*>(msg), strlen(msg), buffer);
	return buffer;
}
