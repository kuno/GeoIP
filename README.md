__GeoIP binding for nodejs__

[![NPM version](https://badge.fury.io/js/geoip.png)](http://badge.fury.io/js/geoip)
[![Build status](https://secure.travis-ci.org/kuno/GeoIP.png)](http://travis-ci.org/kuno/GeoIP)
[![Dependencies Status](https://david-dm.org/kuno/GeoIP.png)](https://david-dm.org/kuno/GeoIP)
[![Bitdeli Badge](https://d2weczhvl823v0.cloudfront.net/kuno/geoip/trend.png)](https://bitdeli.com/free "Bitdeli Badge")

Get geolocation information based on domain or IP address.

**Pure javascript implementation**

If you are looking for a pure javascript implementation, the [`node-geoip`](https://github.com/bluesmoon/node-geoip) maybe your choice.

###Technical Architecture

![new_architecture](https://github.com/kuno/GeoIP/raw/master/misc/new_architecture.png)

###Memory leak issue
As up to the version 0.4.7 (commit [22dadd98646f75318412b54b3de90cc079d34b1d](https://github.com/kuno/GeoIP/commit/22dadd98646f75318412b54b3de90cc079d34b1d)), there will be some memory leak when new a object from GeoIP class.

Here is the [report](https://gist.github.com/4357339), I'll update it frequently.

Fortunately, the number of leaked bytes is not huge, and you wont new a object very offen.

If you are willing to fix this but, please fork this project.

###Need support special database?
Corrently, this package supports the most common databases from MaxMind.

But, it still a lot of special databases out there.

Since I am not a user of any product/service of maxmind, if you require this pacakge to support special db, please make sure you can delivery the testing db and the ip address/domain that can be used to against the db.


##Installation

### for nodejs >= 0.8.x

    [sudo] npm install geoip [-g]

### for nodejs 0.6.x

    [sudo] npm install geoip@0.4.5 [-g]

### for nodejs 0.4.x

    [sudo] npm install geoip@0.4.4 [-g]

### for nodejs older than 0.4.0

From v0.4.0, geoip need nodejs >= 0.4.0, if you want to use it on old nodejs, you can:

    npm install geoip@0.3.4-1


##Usage

See [usage doc](./USAGE.md).
