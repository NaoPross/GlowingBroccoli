#!/bin/sh

set -x

mkdir -p build
cd build

qmake ../GlowingBroccoli.pro
make -j2
