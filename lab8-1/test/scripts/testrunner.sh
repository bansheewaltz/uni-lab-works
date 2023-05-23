#!/bin/bash

mkdir build
cd build


cmake .. -GNinja -DUNLIMITED=ON -DCMAKE_C_COMPILER=gcc
ninja
ninja test

cmake .. -GNinja -DUNLIMITED=ON -DCMAKE_C_COMPILER=clang -DENABLE_ASAN=true -DENABLE_ASAN=true
ninja
ninja test

ninja check
