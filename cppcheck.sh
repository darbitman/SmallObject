#!/bin/bash
cppcheck --enable=all -j2 --language=c++ --report-progress --std=c++17 -I/usr/local/include -ibenchmark -itest src/