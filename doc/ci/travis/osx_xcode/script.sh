#!/bin/sh

set -e
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DUNITTEST=1 ..
make
cd ..
ls
