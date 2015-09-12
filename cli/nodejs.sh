#!/bin/bash

NODE_VERSION=`node --version | grep -i "v0.[6-9|10|11].*"`

echo '#######'
echo "Node Version ${NODE_VERSION}"
echo '#######'

if [[ -e $NODE_VERSION ]]; then
    echo "Going to install memwatch"
    npm install memwatch
else
    echo "Going to uninstall memwatch"
    npm remove memwatch
fi
