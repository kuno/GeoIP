#!/usr/bin/env sh

TEST_DIR=$PWD
BUILD=$TEST_DIR/../build/default/geoip.node

GEOIP='http://archive/geoip/GeoIP.dat'
GEOIPV6='http://archive/geoip/GeoIPv6.dat'
GEOLITECITY='http://archive/geoip/GeoLiteCity.dat'
REGION='http://archive/geoip/GeoIPRegion.dat'
NETSPEED='http://archive/geoip/GeoIPNetSpeed.dat'
ORG='http://archive/geoip/GeoIPOrg.dat'
ASN='http://archive/geoip/GeoIPASNum.dat'

cd /tmp || return 1

if [ ! -e /tmp/GeoIP.dat ]; then
  wget $GEOIP
fi

if [ ! -e /tmp/GeoIPv6.dat ]; then
  wget $GEOIPV6
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

if [ ! -e $BUILD ]; then
  echo "Start to building..."
  cd $TEST_DIR/.. || return 1
  node-waf configure build || return 1
  echo "Building is finished!"
fi

cd $TEST_DIR || return 1

echo "Start to test Country ipv4 module......"
echo "============================================================"
node ./country.js 
echo "============================================================"
echo "Country ipv4 module test is finished!"

echo ""

echo "Start to test Country ipv6 module......"
echo "============================================================"
node ./country6.js 
echo "============================================================"
echo "Country ipv6 module test is finished!"

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
