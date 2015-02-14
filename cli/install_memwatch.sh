#!/bin/bash

NODE_VERSION=`node --version | grep -i v0.1[1-9].*`
IOJS_VERSION=`iojs --version`

echo '#######'
echo "Node Version ${NODE_VERSION}"
echo "Iojs Version ${IOJS_VERSION}"
echo '#######'

if [[ ! $NODE_VERSION && ! $IOJS_VERSION ]]; then
    echo "Going to install memwatch"
    npm install memwatch
else
    echo "Going to uninstall memwatch"
    npm remove memwatch
fi
