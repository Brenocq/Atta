#!/bin/bash
set -e

# WARNING: If you are using use add the -E flag to keep the environment variables
#$ sudo -E ./scripts/install.sh

mkdir --parents build/linux
cd build/linux
cmake -D CMAKE_BUILD_TYPE=Release -D VCPKG_TARGET_TRIPLET=x64-linux -D CMAKE_TOOLCHAIN_FILE=../vcpkg.linux/scripts/buildsystems/vcpkg.cmake -D ATTA_GENERATE_PACKAGE_INSTALL=1 ../.. 
make -j
make install