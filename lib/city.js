/**
 * Module dependencies.
 */
var dns = require('dns');
var path  = require('path');

//
var info = require('debug')('info');
var debug = require('debug')('debug');

var binding = require('bindings')('native.node');

/**
 * Exports
 */
module.exports = City;

/**
 * Class
 *
 * @param {String} file
 * @param {Object} options
 */
function City(file, options) {
    this.type = 'city';
    this.native = new binding.City(path.resolve(file), options);

    return this;
}

/**
 * Async lookup
 * @param {String} ipordomain
 * @param {Function} callback
 */
City.prototype.lookup = function(ipORdomain, callback) {
    var self = this;

    dns.lookup(ipORdomain, function(err, address, family) {
        if (err) return callback.call(self, err);

        debug('Address returned from node dns: %s', address);
        debug('Family number returned from nod dns: %d', family);

        callback.call(self, null, self.native.lookupSync(address));
    });
};

/**
 * Sync lookup
 * @param {String} ipordomain
 */
City.prototype.lookupSync = function(ipORdomain) {
    return this.native.lookupSync(ipORdomain);
};

/**
 * Update
 * @param {String} newFile
 * @param {Object} options
 */
City.prototype.update = function(newFile, options) {
    //
    if (binding.check(newFile) !== this.type) {
        throw new Error('The file ' + newFile + ' is not a ' + this.type + ' database.');
    }

    this.native = new binding.City(path.resolve(newFile), options);

    return this;
};
