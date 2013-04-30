/**
 * New node file
 */
var geoip = require('../index.js');
var path = require('path');
var NetSpeed = geoip.NetSpeed;
var netspeed =
  new NetSpeed(path.resolve(__dirname, '../../../../data/GeoIPSpeedCell.dat'));

var netspeed_str1 = netspeed.lookupCellularSync('8.8.8.8');
var netspeed_str2 = netspeed.lookupCellularSync('1.0.32.1');
var netspeed_str3 = netspeed.lookupCellularSync('1.187.0.1');
var netspeed_str4 = netspeed.lookupCellularSync('1.221.232.1');

console.log("8.8.8.8         => " + netspeed_str1);
console.log("1.0.32.1        => " + netspeed_str2);
console.log("1.187.0.1       => " + netspeed_str3);
console.log("1.221.232.1     => " + netspeed_str4);

//RTA-24 addresses that may be causing crashes
var troublesome_addresses = ['5.32.44.3','10.32.44.3','127.100.0.0'];
var address;
while (address                  = troublesome_addresses.shift()) {
 var speed                   = netspeed.lookupCellularSync(address);
 console.log(padWithSpaces(address,15) + " => " + speed);
}

function padWithSpaces (str, length) {
 var number_of_spaces        = length > str.length ? length - str.length : 0;
 var paddedString            = str;
 for (var i = 0; i < number_of_spaces; i++) {
     paddedString += " ";
 }
 return paddedString;
}