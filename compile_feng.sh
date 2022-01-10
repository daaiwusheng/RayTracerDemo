#! /bin/bash

module add qt/5.13.0
qmake RaytraceRenderWindow.pro
make -j8

