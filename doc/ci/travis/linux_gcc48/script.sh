#!/bin/sh

set -e
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DUNITTEST=1 -DRUNCOV=1 ..
make
make test
make gcov_utilsclass
cd ..
ls
codecov -t 002bf92f-6d1a-4e84-85ef-fa5c31ca4866 -X gcov
