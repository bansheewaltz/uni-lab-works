#!/bin/bash

STATUS=0

mkdir build
cd build


cmake .. -GNinja -DUNLIMITED=ON -DCMAKE_C_COMPILER=clang
ninja
ninja test || STATUS=1

rm -rf *

cmake .. -GNinja -DUNLIMITED=ON -DCMAKE_C_COMPILER=gcc -DENABLE_ASAN=true -DENABLE_USAN=true
ninja
ninja test || STATUS=2

ninja check || STATUS=3

exit $STATUS
