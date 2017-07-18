#!/bin/sh

set -e
ls
mkdir utils_linux_gcc
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DINSTALL_PREFIX=../utils_linux_gcc
make -j4
sudo make install
cd ..
# zip the compiled binary
zip -r utils_linux_gcc.zip utils_linux_gcc
# list all executable file
ls
cd bin
ls
