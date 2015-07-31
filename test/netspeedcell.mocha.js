var warning = require('debug')('geoip:test:city:warning');
var semver = require('semver');
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

var NetSpeedCell = require('../lib/netspeedcell');
var file = path.resolve(__dirname, '../database/GeoIPNetSpeedCell.dat');

describe('NetSpeedCell', function() {
    describe('Instance', function() {
        var instance = new NetSpeedCell(file, true);

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

            it.skip('should can find speed by domain', function(done) {
                var data = instance.lookupSync('www.163.com');
                data.should.be.a('string');
                setTimeout(done, 1);
            });

            it('should can find speed by ip address', function(done) {
                var data = instance.lookupSync('151.42.234.143');
                data.should.be.a('string');
                data.should.equal("Cable/DSL");
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

            it.skip('should find speed by domain', function(done) {
                instance.lookup('www.163.com', function(err, data) {
                    should.not.exist(err);
                    should.exist(data);
                    data.should.be.a('string');
                    setTimeout(done, 1);
                });
            });

            it('should find speed by ip address', function(done) {
                instance.lookup('212.245.206.97', function(err, data) {
                    should.not.exist(err);
                    should.exist(data);
                    data.should.be.a('string');
                    //data.should.equal("Cable/DSL");
                    setTimeout(done, 1);
                });
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
        var instance = new NetSpeedCell(file, {silent: true});
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
