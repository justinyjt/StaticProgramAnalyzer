#!/usr/bin/env bash

cpplint --extensions=h,cpp --linelength=120 --filter="-build/c++11",-legal/copyright,-runtime/references --recursive ./src/spa/src/
