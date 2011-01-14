#!/usr/bin/env sh

TEST_DIR=$PWD

GEOIP='http://archive/geoip/GeoIP.dat'
GEOLITECITY='http://archive/geoip/GeoLiteCity.dat'
REGION='http://archive/geoip/GeoIPRegion.dat'
NETSPEED='http://archive/geoip/GeoIPNetSpeed.dat'
ORG='http://archive/geoip/GeoIPOrg.dat'
ASN='http://archive/geoip/GeoIPASNum.dat'

cd /tmp || return 1

if [ ! -e /tmp/GeoIP.dat ]; then
  wget $GEOIP
fi

if [ ! -e /tmp/GeoLiteCity.dat ]; then
  wget $GEOLITECITY
fi

if [ ! -e /tmp/GeoIPOrg.dat ]; then
  wget $ORG  
fi

if [ ! -e /tmp/GeoIPASNum.dat ]; then
  wget $ASN
fi

if [ ! -e /tmp/GeoIPNetSpeed.dat ]; then
  wget $NETSPEED
fi

if [ ! -e /tmp/GeoIPRegion.dat ]; then
  wget $REGION
fi

cd $TEST_DIR || return 1

echo "Start to test Country module......"
echo "============================================================"
node ./country.js 
echo "============================================================"
echo "Country module test is finished!"

echo ""

echo "Start to test City moduel......"
echo "============================================================"
node ./city.js
echo "============================================================"
echo "City moduel test is finished!"

echo ""

echo "Start to test Org module......"
echo "============================================================"
node ./org.js
echo "============================================================"
echo "Org module test is finished!"

echo ""

echo "Start to test Region module......"
echo "============================================================"
node ./region.js
echo "============================================================"
echo "Region module test is finished!"

echo ""

echo "Start to test NetSpeed module......"
echo "============================================================"
node ./netspeed.js
echo "============================================================"
echo "NetSpeed module test is finished!"
