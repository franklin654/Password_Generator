# Password_Generator

## Description
This repository contains a Password Generator application written in C++. The application is designed to create and manage passwords securely.

## Features
- Generate secure passwords.
- Store passwords in a secure directory.
- User-friendly interface built with Qt.

## Installation
To build and run the application, you need a C++ compiler, Qt framework, and vcpkg for dependency management.

### Install Dependencies
1. Install [vcpkg](https://github.com/microsoft/vcpkg):
    ```sh
    git clone https://github.com/microsoft/vcpkg.git
    cd vcpkg
    ./bootstrap-vcpkg.sh
    ./vcpkg integrate install
    ```
2. Use vcpkg to install jsoncpp and OpenSSH:
    ```sh
    ./vcpkg install jsoncpp
    ./vcpkg install openssh
    ```

### Build the Project
1. Clone the repository:
    ```sh
    git clone https://github.com/franklin654/Password_Generator.git
    ```
2. Navigate to the project directory:
    ```sh
    cd Password_Generator
    ```
3. Build the project using CMake:
    ```sh
    mkdir build
    cd build
    cmake ..
    make
    ```

## Contributing
Contributions are welcome! Please submit a pull request or open an issue to discuss any changes or improvements.

## License
This project does not have a license specified.

## Author
[franklin654](https://github.com/franklin654)

Let me know if you need any more modifications or information!
