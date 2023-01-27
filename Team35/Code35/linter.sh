#!/usr/bin/env bash

cpplint --extensions=h,cpp --filter="-build/c++11",-legal/copyright --recursive ./src/spa/src/
