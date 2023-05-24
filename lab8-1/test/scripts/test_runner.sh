#!/bin/bash

STATUS=0

mkdir build
cd build


cmake .. -GNinja -DUNLIMITED=ON -DCMAKE_C_COMPILER=gcc
ninja
ninja test  || STATUS=1

rm -rf *

cmake .. -GNinja -DUNLIMITED=ON -DCMAKE_C_COMPILER=clang -DENABLE_ASAN=true
ninja
ninja test  || STATUS=3

rm -rf *

cmake .. -GNinja -DUNLIMITED=ON -DCMAKE_C_COMPILER=clang -DENABLE_USAN=true
ninja
ninja test  || STATUS=4

ninja check || STATUS=5

exit $STATUS
