#!/bin/sh

set -e
ls
mkdir utils_mac_clang
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DINSTALL_PREFIX=../utils_mac_clang
make -j4
sudo make install
cd ..
# zip the compiled binary
zip -r utils_mac_clang.zip utils_mac_clang
# list all executable file
ls
cd utils_mac_clang
ls
