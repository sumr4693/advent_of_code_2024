#!/bin/bash

mkdir -p ./build
{
    EXE=./build/$1
    g++ -O2 $1.cpp -o $EXE
    ./${EXE}
}