# GlowingBroccoli: Snake Game
Projektmanagement & Software Engineering
OST FHO Rapperswil Herbstsemester 2020

## Documentation

Documentation (in german) can be found under `doc/` in the project directory. It
contains a Software Requirements Specifications `doc/srs/build/srs.pdf` and a
Project Plan `doc/plan/build/plan.pdf`. For Doxygen there is a Doxyfile under
`doc/doxygen/Doxyfile` which builds a reference in `doc/doxygen/html`.

## How to build

Dependencies:

- C++ compiler
- Qt >= 5.1 (ex. with QtCreator or just qmake)
- Doxygen
- LaTeX (documentation)

### On Linux

For ubuntu-based distributions the following packages should satisfy the minimum
requirements to build the project. Optionally `qtcreator` can also be installed.
```
$ sudo apt install clang qt5base-dev qt5-qmake make graphviz doxygen
```
Clone the project *with submodules*:
```
$ git clone --recursive https://github.com/NaoPross/GlowingBroccoli.git
```
Then to build the CI script can be used:
```
/proj/dir $ ./ci/build.sh # builds project, runs unit-tests and builds doxygen
```
Or if one prefers, manually:
```
/proj/dir $ mkdir build && cd build
/proj/dir/build $ qmake ../GlowingBroccoli.pro
/proj/dir/build $ make -j4
/proj/dir/build $ ./GlowingBroccoli # launch program
```
then similarly the Unit-Tests:
```
/proj/dir $ git submodule update --init # in case you didn't clone with --recursive
/proj/dir $ mkdir build-tests && cd build-tests
/proj/dir/build-tests $ qmake ../test/GlowingBroccoliTest.pro
/proj/dir/build-tests $ make -j4
/proj/dir/build-tests $ ./GlowingBroccoliTest # run test suite
```
finally Doxygen
```
/proj/dir $ doxygen doc/doxygen/Doxyfile
```

### On Windows / MacOS

Install Qt and QtCreator (OpenSource Edition) from
https://www.qt.io/product/development-tools
Then launch QtCreator and load `proj/dir/GlowingBroccoli.pro`, the main
project, and `proj/dir/test/GlowingBroccoliTest.pro` for the Unit-Tests.
