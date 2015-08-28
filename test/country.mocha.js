var warning = require('debug')('geoip:test:country:warning');
var semver = require('semver');
var randomip = require('random-ip');
// memwatch only works with node 0.6.x ~ 0.10.x
if (semver.gte(process.version, '0.6.0') && semver.lt(process.version, '0.11.0')) {
    require('memwatch').on('leak', function(info) {
        warning('Memory leak detected: %j', info);
    });
}

var path = require('path');
var mocha = require('mocha');
var chai = require('chai');
chai.Assertion.includeStack = true;

var should = chai.should();

var geoip = require('../index.js');
var Country = geoip.Country;
var file = path.resolve(__dirname, '../database/GeoIP.dat');

describe('Country', function() {
    describe('Instance', function() {
        var instance = new Country(file, true);

        it('should be a object', function(done) {
            instance.should.be.an('object');
            setTimeout(done, 1);
        });

        it('should has a lookup method', function(done) {
            instance.lookup.should.be.a('function');
            setTimeout(done, 1);
        });

        it('should has a lookupSync method', function(done) {
            instance.lookupSync.should.be.a('function');
            setTimeout(done, 1);
        });

        it('should has a update method', function(done) {
            instance.update.should.be.a('function');
            setTimeout(done, 1);
        });

        describe('Synchrouns Lookup', function() {
            it('should throw error when input is not a string', function(done) {
                try {
                    instance.lookupSync(null);
                } catch(err) {
                    should.exist(err);
                    setTimeout(done, 1);
                }
            });

            it('should can find location by domain', function(done) {
                var data = instance.lookupSync('www.163.com');
                data.should.be.a('object');
                setTimeout(done, 1);
            });

            it('should can find location by ip address', function(done) {
                var data = instance.lookupSync('8.8.8.8');
                data.should.be.a('object');
                data.country_code.should.equals('US');
                setTimeout(done, 1);
            });

            it('should can find location by ip address', function(done) {
                var data = instance.lookupSync('168.126.63.1');
                data.should.be.a('object');
                data.country_code.should.equals('KR');
                setTimeout(done, 1);
            });
        });

        describe('Asynchrouns Lookup', function() {
            it('should return error when input is not a string', function(done) {
                instance.lookup(null, function(err, data) {
                    should.exist(err);
                    setTimeout(done, 1);
                });
            });

            it('should can find location by domain', function(done) {
                instance.lookup('www.163.com', function(err, data) {
                    should.not.exist(err);
                    should.exist(data);
                    data.should.be.an('object');
                    setTimeout(done, 1);
                });
            });

            it('should can find location by ip address', function(done) {
                instance.lookup('8.8.8.8', function(err, data) {
                    should.not.exist(err);
                    should.exist(data);
                    data.should.be.an('object');
                    setTimeout(done, 1);
                });
            });

            it('lookupSync method should return the same result as lookup method', function(done) {
                var country = instance.lookupSync('134.191.232.69');
                instance.lookup('134.191.232.69', function (err, data) {
                    should.not.exist(err);
                    should.exist(data);
                    data.country_code.should.equal(country.country_code);
                    setTimeout(done, 1);
                });
            });
        });

        describe('Test random IPs', function() {
          it('should not crash for 2000 v4 IPs', function (done) {
            for (var i = 0; i < 2000; ++i) {
              var ip = randomip('0.0.0.0', 0);
              //console.log(ip);
              var addr = instance.lookupSync(ip);
              //console.log(addr);
            }
              setTimeout(done, 1);
          });
        });

        describe('Update database on the fly', function() {
            it('should be ok', function(done) {
                instance.update(file).should.be.ok;
                setTimeout(done, 1);
            });
        });
    });

    describe('Silent instance', function() {
        var instance = new Country(file, {silent: true});
        describe('Synchrouns Lookup', function() {
            it('should return null when input is not a string', function(done) {
                should.equal(instance.lookupSync(null), null);
                setTimeout(done, 1);
            });
        });

        describe('Asynchrouns Lookup', function() {
            it('should return null when input is not a string', function(done) {
                instance.lookup(null, function(err, data) {
                    should.not.exist(err);
                    should.equal(data, null);
                    setTimeout(done, 1);
                });
            });
        });
    });
});
