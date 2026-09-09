#!/bin/bash

cd build-web/
mkdir dist/

mv script.js dist/
mv index.html dist/
mv asset.data dist/
mv main.wasm dist/
mkdir dist/assets/
cp assets/yinyang.svg dist/assets/

echo "Done!"
