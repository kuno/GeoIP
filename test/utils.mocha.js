var path = require('path');
var mocha = require('mocha');
var chai = require('chai');
chai.Assertion.includeStack = true;

var should = chai.should();

var geoip = require('../index.js');

describe('Check database file type', function() {
  describe('../database/GeoIP-171_20040418.dat', function() {
    it('should equal to netspeed', function(done) {
      var file = path.resolve(__dirname, '../database/GeoIP-171_20040418.dat');
      var type = geoip.check(file);

      type.should.equal('netspeed');
      setTimeout(done, 1);
    });
  });

  describe('../database/GeoIPASNum.dat', function() {
    it('should equal to asnum', function(done) {
      var file = path.resolve(__dirname, '../database/GeoIPASNum.dat');
      var type = geoip.check(file);

      type.should.equal('asnum');
      setTimeout(done, 1);
    });
  });

  describe('../database/GeoIP.dat', function() {
    it('should equal to country', function(done) {
      var file = path.resolve(__dirname, '../database/GeoIP.dat');
      var type = geoip.check(file);

      type.should.equal('country');
      setTimeout(done, 1);
    });
  });

  describe('../database/GeoIPv6.dat', function() {
    it('should equal to country_v6', function(done) {
      var file = path.resolve(__dirname, '../database/GeoIPv6.dat');
      var type = geoip.check(file);

      type.should.equal('country_v6');
      setTimeout(done, 1);
    });
  });

  describe('../database/GeoLiteCity.dat', function() {
    it('should equal to city', function(done) {
      var file = path.resolve(__dirname, '../database/GeoLiteCity.dat');
      var type = geoip.check(file);

      type.should.equal('city');
      setTimeout(done, 1);
    });
  });

  describe('../database/GeoLiteCityv6.dat', function() {
    it('should equal to city_v6', function(done) {
      var file = path.resolve(__dirname, '../database/GeoLiteCityv6.dat');
      var type = geoip.check(file);

      type.should.equal('city_v6');
      setTimeout(done, 1);
    });
  });

  describe('../database/GeoIPRegion-515.dat', function() {
    it('should equal to region', function(done) {
      var file = path.resolve(__dirname, '../database/GeoIPRegion-515.dat');
      var type = geoip.check(file);

      type.should.equal('region');
      setTimeout(done, 1);
    });
  });

  describe('../database/GeoIPOrg-111.dat', function() {
    it('should equal to org', function(done) {
      var file = path.resolve(__dirname, '../database/GeoIPOrg-111.dat');
      var type = geoip.check(file);

      type.should.equal('org');
      setTimeout(done, 1);
    });
  });

  describe('../database/GeoIPASNum.dat', function() {
    it('should equal to asnum', function(done) {
      var file = path.resolve(__dirname, '../database/GeoIPASNum.dat');
      var type = geoip.check(file);

      type.should.equal('asnum');
      setTimeout(done, 1);
    });
  });
});
