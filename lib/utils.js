/**
 * Module dependencies
 */
var fs = require('fs');
var dns = require('dns');
var path  = require('path');

//
var info = require('debug')('geoip:lib:util:info');
var debug = require('debug')('geoip:lib:util:debug');

var binding = require('bindings')('native.node');

exports.validateFile = function(file, type) {
    var message;

    //
    if (!fs.existsSync(file)) {
        message =  'File ' + file + 'dose not exists';

        debug(message);
        throw new Error(message);
    }

    //
    if (binding.check(file) !== type) {
        message = 'The file ' + file + ' is not a ' + type + ' database.';

        debug(message)
        throw new Error(message);
    }

    return;
}
