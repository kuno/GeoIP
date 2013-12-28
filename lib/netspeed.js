/**
 * Module dependencies
 */
var fs = require('fs');
var dns = require('dns');
var path  = require('path');

//
var info = require('debug')('geoip:lib:netSpeed:info');
var debug = require('debug')('geoip:lib:netSpeed:debug');

var binding = require('bindings')('native.node');

var helper = require('./helper');

/**
 * Exports
 */
module.exports = NetSpeed;

/**
 * Class
 *
 * @param {String} file
 * @param {Object} options
 */
function NetSpeed(file, options) {
    this.types = ['netspeed', 'netspeed cellular'];

    //
    helper.validateFile(file, this.types);

    this.native = new binding.NetSpeed(path.resolve(file), options);

    return this;
}

/**
 * Asynchronously lookup
 * @param {String} ipORdomain
 * @param {Function} callback
 */
NetSpeed.prototype.lookup = function(ipORdomain, callback) {
    var self = this;

    dns.lookup(ipORdomain, 4, function(err, address, family) {
        if (err) {
            // See @http://nodejs.org/api/dns.html#dns_error_codes
            debug('Dns error code: %s', err.code);
            return callback.call(self, err);
        }

        info('Address returned from node dns: %s', address);
        info('Family number returned from nod dns: %d', family);

        callback.call(self, null, self.native.lookupSync(address));
    });
};

/**
 * Synchronously lookup
 * @param {String} ipORdomain
 */
NetSpeed.prototype.lookupSync = function(ipORdomain) {
    return this.native.lookupSync(ipORdomain);
};

NetSpeed.prototype.lookupCellularSync = function(ipORdomain) {
    return this.native.lookupCellularSync(ipORdomain);
}; 

/**
 * Update
 * @param {String} newFile
 * @param {Object} options
 */
NetSpeed.prototype.update = function(newFile, options) {
    //
    helper.validateFile(newFile, this.types);

    this.native = new binding.NetSpeed(path.resolve(newFile), options);

    return this;
};
