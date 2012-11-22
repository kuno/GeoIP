var path = require('path');
var mocha = require('mocha');
var chai = require('chai');
chai.Assertion.includeStack = true;

var should = chai.should();

var geoip = require('../index.js');
var Country6 = geoip.Country6;
var file = path.resolve(__dirname, '../database/GeoIPv6.dat');
var instance = new Country6(file, true);

describe('Country6', function() {
  describe('Instance', function() {
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

    it('should has a close method', function(done) {
      instance.close.should.be.a('function');
      setTimeout(done, 1);
    });
  });

  describe('Synchrouns Lookup', function() {
    it('should find location by domain', function(done) {
      var data = instance.lookupSync('www.google.com');
      data.should.be.a('object');
      setTimeout(done, 1);
    });

    it('should find location by ip address', function(done) {
      var data = instance.lookupSync('2406:a000:f0ff:fffe::122d');
      data.should.be.a('object');
      setTimeout(done, 1);
    }); 
  });

  describe('Asynchrouns Lookup', function() {
    it('should find location by demain', function(doen) {
      instance.lookup('www.google.com', function(err, data) {
        should.not.exist(err);
        should.exist(data);
        data.should.be.an('object');
      }); 
    });

    it('should find location by ip address', function(doen) {
      instance.lookup('2607:f0d0:1002:0051:0000:0000:0000:0004', function(err, data) {
        should.not.exist(err);
        should.exist(data);
        data.should.be.an('object');
      }); 
    });

    describe('Update database on the fly', function() {
      it('should be ok', function(done) {
        instance.update(file).should.be.ok;
        setTimeout(done, 1);
      });
    }); 
  });
}); 
// var assert = require('assert'), 
//     geoip = require('../index.js');
// 
// var Country6 = geoip.Country6;
// var country6 = new Country6('../database/GeoIPv6.dat', true);
// 
// var sync_data6 = country6.lookupSync('www.google.com');
// assert.ok(sync_data6, 'Can not find google domain in country ipv6 module');
// 
// var sync_data6 = country6.lookupSync('2406:a000:f0ff:fffe::122d');
// assert.ok(sync_data6, 'Can not find google ip in country ipv6 database');
// 
// console.log(sync_data6);
// country6.lookup('2607:f0d0:1002:0051:0000:0000:0000:0004', function(err, data6) {
//     if (err) {throw err;}
//     if (data6) {
//       console.log(data6);
//     }
// });
// 
// assert.ok(country6.update('../database/GeoIPv6.dat'), 'Oops when updating country ipv6 database');
