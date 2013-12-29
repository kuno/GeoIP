var warning = require('debug')('geoip:test:utils:warning');
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

var geoip = require('../index.js');

describe('Check database file type', function() {
    describe(path.resolve(__dirname, '../database/GeoIP-171_20040418.dat'), function() {
        it('is netspeed db', function(done) {
            var file = path.resolve(__dirname, '../database/GeoIP-171_20040418.dat');
            var type = geoip.check(file);

            type.should.equal('netspeed');
            setTimeout(done, 1);
        });
    });

    describe(path.resolve(__dirname, '../database/GeoIPASNum.dat'), function() {
        it('is asnum db', function(done) {
            var file = path.resolve(__dirname, '../database/GeoIPASNum.dat');
            var type = geoip.check(file);

            type.should.equal('asnum');
            setTimeout(done, 1);
        });
    });

    describe(path.resolve(__dirname, '../database/GeoIP.dat'), function() {
        it('is country db', function(done) {
            var file = path.resolve(__dirname, '../database/GeoIP.dat');
            var type = geoip.check(file);

            type.should.equal('country');
            setTimeout(done, 1);
        });
    });

    describe(path.resolve(__dirname, '../database/GeoIPv6.dat'), function() {
        it('is country_v6 db', function(done) {
            var file = path.resolve(__dirname, '../database/GeoIPv6.dat');
            var type = geoip.check(file);

            type.should.equal('country_v6');
            setTimeout(done, 1);
        });
    });

    describe(path.resolve(__dirname, '../database/GeoLiteCity.dat'), function() {
        it('is city db', function(done) {
            var file = path.resolve(__dirname, '../database/GeoLiteCity.dat');
            var type = geoip.check(file);

            type.should.equal('city');
            setTimeout(done, 1);
        });
    });

    describe(path.resolve(__dirname, '../database/GeoLiteCityv6.dat'), function() {
        it('is city_v6 db', function(done) {
            var file = path.resolve(__dirname, '../database/GeoLiteCityv6.dat');
            var type = geoip.check(file);

            type.should.equal('city_v6');
            setTimeout(done, 1);
        });
    });

    describe(path.resolve(__dirname, '../database/GeoIPRegion-515.dat'), function() {
        it('is region db', function(done) {
            var file = path.resolve(__dirname, '../database/GeoIPRegion-515.dat');
            var type = geoip.check(file);

            type.should.equal('region');
            setTimeout(done, 1);
        });
    });

    describe(path.resolve(__dirname, '../database/GeoIPOrg-111.dat'), function() {
        it('is org db', function(done) {
            var file = path.resolve(__dirname, '../database/GeoIPOrg-111.dat');
            var type = geoip.check(file);

            type.should.equal('org');
            setTimeout(done, 1);
        });
    });

    describe(path.resolve(__dirname, '../database/GeoIPASNum.dat'), function() {
        it('is asnum db', function(done) {
            var file = path.resolve(__dirname, '../database/GeoIPASNum.dat');
            var type = geoip.check(file);

            type.should.equal('asnum');
            setTimeout(done, 1);
        });
    });

    describe(path.resolve(__dirname, '../database/GeoIPNetSpeedCell.dat'), function() {
        it('is netspeed cellular db', function(done) {
            var file = path.resolve(__dirname, '../database/GeoIPNetSpeedCell.dat');
            var type = geoip.check(file);

            type.should.equal('netspeed cellular');
            setTimeout(done, 1);
        });
    });
});
