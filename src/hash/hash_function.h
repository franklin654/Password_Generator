#pragma once

#include <cassert>
#include <openssl/sha.h>
#include <openssl/err.h>
#include <vector>

std::vector<unsigned char> hash_calculator(const char* msg);
