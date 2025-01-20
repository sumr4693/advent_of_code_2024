#!/bin/bash

mkdir -p ./build
{
    EXE=./build/$1
    EXE_DBG=./build/$1_dbg
    g++ -O2 $1.cpp -o $EXE
    g++ -g $1.cpp -o $EXE_DBG
    ./${EXE}
}