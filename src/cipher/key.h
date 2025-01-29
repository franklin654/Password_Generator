#pragma once

#include <cassert>
#include <cstring>
#include <openssl/sha.h>
#include <openssl/err.h>

unsigned char* key_generator(char* msg);
