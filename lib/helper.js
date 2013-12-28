/**
 * Module dependencies
 */
var fs = require('fs');
var dns = require('dns');
var path  = require('path');
var util = require('util');

//
var info = require('debug')('geoip:lib:helper:info');
var debug = require('debug')('geoip:lib:helper:debug');

var binding = require('bindings')('native.node');

exports.validateFile = function(file, types) {
    var err;
    var type = binding.check(file);

    //
    if (!fs.existsSync(file)) {
        err = new Error(
            util.format('File %s dose not exists', file)
        );

        debug('Error: ', err.message);
        throw new Error(err);
    }

    //
    if (types.indexOf(type) === -1) {
        err = new Error(
            util.format(
                'The type of file %s, "%s", is not in the %j',
                file, type, types
            )
        );

        debug('Error: ', err.message);
        throw new Error(err);
    }

    return;
}
