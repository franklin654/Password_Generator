#pragma once

#include <string>
#include <cassert>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <json/json.h>

Json::Value encrypt(const char* msg = nullptr, unsigned char* key = nullptr);
std::string decrypt(Json::Value root, unsigned char* key = nullptr);
void handleErrors();
