#!/bin/bash
outputFile=$(find lorthon.*.so)
mv $outputFile lorthon.so
mkdir -p output
cp lorthon.so tst/lorthon.so
mv lorthon.so output/lorthon.so
