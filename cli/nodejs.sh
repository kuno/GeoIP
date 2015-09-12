#!/bin/bash

NODE_VERSION=`node --version  | sed 's/v//g'`

echo '#######'
echo "Node Version ${NODE_VERSION}"
echo '#######'

# memwatch
if [[ `echo $NODE_VERSION | grep -i "^0.[6-9|10|11].*"` ]]; then
    echo "Going to install memwatch"
    npm install memwatch
else
    echo "Going to uninstall memwatch"
    npm remove memwatch
fi

# development files
if [[ ! -e "$HOME/.node-gyp/${NODE_VERSION}/common.gypi" ]]; then
    echo "Going to fetch development files"
    node-gyp install ${NODE_VERSION}
fi
