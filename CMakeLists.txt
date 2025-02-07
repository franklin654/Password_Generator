cmake_minimum_required(VERSION 3.16)

set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}\\scripts\\buildsystems\\vcpkg.cmake")

project(Password_Generator VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(OPENSSL_ROOT_DIR "B:\\vcpkg\\packages\\openssl_x64-windows")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core)

add_subdirectory(src/hash)
add_subdirectory(src/cipher)

set(PROJECT_SOURCES
        src/main.cpp
        src/global_variables.h src/global_variables.cpp
        src/homepage.h src/homepage.cpp src/homepage.ui
        src/mainwidget.h src/mainwidget.cpp src/mainwidget.ui
        src/createpassword.h src/createpassword.cpp src/createpassword.ui
        src/Password_Utilities/password.h src/Password_Utilities/password.cpp
        src/masterpassword.h src/masterpassword.cpp src/masterpassword.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Password_Generator
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
endif()

target_link_libraries(Password_Generator PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(Password_Generator PUBLIC sha256)
target_link_libraries(Password_Generator PRIVATE Cipher)


include(GNUInstallDirs)
install(TARGETS Password_Generator
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Password_Generator)
endif()
