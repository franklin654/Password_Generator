#include "hash_function.h"


std::vector<unsigned char> hash_calculator(char * msg) {
    assert((msg != nullptr));
	unsigned char buffer[SHA256_DIGEST_LENGTH];
	SHA256(reinterpret_cast<unsigned char*>(msg), strlen(msg), buffer);
    std::vector<unsigned char> hashString;
	hashString.assign(buffer, buffer + SHA256_DIGEST_LENGTH);
	return hashString;
}
