set(CIPHER_SOURCES
	encrypt.h encrypt.cpp
        key.h key.cpp
)

list(APPEND CMAKE_PREFIX_PATH "B:\\vcpkg\\packages\\jsoncpp_x64-windows\\share\\jsoncpp")

find_package(OpenSSL REQUIRED COMPONENTS Crypto SSL)
find_package(jsoncpp CONFIG REQUIRED)

add_library(Cipher ${CIPHER_SOURCES})

if(${OpenSSL_FOUND})
	target_include_directories(Cipher PUBLIC OpenSSL::Crypto OpenSSL::SSL)
	target_link_libraries(Cipher PUBLIC OpenSSL::Crypto OpenSSL::SSL)
endif()
if(${jsoncpp_FOUND})
	target_include_directories(Cipher PUBLIC jsoncpp_lib)
	target_link_libraries(Cipher PUBLIC jsoncpp_lib)
endif()

target_include_directories(Cipher PUBLIC ${PROJECT_BINARY_DIR})
