#!/bin/bash

IOJS_VERSION=`iojs --version`

echo '#######'
echo "Iojs Version ${IOJS_VERSION}"
echo '#######'

if [[ $IOJS_VERSION ]]; then
    echo "Going to install iojs development file"
    npm install pangyp;
    iojs ./node_modules/.bin/pangyp install ${IOJS_VERSION};
fi
