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

exports = {};
