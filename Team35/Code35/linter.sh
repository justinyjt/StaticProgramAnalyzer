#!/usr/bin/env bash

cpplint --extensions=h,cpp --linelength=120 --filter="-build/c++11",-legal/copyright,-runtime/references,-runtime/threadsafe_fn --recursive ./src/spa/src/ ./src/unit_testing/src/ ./src/integration_testing/src/

# windows:
# python c:\python310\lib\site-packages\cpplint.py --extensions="h,cpp" --linelength=120 --filter="-build/c++11,-legal/copyright,-runtime/references" --recursive ./Team35/Code35/src/spa/src/qps    
