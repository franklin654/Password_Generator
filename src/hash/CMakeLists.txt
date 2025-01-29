set(HASH_SOURCES
	hash_function.h
	hash_function.cpp
)

find_package(OpenSSL REQUIRED)

add_library(sha256 ${HASH_SOURCES})
if(${OpenSSL_FOUND})
    target_include_directories(sha256 PRIVATE OpenSSL)
    target_link_libraries(sha256 PRIVATE OpenSSL::Crypto)
    target_link_libraries(sha256 PRIVATE OpenSSL::SSL)
endif()
