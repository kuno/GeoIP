/**
 * Module dependencies
 */
var fs = require('fs');
var dns = require('dns');
var path  = require('path');

//
var info = require('debug')('geoip:lib:city6:info');
var debug = require('debug')('geoip:lib:city6:debug');

var binding = require('bindings')('native.node');

var helper = require('./helper');

/**
 * Exports
 */
module.exports = City6;

/**
 * Class
 *
 * @param {String} file
 * @param {Object} options
 */
function City6(file, options) {
    this.types = ['city_v6'];

    //
    helper.validateFile(file, this.types);

    this.native = new binding.City6(path.resolve(file), options);

    return this;
}

/**
 * Asynchronously lookup
 * @param {String} ipORdomain
 * @param {Function} callback
 */
City6.prototype.lookup = function(ipORdomain, callback) {
    var self = this;

    dns.lookup(ipORdomain, 6, function(err, address, family) {
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
City6.prototype.lookupSync = function(ipORdomain) {
    return this.native.lookupSync(ipORdomain);
};

/**
 * Update
 * @param {String} newFile
 * @param {Object} options
 */
City6.prototype.update = function(newFile, options) {
    //
    helper.validateFile(newFile, this.types);

    this.native = new binding.City6(path.resolve(newFile), options);

    return this;
};
