/**
 * Module dependencies
 */
var fs = require('fs');
var dns = require('dns');
var path  = require('path');

//
var info = require('debug')('geoip:lib:country:info');
var debug = require('debug')('geoip:lib:country:debug');

var binding = require('bindings')('native.node');

var helper = require('./helper');

/**
 * Exports
 */
module.exports = Country6;

/**
 * Class
 *
 * @param {String} file
 * @param {Object} options
 */
function Country6(file, options) {
    this.types = ['country_v6'];

    //
    helper.validateFile(file, this.types);

    this.native = new binding.Country6(path.resolve(file), options);

    return this;
}

/**
 * Asynchronously lookup
 * @param {String} ipORdomain
 * @param {Function} callback
 */
Country6.prototype.lookup = function(ipORdomain, callback) {
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
Country6.prototype.lookupSync = function(ipORdomain) {
    return this.native.lookupSync(ipORdomain);
};

/**
 * Update
 * @param {String} newFile
 * @param {Object} options
 */
Country6.prototype.update = function(newFile, options) {
    //
    helper.validateFile(newFile, this.types);

    this.native = new binding.Country6(path.resolve(newFile), options);

    return this;
};
