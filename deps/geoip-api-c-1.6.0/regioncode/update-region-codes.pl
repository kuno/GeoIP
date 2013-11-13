#!/usr/bin/perl

use strict;
use warnings;
use LWP::Simple qw(get $ua);
# WPEngine does not allow LWP::Simple clients
$ua->agent("Mozilla/0.1");

our $VERSION = '0.01';

#my $content = get('http://dev.maxmind.com/static/maxmind-region-codes.csv');
my $content = get('http://dev.maxmind.com/static/csv/codes/maxmind/region.csv');
exit 5 unless $content;
print "Update fips10_4.txt\n";
open my $fips,       '>', 'fips10_4.txt' or die $!;
open my $content_fh, '<', \$content      or die $!;
print $fips q["iso 3166 country","fips 10-4 region code","name"], "\n";
print $fips grep { !/^(US|CA)/ } <$content_fh>;
print $fips <<__FIPS_CA_US__;
CA,01,"Alberta"
CA,02,"British Columbia"
CA,03,"Manitoba"
CA,04,"New Brunswick"
CA,05,"Newfoundland and Labrador"
CA,07,"Nova Scotia"
CA,08,"Ontario"
CA,09,"Prince Edward Island"
CA,10,"Quebec"
CA,11,"Saskatchewan"
CA,12,"Yukon Territory"
CA,13,"Northwest Territories"
CA,14,"Nunavut"
US,01,"Alabama"
US,02,"Alaska"
US,04,"Arizona"
US,05,"Arkansas"
US,06,"California"
US,08,"Colorado"
US,09,"Connecticut"
US,10,"Delaware"
US,11,"District of Columbia"
US,12,"Florida"
US,13,"Georgia"
US,15,"Hawaii"
US,16,"Idaho"
US,17,"Illinois"
US,18,"Indiana"
US,19,"Iowa"
US,20,"Kansas"
US,21,"Kentucky"
US,22,"Louisiana"
US,23,"Maine"
US,24,"Maryland"
US,25,"Massachusetts"
US,26,"Michigan"
US,27,"Minnesota"
US,28,"Mississippi"
US,29,"Missouri"
US,30,"Montana"
US,31,"Nebraska"
US,32,"Nevada"
US,33,"New Hampshire"
US,34,"New Jersey"
US,35,"New Mexico"
US,36,"New York"
US,37,"North Carolina"
US,38,"North Dakota"
US,39,"Ohio"
US,40,"Oklahoma"
US,41,"Oregon"
US,42,"Pennsylvania"
US,44,"Rhode Island"
US,45,"South Carolina"
US,46,"South Dakota"
US,47,"Tennessee"
US,48,"Texas"
US,49,"Utah"
US,50,"Vermont"
US,51,"Virginia"
US,53,"Washington"
US,54,"West Virginia"
US,55,"Wisconsin"
US,56,"Wyoming"
__FIPS_CA_US__
print <<__TXT__;

Now run
./generate_regionName.pl > ../libGeoIP/regionName.c
__TXT__
exit 0;
