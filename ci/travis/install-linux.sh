#!/usr/bin/env bash
sudo pip3 install conan
sudo apt-get install ninja-build
sudo rm -rf /usr/local/cmake*
CMAKE_INSTALLER=install-cmake.sh
curl -sSL https://cmake.org/files/v3.13/cmake-3.13.4-Linux-x86_64.sh -o ${CMAKE_INSTALLER}
chmod +x ${CMAKE_INSTALLER}
sudo ./${CMAKE_INSTALLER} --prefix=/usr/local --skip-license
export PATH=${DEPS_DIR}/cmake/bin:${PATH}
