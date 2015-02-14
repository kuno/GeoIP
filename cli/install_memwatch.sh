#!/bin/bash

NODE_VERSION=`node --version | grep -i v0.12.*`
IOJS_VERSION='iojs --version'

echo $NODE_VERSION

if [[ ! $NODE_VERSION && ! IOJS_VERSION ]]; then
    npm install memwatch
else
    npm remove memwatch
fi
