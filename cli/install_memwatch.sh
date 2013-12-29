#!/bin/bash

NODE_VERSION=`node --version | grep -i v0.11.*`

echo $NODE_VERSION

if [[ ! $NODE_VERSION ]]; then
    npm install memwatch
else
    npm remove memwatch
fi
