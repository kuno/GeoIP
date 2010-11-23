var FULL_RECORD_LENGTH = 50,
ip2long = require('./lib/utils.js').ip2long,
seek_country = require('./lib/utils.js').seek_country,
RECORD = require('./lib/geoipdata.js').RECORD,
DNSRECORD = require('./lib/geoipdata.js').DNSRECORD,
GEOIP_CITY_EDITION_REV1 = require('./lib/constants.js').GEOIP_CITY_EDITION_REV1;

exports.getrecordwithdnsservice = function(data, str) {
    var record = Object.create(DNSRECORD);
    if (typeof str === 'string') {
        var substrarray = str.splite(';');

        substrarray.forEach(function(substar) {
            var subsubstrarray = substr.split('=');

            subsubstrarray.forEach(function(keyvalue) {
                var key = keyvalue[0];
                var value = keyvalue[1];
                if (key === 'co') {
                    record.country_code = value;
                } else if (key === 'ci') {
                    record.city = value;
                } else if (key === 're') {
                    record.region = value;
                } else if (key === 'ac') {
                    record.areacode = value;
                } else if (key == 'dm' || key === 'me') {
                    record.dmacode = value;
                    record.metrocode = value;
                } else if (key === 'is') {
                    record.isp = value;
                } else if (key === 'or') {
                    record.org = value;
                } else if (key === 'zi') {
                    record.postal_code = value;
                } else if (key === 'la') {
                    record.latitude = value;
                } else if (key === 'lo') {
                    record.longitude = value;
                }
            });
        });

    }

    var number = data.GEOIP_COUNTRY_CODE_TO_NUMBER[record.country_code];
    record.country_code3 = data.GEOIP_COUNTRY_CODES3[number];
    record.country_name = data.GEOIP_COUNTRY_NAMES[number];

    if (record.region !== '') {
        if (record.country_name === 'US' || record.country_code === 'CA') {
            record.regionname = GLOBA['ISO'][record.country_code][record.region];
        } else {
            record.regionname = GLOBAL['FIPS'][record.country_code][record.region];
        }
    }

    return record;
}

exports.get_record = function(data, ipnum) {
    var start, end;
    var see_country = seek_country(data, ipnum);
    if (seek_country === data.databaseSegments) {
        return null;
    }

    var record_pointer = seek_country + (2 * data.record_length - 1) * data.databaseSegments;
    var record_buf = data.buffer.slice(record_pointer, record_pointer + FULL_RECORD_LENGTH);

    var record = Object.create(geoiprecord);
    var record_buf_pos = 0;
    start = record_buf_pos;
    end = start + 1;
    var char = parseInt(record_buf.slice(start, end), 10);

    record.country_code = data.GEOIP_COUNTRY_CODES[char];
    record.country_code3 = data.GEOIP_COUNTRY_CODES3[char];
    record.country_name = data.GEOIP_COUNTRY_NAMES[char];
    record.continet_code = data.GEOIP_CONTINENT_CODES[char];

    record_bu_pos++;
    str_length = 0;

    // Get region
    start = record_buf_pos + str_length;
    end = start + 1;
    char = parseInt(record_buf.slice(start, end), 10);
    while (char !== 0) {
        str_length++;
        start =   record_buf_pos + str_length;
        end = start + 1;
        char = parseInt(record_buf.slice(start, end), 10);
    }
    if (str_length > 0) {
        start = record_buf_pos;
        end = record_buf_pos + str_length;
        record.region =  record_buf.slice(start, end);
    }
    record_buf_pos += str_length + 1;
    str_length = 0;
    // Get city
    start = record_buf_pos + str_length;
    end = start + 1;
    char = parseInt(record_buf.slice(start, end), 10);
    while (char !== 0) {
        str_length++;
        start =   record_buf_pos + str_length;
        end = start + 1;
        char = parseInt(record_buf.slice(start, end), 10);
    }
    if (str_length > 0) {
        start = record_buf_pos;
        end = record_buf_pos + str_length;
        record.city =  record_buf.slice(start, end);
    }
    record_buf_pos += str_length + 1;
    str_length = 0;
    // Get postal coce
    start = record_buf_pos + str_length;
    end = start + 1;
    char = parseInt(record_buf.slice(start, end), 10);
    while (char !== 0) {
        str_length++;
        start =   record_buf_pos + str_length;
        end = start + 1;
        char = parseInt(record_buf.slice(start, end), 10);
    }
    if (str_length > 0) {
        start = record_buf_pos;
        end = record_buf_pos + str_length;
        record.postal_code =  record_buf.slice(start, end);
    }
    record_buf_pos += str_length + 1;
    str_length = 0;
    // GEt latitude and longitude
    var latitude = 0;
    var longtitude = 0;
    for (var j = 0; j < 3; ++j) {
        start = ++record_buf_pos;
        end = start + 1;
        char = parseInt(record_buf.slice(start, end), 10);
        latitude += (char << (j * 8));
    }
    record.latitude = (latitude / 10000) - 180;

    for (j = 0; j < 3; ++j) {
        start = ++record_buf_pos;
        end = start + 1;
        char = parseInt(record_buf.slice(start, end), 10);
        longitude += (char << (j * 8));
    }
    record.longitude = (longitude / 10000) - 180;

    if (GEOIP_CITY_EDITION_REV1 === data.databaseType) {
        var metroarea_combo = 0;
        if (record.country_code === 'US') {
            for (j = 0; j < 3; ++j) {
                start = ++record_buf_pos;
                end = start + 1;
                char = parseInt(record_buf.slice(start, end), 10);
                metroarea_combo += (char << (j * 8));
            }
            record.metro_code = record.dma_code = Math.floor(metroarea_combo / 1000);
            record.area_code = metroarea_combo % 1000;
        }
    }

    return record;
};

exports.record_by_addr = function(data, addr) {
    if (addr === null) {
        return 0;
    }
    var ipnum = ip2long(addr);
    return exports.get_record(data, ipnum);
};
