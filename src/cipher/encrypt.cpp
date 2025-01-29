#include "encrypt.h"
#include "key.h"
#include "b64/base64.h"

Json::Value encrypt(char* msg, char* pass_key) {
	
	/* make sure the password is not null */
	assert((msg != nullptr, pass_key != nullptr));

	/* variables to store output */
	unsigned char outBuf[512+EVP_MAX_BLOCK_LENGTH];
	int outlen, tmplen;

	/*
	* generating key and iv
	*/
	unsigned char *key = key_generator(pass_key);
	unsigned char iv[12];
	if (RAND_bytes(iv, 12) < 0) {
		fprintf(stderr, "cipher intialization vector generation failed\n");
		return Json::Value(Json::nullValue);
	}

	/*
	* Creating a EVP_Cipher_CTX
	*/
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	if (!ctx) {
		fprintf(stderr, "cipher context initialization failed\n");
		EVP_CIPHER_CTX_free(ctx);
		return Json::Value(Json::nullValue);
	}

	/*
	* generating an encryptor
	*/
	if (!EVP_EncryptInit_ex2(ctx, EVP_aes_256_gcm(), key, iv, NULL)) {
		fprintf(stderr, "cipher intialization failed\n");
		EVP_CIPHER_CTX_free(ctx);
		return Json::Value(Json::nullValue);
	}
	EVP_CIPHER_CTX_set_padding(ctx, 0);

	/*
	* generating the cipher text
	*/
	if (!EVP_EncryptUpdate(ctx, outBuf, &outlen, reinterpret_cast<unsigned char*>(msg), strlen(msg))) {
		fprintf(stderr, "plain text encryption failed\n");
		EVP_CIPHER_CTX_free(ctx);
		return Json::Value(Json::nullValue);
	}

	/*
	 * Buffer passed to EVP_EncryptFinal() after data is 
	 * encrypted to avoid overwriting it.
	*/
	if (!EVP_EncryptFinal_ex(ctx, outBuf + outlen, &tmplen)) {
		fprintf(stderr, "error occurred while writing the final block of cipher text\n");
		EVP_CIPHER_CTX_free(ctx);
		return Json::Value(Json::nullValue);
	}
	outlen += tmplen;

	/* Clean UP */
	EVP_CIPHER_CTX_free(ctx);
	OPENSSL_free(key);

	/* Encoding the ciphertext and iv into base64 for storage */
	unsigned char base64Cipher[128];
	unsigned char base64iv[66];
	EVP_EncodeBlock(base64Cipher, outBuf, outlen);
	EVP_EncodeBlock(base64iv, iv, 12);
	base64iv[65] = '\0';

	/* Creating a JSON Object for storeage */
	std::string base64Iv(reinterpret_cast<char*>(base64iv));
	std::string cipherStr(reinterpret_cast<char*>(base64Cipher));
	Json::Value root(Json::objectValue);
	root["cipher_text"] = Json::Value(cipherStr);
	root["cipher_iv"] = Json::Value(base64Iv);
	return root;
}

std::string decrypt(Json::Value root, char* passkey) {
	unsigned char* plainTextBytes = (unsigned char*)malloc(512);
	unsigned char* in = (unsigned char*)malloc(512);
	unsigned char* key = key_generator(passkey);
	unsigned char* iv = (unsigned char*)malloc(12);
	int plainTextBytes_len, out_len, in_len, iv_length;
	EVP_CIPHER_CTX* ctx;
	
	/* Retrieving Cipher */
	const char* encodedCipher = root["cipher_text"].asCString();
	EVP_DecodeBlock(in, reinterpret_cast<const unsigned char*>(encodedCipher), strlen(encodedCipher));
	in_len = strlen(reinterpret_cast<const char*>(in));

	/* Retrieving iv */
	const char* encodedIV = root["cipher_iv"].asCString();
	iv_length = EVP_DecodeBlock(iv, reinterpret_cast<const unsigned char*>(encodedIV), strlen(encodedIV));

	/* Create and initialize the context */
	if (!(ctx = EVP_CIPHER_CTX_new())) {
		EVP_CIPHER_CTX_free(ctx);
		handleErrors();
	}

	/*
	* Initialize the decryption operation.
	*/
	if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, key, iv))
		handleErrors();

	/* Decrypting the message to obtain the plainTextBytes output. */
	if (1 != EVP_DecryptUpdate(ctx, plainTextBytes, &out_len, in, in_len))
		handleErrors();
	plainTextBytes_len = out_len;

	/* Finalize the decryption */
	EVP_DecryptFinal_ex(ctx, plainTextBytes + out_len, &out_len);
	plainTextBytes_len += out_len;

	/* store the plain text in a string */
	std::string plainText((char*)plainTextBytes, (char *)(plainTextBytes+plainTextBytes_len));
	std::cout << "Plain Text: " << plainText << std::endl;

	/* Clean UP */
	EVP_CIPHER_CTX_free(ctx);
	free(key);
	free(in);
	free(iv);
	free(plainTextBytes);

	return plainText;
}

void handleErrors() {
	ERR_print_errors_fp(stderr);
	abort();
}
