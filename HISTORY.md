## v0.1.0, 2010-11-25
1, Initial release.

## v0.1.1, 2010-11-27:
1, Removed GeoIP.dat, due to licence(or money) problem.

2, renamed city property to City, renamed country property to Country.

3, Allow close method to set all properties of an exsiting obj to undefined.

## v0.1.2, 2010-12-02:
1, Fixed missing of isIP method in node 0.2.x series.

2, Hidden some unsafe an useless funcions.

## v0.1.3, 2010-12-02:
1, Fixed that open and close methos dosen't works in real script.

## v0.2.1, 2010-12-03:
1, added support for GeoIPASNum.dat binary data.

2, make org_by_addr method return an array of org name string.

## v0.3.0, 2010-12-15:
1, All module now has asynchronous version method.

2, When data is not match with module, throw an error object.

3, When not found  desired data, return an error string instead an error objerct.

## v0.3.1, 2010-12-16:
1, Limit the length of the floating part of latitude and langitude to 6 digits.

## v0.3.2, 2011-01-21:
1, add geoip.check(data) method.

2, validate country binary data based on 8-types fingerprint.

3, support nodejs v0.3.6.

4, make geoip.close(data) method sync style.

## v0.3.3, 2011-02-06:
1, drop support for nodejs 0.2.1, pass testing up to nodejs 0.3.8.

2, add geoip.filter method.

3, re-writed testing shell script.

4, If not found matched data, alway return null.

5, use index.js as default main module.

6, added github bug reporting address.

## v0.3.3-1, 2011-02-12
1, make compatibility with nodejs v0.4.0 stable branch.

## v0.3.4, 2011-02-15
1, new seekCountry algorithm, speedup to about 3x faster (Vladimir Dronnikov)
.
2, added simple usage property.

3, improve org_by_domain method output.

## v0.3.4-1, 2011-03-07
1, fixed cotinent typo.

## V0.4.0beta1, 2011-05-11
switch to geoip C library binding.

## v0.4.0beta2, 2011-05-15
1, added error checking in async methods.

2, fixed latitude and longitude range problems.

3, added city property in record object.

## v0.4.0rc, 2011-05-20
1, implemented basic ipv6 support in Country module.

2, fixed vavious typos.

## v0.4.0, 2011-05-23
1, added country ipv6 samples.

2, clear some redundant codes.

## v0.4.0final, 2011-05-24
1, change version to 0.4.0final.

## v0.4.1, 2011-06-21
1, build addon during installation not preinstall.

## v0.4.2, 2011-07-23
1, enbale update database on the fly.

2, disable close method.

3, removed all sleep function.

4, always execute lookup in the main thread.

5, separate county ipv6 into a indiviual module.

## v0.4.2-1, 2011-07-24
1, added James Taylor as contributor.

2, added libgeoip's version literature.

## v0.4.2-2, 2011-08-01
1, disabled libgeoip version literature due to compatibility issue.

## v0.4.3, 2011-08-03
1, enabled converting city name to utf8 characters.

## v0.4.4, 2011-09-15
1, support lookup city information through ipv6 address.

## v0.4.5, 2011-11-07
1, now needs nodejs >= 0.6.0

## v0.4.6, 2012-07-29
1, fully support nodejs 0.8.x, breaking the compatibility with 0.6.x.

## v0.4.7*, 2012-12-22
1, fixed most memory leak

2, some cleanup

## v0.4.7, 2013-01-04
1, fix serious sagfault issue, re-release v0.4.7.

## v0.4.8, 2013-02-15
1, fix missing city data when looking up some special ip in async mode.

## v0.4.9, 2013-03-12
1, make compatible with node 0.10.0.

## v0.4.10, 2013-05-04
1, Make package self-building, i.e not depending on external libgeoip any
more.

## v0.4.11, 2013-10-29
1, Use nan to solved version dependency.

2, Fix #60, Bug in the async country lookup method.

## v0.4.12, 2013-11-14
1, Update geoip c api to v1.5.2.

## v0.5.0, 2014-01-01
1, Wrap native c++ object as js object.

2, Not directly expose native addon any more.

3, Use node's dns module to resovle domain name (Ilya Furman).

4, Add official support for netspeed cellular db (mcvella?).

## v0.5.1, 2014-01-13
1, Update libgeoip to v1.6.0.

## v0.5.2, 2014-01-30
1, Migrate all c++ code to new pattern.
2, Reserved sample of legacy c++  code.
3, Happy Chinese new year 2014 :)
