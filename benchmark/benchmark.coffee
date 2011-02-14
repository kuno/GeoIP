#!/usr/local/bin/coffee

getLocation = require('./index') '/tmp/GeoLiteCity.dat'

n255 = -> Math.floor Math.random()*256
for i in [0...10000]
	ip = n255() + '.' + n255() + '.' + n255() + '.' + n255()
	getLocation ip, true
