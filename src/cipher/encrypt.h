#pragma once

#include <iostream>
#include <string>
#include <cassert>
#include <cstring>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <json/json.h>

Json::Value encrypt(char* msg = nullptr, char* pass_key = nullptr);
std::string decrypt(Json::Value root, char* pass_key = nullptr);
void handleErrors();