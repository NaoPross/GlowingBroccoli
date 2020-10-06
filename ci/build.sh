#!/bin/sh
# set -x

# build documentation
doxygen doc/doxygen/Doxyfile

mkdir -p build
cd build

# build project
qmake ../GlowingBroccoli.pro
make -j2
