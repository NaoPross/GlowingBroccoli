#!/bin/sh
# set -x

mkdir -p build
cd build

# build documentation
doxygen doc/doxygen/Doxyfile

# build project
qmake ../GlowingBroccoli.pro
make -j2
