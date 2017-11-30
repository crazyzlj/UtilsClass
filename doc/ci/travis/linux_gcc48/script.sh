#!/bin/sh

set -e
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DUNITTEST=1 -DRUNCOV=1 ..
make
make UnitTestCoverage
cd ..
ls
codecov -t d9199af5-b185-48e9-b264-71bdc42b85f4 -X gcov
