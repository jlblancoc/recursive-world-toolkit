#!/bin/sh

SRC_DIR=`pwd`
BUILD_DIR=build

CMAKE_C_FLAGS="-Wall -Wextra -Wabi -O2"
CMAKE_CXX_FLAGS="-Wall -Wextra -Wabi -O2"

function build ()
{
  #env 
  mkdir $BUILD_DIR && cd $BUILD_DIR

  cmake $SRC_DIR
  make
}

case $TASK in
  build ) build;;
esac
