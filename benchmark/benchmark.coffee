#!/usr/local/bin/coffee
util = require 'util'
getLocation = require('./index') '/tmp/GeoLiteCity.dat'
n255 = -> Math.floor Math.random()*256
start = new Date().getTime();
for i in [0...10000]
	ip = n255() + '.' + n255() + '.' + n255() + '.' + n255()
	getLocation ip, true
end = new Date().getTime();

util.puts ((end - start) / 1000)
