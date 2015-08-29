#!/bin/bash

IOJS_VERSION=`iojs --version | sed 's/v//g'`

echo '#######'
echo "Iojs Version ${IOJS_VERSION}"
echo '#######'

if [[ "${IOJS_VERSION}" ]] && [[ ! -e "$HOME/.node-gyp/${IOJS_VERSION}/common.gypi" ]]; then
    echo "Going to install iojs development file"
    npm install pangyp;
    iojs ./node_modules/.bin/pangyp install ${IOJS_VERSION};
fi
