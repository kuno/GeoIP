var GEOIP = require('./gi.js');

var GEOIP_COUNTRY_BEGIN = 16776960;
var GEOIP_STATE_BEGIN_REV0 = 16700000;
var GEOIP_STATE_BEGIN_REV1 = 16000000;
var GEOIP_STANDARD = 0;
var GEOIP_MEMORY_CACHE = 1;
var GEOIP_SHARED_MEMORY = 2;
var STRUCTURE_INFO_MAX_SIZE = 20;
var DATABASE_INFO_MAX_SIZE = 100;
var GEOIP_COUNTRY_EDITION = 106;
var GEOIP_PROXY_EDITION = 8;
var GEOIP_ASNUM_EDITION = 9;
var GEOIP_NETSPEED_EDITION = 10;
var GEOIP_REGION_EDITION_REV0 = 112;
var GEOIP_REGION_EDITION_REV1 = 3;
var GEOIP_CITY_EDITION_REV0 = 111;
var GEOIP_CITY_EDITION_REV1 = 2;
var GEOIP_ORG_EDITION = 110;
var GEOIP_ISP_EDITION = 4;
var SEGMENT_RECORD_LENGTH = 3;
var STANDARD_RECORD_LENGTH = 3;
var ORG_RECORD_LENGTH = 4;
var MAX_RECORD_LENGTH = 4;
var MAX_ORG_RECORD_LENGTH = 300;
var GEOIP_SHM_KEY = 0x4f415401;
var US_OFFSET = 1;
var CANADA_OFFSET = 677;
var WORLD_OFFSET = 1353;
var FIPS_RANGE = 360;
var GEOIP_UNKNOWN_SPEED = 0;
var GEOIP_DIALUP_SPEED = 1;
var GEOIP_CABLEDSL_SPEED = 2;
var GEOIP_CORPORATE_SPEED = 3;
var GEOIP_DOMAIN_EDITION = 11;
var GEOIP_LOCATIONA_EDITION = 13;
var GEOIP_ACCURACYRADIUS_EDITION = 14;
var GEOIP_CITYCOMBINED_EDITION = 15;
var CITYCOMBINED_FIXED_RECORD = 7;

exports.load_shared_mem = function(file) {
};

exports._setup_segments = function(gi) {
};

exports.open = function(file, flags) {
    var gi = Object.create();
    gi.flags = flags;
    if (gi.flags && GEOIP_SHARED_MEMORY) {
    }
};

exports.close = function(gi) {
};

exports.id_by_name = function(gi, name) {
};

exports.id_by_addr = function(gi, addr) {
};

exports.code_by_name = function(gi, name) {
};

exports.name_by_name = function(gi, name) {
};

exports.id_by_addr = function(gi, addr) {
};

exports.code_by_addr = function(gi, addr) {
};

exports.name_by_addr = function(gi, addr) {
};
