#!/bin/bash
# Rebuild I-IBM2 on Linux (Ubuntu 24.04, Qt 5.15, GCC 13).
# Deps: qtbase5-dev libopencv-dev libsuitesparse-dev freeglut3-dev libpcl-dev libeigen3-dev libboost-filesystem-dev
set -e
cd "$(dirname "$0")"
# 1) bundled OpenMesh 3.1 -> static lib
if [ ! -f 3rdparty/OpenMesh-3.1/lib/libOpenMesh.a ]; then
  cd 3rdparty/OpenMesh-3.1; mkdir -p build lib
  for f in $(find OpenMesh/Core OpenMesh/Tools/Utils OpenMesh/Tools/Decimater OpenMesh/Tools/Subdivider OpenMesh/Tools/Smoother \
             -name "*.cc" ! -name "*T.cc" ! -name "MeshClean.cc" ! -path "*/Templates/*") $(find OpenMesh/Tools/Utils -name "*.c"); do
    g++ -std=c++11 -O2 -fPIC -w -D_USE_MATH_DEFINES -I. -c $f -o build/$(echo $f | tr / _).o
  done
  ar rcs lib/libOpenMesh.a build/*.o; cd ../..
fi
# 2) the app
cd I-IBM; qmake I-IBM_linux.pro -o Makefile.linux; make -f Makefile.linux -j$(nproc)
echo "built: $(realpath ../bin/I-IBM)"
