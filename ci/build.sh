#!/bin/sh
# set -x

# build documentation
doxygen doc/doxygen/Doxyfile

mkdir -p build
cd build

# build project
qmake ../GlowingBroccoli.pro
make -j2

cd ..

# pull googletest
git submodule update --init

mkdir -p build-test
cd build-test

# build tests
qmake ../test/GlowingBroccoliTest.pro
make -j2
./GlowingBroccoliTest
