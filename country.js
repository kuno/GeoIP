(function() {
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

    var GeoIP = Object.create(null);
    GeoIP.prototype.flags = undefined;
    GeoIP.prototype.filehandle = undefined;
    GeoIP.prototype.memory_buffer = undefined;
    GeoIP.prototype.databaseType = undefined;
    GeoIP.prototype.databaseSegments = undefined;
    GeoIP.prototype.record_length = undefined;
    GeoIP.prototype.shmid = undefined;
    GeoIP.prototype.GEOIP_COUNTRY_CODE_TO_NUMBER = [
    {"":0}, {"AP":1}, {"EU":2}, {"AD":3}, {"AE":4}, {"AF":5}, 
    {"AG":6}, {"AI":7}, {"AL":8}, {"AM":9}, {"AN":10}, {"AO":11}, 
    {"AQ":12}, {"AR":13}, {"AS":14}, {"AT":15}, {"AU":16}, {"AW":17}, 
    {"AZ":18}, {"BA":19}, {"BB":20}, {"BD":21}, {"BE":22}, {"BF":23}, 
    {"BG":24}, {"BH":25}, {"BI":26}, {"BJ":27}, {"BM":28}, {"BN":29}, 
    {"BO":30}, {"BR":31}, {"BS":32}, {"BT":33}, {"BV":34}, {"BW":35}, 
    {"BY":36}, {"BZ":37}, {"CA":38}, {"CC":39}, {"CD":40}, {"CF":41}, 
    {"CG":42}, {"CH":43}, {"CI":44}, {"CK":45}, {"CL":46}, {"CM":47}, 
    {"CN":48}, {"CO":49}, {"CR":50}, {"CU":51}, {"CV":52}, {"CX":53}, 
    {"CY":54}, {"CZ":55}, {"DE":56}, {"DJ":57}, {"DK":58}, {"DM":59}, 
    {"DO":60}, {"DZ":61}, {"EC":62}, {"EE":63}, {"EG":64}, {"EH":65}, 
    {"ER":66}, {"ES":67}, {"ET":68}, {"FI":69}, {"FJ":70}, {"FK":71}, 
    {"FM":72}, {"FO":73}, {"FR":74}, {"FX":75}, {"GA":76}, {"GB":77},
    {"GD":78}, {"GE":79}, {"GF":80}, {"GH":81}, {"GI":82}, {"GL":83}, 
    {"GM":84}, {"GN":85}, {"GP":86}, {"GQ":87}, {"GR":88}, {"GS":89}, 
    {"GT":90}, {"GU":91}, {"GW":92}, {"GY":93}, {"HK":94}, {"HM":95}, 
    {"HN":96}, {"HR":97}, {"HT":98}, {"HU":99}, {"ID":100}, {"IE":101}, 
    {"IL":102}, {"IN":103}, {"IO":104}, {"IQ":105}, {"IR":106}, {"IS":107}, 
    {"IT":108}, {"JM":109}, {"JO":110}, {"JP":111}, {"KE":112}, {"KG":113}, 
    {"KH":114}, {"KI":115}, {"KM":116}, {"KN":117}, {"KP":118}, {"KR":119}, 
    {"KW":120}, {"KY":121}, {"KZ":122}, {"LA":123}, {"LB":124}, {"LC":125}, 
    {"LI":126}, {"LK":127}, {"LR":128}, {"LS":129}, {"LT":130}, {"LU":131}, 
    {"LV":132}, {"LY":133}, {"MA":134}, {"MC":135}, {"MD":136}, {"MG":137}, 
    {"MH":138}, {"MK":139}, {"ML":140}, {"MM":141}, {"MN":142}, {"MO":143}, 
    {"MP":144}, {"MQ":145}, {"MR":146}, {"MS":147}, {"MT":148}, {"MU":149}, 
    {"MV":150}, {"MW":151}, {"MX":152}, {"MY":153}, {"MZ":154}, {"NA":155},
    {"NC":156}, {"NE":157}, {"NF":158}, {"NG":159}, {"NI":160}, {"NL":161}, 
    {"NO":162}, {"NP":163}, {"NR":164}, {"NU":165}, {"NZ":166}, {"OM":167}, 
    {"PA":168}, {"PE":169}, {"PF":170}, {"PG":171}, {"PH":172}, {"PK":173}, 
    {"PL":174}, {"PM":175}, {"PN":176}, {"PR":177}, {"PS":178}, {"PT":179}, 
    {"PW":180}, {"PY":181}, {"QA":182}, {"RE":183}, {"RO":184}, {"RU":185}, 
    {"RW":186}, {"SA":187}, {"SB":188}, {"SC":189}, {"SD":190}, {"SE":191}, 
    {"SG":192}, {"SH":193}, {"SI":194}, {"SJ":195}, {"SK":196}, {"SL":197}, 
    {"SM":198}, {"SN":199}, {"SO":200}, {"SR":201}, {"ST":202}, {"SV":203}, 
    {"SY":204}, {"SZ":205}, {"TC":206}, {"TD":207}, {"TF":208}, {"TG":209}, 
    {"TH":210}, {"TJ":211}, {"TK":212}, {"TM":213}, {"TN":214}, {"TO":215}, 
    {"TL":216}, {"TR":217}, {"TT":218}, {"TV":219}, {"TW":220}, {"TZ":221}, 
    {"UA":222}, {"UG":223}, {"UM":224}, {"US":225}, {"UY":226}, {"UZ":227}, 
    {"VA":228}, {"VC":229}, {"VE":230}, {"VG":231}, {"VI":232}, {"VN":233},
    {"VU":234}, {"WF":235}, {"WS":236}, {"YE":237}, {"YT":238}, {"RS":239}, 
    {"ZA":240}, {"ZM":241}, {"ME":242}, {"ZW":243}, {"A1":244}, {"A2":245}, 
    {"O1":246}, {"AX":247}, {"GG":248}, {"IM":249}, {"JE":250}, {"BL":251},
    {"MF":252}
    ];
    GeoIP.prototype.GEOIP_COUNTRY_CODES = [
        "", "AP", "EU", "AD", "AE", "AF", "AG", "AI", "AL", "AM", "AN", "AO", "AQ",
        "AR", "AS", "AT", "AU", "AW", "AZ", "BA", "BB", "BD", "BE", "BF", "BG", "BH",
        "BI", "BJ", "BM", "BN", "BO", "BR", "BS", "BT", "BV", "BW", "BY", "BZ", "CA",
        "CC", "CD", "CF", "CG", "CH", "CI", "CK", "CL", "CM", "CN", "CO", "CR", "CU",
        "CV", "CX", "CY", "CZ", "DE", "DJ", "DK", "DM", "DO", "DZ", "EC", "EE", "EG",
        "EH", "ER", "ES", "ET", "FI", "FJ", "FK", "FM", "FO", "FR", "FX", "GA", "GB",
        "GD", "GE", "GF", "GH", "GI", "GL", "GM", "GN", "GP", "GQ", "GR", "GS", "GT",
        "GU", "GW", "GY", "HK", "HM", "HN", "HR", "HT", "HU", "ID", "IE", "IL", "IN",
        "IO", "IQ", "IR", "IS", "IT", "JM", "JO", "JP", "KE", "KG", "KH", "KI", "KM",
        "KN", "KP", "KR", "KW", "KY", "KZ", "LA", "LB", "LC", "LI", "LK", "LR", "LS",
        "LT", "LU", "LV", "LY", "MA", "MC", "MD", "MG", "MH", "MK", "ML", "MM", "MN",
        "MO", "MP", "MQ", "MR", "MS", "MT", "MU", "MV", "MW", "MX", "MY", "MZ", "NA",
        "NC", "NE", "NF", "NG", "NI", "NL", "NO", "NP", "NR", "NU", "NZ", "OM", "PA",
        "PE", "PF", "PG", "PH", "PK", "PL", "PM", "PN", "PR", "PS", "PT", "PW", "PY",
        "QA", "RE", "RO", "RU", "RW", "SA", "SB", "SC", "SD", "SE", "SG", "SH", "SI",
        "SJ", "SK", "SL", "SM", "SN", "SO", "SR", "ST", "SV", "SY", "SZ", "TC", "TD",
        "TF", "TG", "TH", "TJ", "TK", "TM", "TN", "TO", "TL", "TR", "TT", "TV", "TW",
        "TZ", "UA", "UG", "UM", "US", "UY", "UZ", "VA", "VC", "VE", "VG", "VI", "VN",
        "VU", "WF", "WS", "YE", "YT", "RS", "ZA", "ZM", "ME", "ZW", "A1", "A2", "O1",
        "AX", "GG", "IM", "JE", "BL", "MF"
            ];

    GeoIP.prototype.GEOIP_COUNTRY_CODES3 = [
        "","AP","EU","AND","ARE","AFG","ATG","AIA","ALB","ARM","ANT","AGO","ATA","ARG",
        "ASM","AUT","AUS","ABW","AZE","BIH","BRB","BGD","BEL","BFA","BGR","BHR","BDI",
        "BEN","BMU","BRN","BOL","BRA","BHS","BTN","BVT","BWA","BLR","BLZ","CAN","CCK",
        "COD","CAF","COG","CHE","CIV","COK","CHL","CMR","CHN","COL","CRI","CUB","CPV",
        "CXR","CYP","CZE","DEU","DJI","DNK","DMA","DOM","DZA","ECU","EST","EGY","ESH",
        "ERI","ESP","ETH","FIN","FJI","FLK","FSM","FRO","FRA","FX","GAB","GBR","GRD",
        "GEO","GUF","GHA","GIB","GRL","GMB","GIN","GLP","GNQ","GRC","SGS","GTM","GUM",
        "GNB","GUY","HKG","HMD","HND","HRV","HTI","HUN","IDN","IRL","ISR","IND","IOT",
        "IRQ","IRN","ISL","ITA","JAM","JOR","JPN","KEN","KGZ","KHM","KIR","COM","KNA",
        "PRK","KOR","KWT","CYM","KAZ","LAO","LBN","LCA","LIE","LKA","LBR","LSO","LTU",
        "LUX","LVA","LBY","MAR","MCO","MDA","MDG","MHL","MKD","MLI","MMR","MNG","MAC",
        "MNP","MTQ","MRT","MSR","MLT","MUS","MDV","MWI","MEX","MYS","MOZ","NAM","NCL",
        "NER","NFK","NGA","NIC","NLD","NOR","NPL","NRU","NIU","NZL","OMN","PAN","PER",
        "PYF","PNG","PHL","PAK","POL","SPM","PCN","PRI","PSE","PRT","PLW","PRY","QAT",
        "REU","ROU","RUS","RWA","SAU","SLB","SYC","SDN","SWE","SGP","SHN","SVN","SJM",
        "SVK","SLE","SMR","SEN","SOM","SUR","STP","SLV","SYR","SWZ","TCA","TCD","ATF",
        "TGO","THA","TJK","TKL","TLS","TKM","TUN","TON","TUR","TTO","TUV","TWN","TZA",
        "UKR","UGA","UMI","USA","URY","UZB","VAT","VCT","VEN","VGB","VIR","VNM","VUT",
        "WLF","WSM","YEM","MYT","SRB","ZAF","ZMB","MNE","ZWE","A1","A2","O1",
        "ALA","GGY","IMN","JEY","BLM","MAF"  
            ];
    GeoIp.prototype.GEOIP_COUNTRY_NAMES = [
        "", "Asia/Pacific Region", "Europe", "Andorra", "United Arab Emirates",
        "Afghanistan", "Antigua and Barbuda", "Anguilla", "Albania", "Armenia",
        "Netherlands Antilles", "Angola", "Antarctica", "Argentina", "American Samoa",
        "Austria", "Australia", "Aruba", "Azerbaijan", "Bosnia and Herzegovina",
        "Barbados", "Bangladesh", "Belgium", "Burkina Faso", "Bulgaria", "Bahrain",
        "Burundi", "Benin", "Bermuda", "Brunei Darussalam", "Bolivia", "Brazil",
        "Bahamas", "Bhutan", "Bouvet Island", "Botswana", "Belarus", "Belize",
        "Canada", "Cocos (Keeling) Islands", "Congo, The Democratic Republic of the",
        "Central African Republic", "Congo", "Switzerland", "Cote D'Ivoire", "Cook Islands",
        "Chile", "Cameroon", "China", "Colombia", "Costa Rica", "Cuba", "Cape Verde",
        "Christmas Island", "Cyprus", "Czech Republic", "Germany", "Djibouti",
        "Denmark", "Dominica", "Dominican Republic", "Algeria", "Ecuador", "Estonia",
        "Egypt", "Western Sahara", "Eritrea", "Spain", "Ethiopia", "Finland", "Fiji",
        "Falkland Islands (Malvinas)", "Micronesia, Federated States of", "Faroe Islands",
        "France", "France, Metropolitan", "Gabon", "United Kingdom",
        "Grenada", "Georgia", "French Guiana", "Ghana", "Gibraltar", "Greenland",
        "Gambia", "Guinea", "Guadeloupe", "Equatorial Guinea", "Greece", "South Georgia and the South Sandwich Islands",
        "Guatemala", "Guam", "Guinea-Bissau",
        "Guyana", "Hong Kong", "Heard Island and McDonald Islands", "Honduras",
        "Croatia", "Haiti", "Hungary", "Indonesia", "Ireland", "Israel", "India",
        "British Indian Ocean Territory", "Iraq", "Iran, Islamic Republic of",
        "Iceland", "Italy", "Jamaica", "Jordan", "Japan", "Kenya", "Kyrgyzstan",
        "Cambodia", "Kiribati", "Comoros", "Saint Kitts and Nevis", "Korea, Democratic People's Republic of",
        "Korea, Republic of", "Kuwait", "Cayman Islands",
        "Kazakhstan", "Lao People's Democratic Republic", "Lebanon", "Saint Lucia",
        "Liechtenstein", "Sri Lanka", "Liberia", "Lesotho", "Lithuania", "Luxembourg",
        "Latvia", "Libyan Arab Jamahiriya", "Morocco", "Monaco", "Moldova, Republic of",
        "Madagascar", "Marshall Islands", "Macedonia",
        "Mali", "Myanmar", "Mongolia", "Macau", "Northern Mariana Islands",
        "Martinique", "Mauritania", "Montserrat", "Malta", "Mauritius", "Maldives",
        "Malawi", "Mexico", "Malaysia", "Mozambique", "Namibia", "New Caledonia",
        "Niger", "Norfolk Island", "Nigeria", "Nicaragua", "Netherlands", "Norway",
        "Nepal", "Nauru", "Niue", "New Zealand", "Oman", "Panama", "Peru", "French Polynesia",
        "Papua New Guinea", "Philippines", "Pakistan", "Poland", "Saint Pierre and Miquelon",
        "Pitcairn Islands", "Puerto Rico", "Palestinian Territory",
        "Portugal", "Palau", "Paraguay", "Qatar", "Reunion", "Romania",
        "Russian Federation", "Rwanda", "Saudi Arabia", "Solomon Islands",
        "Seychelles", "Sudan", "Sweden", "Singapore", "Saint Helena", "Slovenia",
        "Svalbard and Jan Mayen", "Slovakia", "Sierra Leone", "San Marino", "Senegal",
        "Somalia", "Suriname", "Sao Tome and Principe", "El Salvador", "Syrian Arab Republic",
        "Swaziland", "Turks and Caicos Islands", "Chad", "French Southern Territories",
        "Togo", "Thailand", "Tajikistan", "Tokelau", "Turkmenistan",
        "Tunisia", "Tonga", "Timor-Leste", "Turkey", "Trinidad and Tobago", "Tuvalu",
        "Taiwan", "Tanzania, United Republic of", "Ukraine",
        "Uganda", "United States Minor Outlying Islands", "United States", "Uruguay",
        "Uzbekistan", "Holy See (Vatican City State)", "Saint Vincent and the Grenadines",
        "Venezuela", "Virgin Islands, British", "Virgin Islands, U.S.",
        "Vietnam", "Vanuatu", "Wallis and Futuna", "Samoa", "Yemen", "Mayotte",
        "Serbia", "South Africa", "Zambia", "Montenegro", "Zimbabwe",
        "Anonymous Proxy","Satellite Provider","Other",
        "Aland Islands","Guernsey","Isle of Man","Jersey","Saint Barthelemy","Saint Martin"  
            ];
    GeoIP.prototype.GEOIP_CONTINENT_CODES = [
        "--", "AS", "EU", "EU", "AS", "AS", "NA", "NA", "EU", "AS", 
        "NA", "AF", "AN", "SA", "OC", "EU", "OC", "NA", "AS", "EU", 
        "NA", "AS", "EU", "AF", "EU", "AS", "AF", "AF", "NA", "AS", 
        "SA", "SA", "NA", "AS", "AN", "AF", "EU", "NA", "NA", "AS", 
        "AF", "AF", "AF", "EU", "AF", "OC", "SA", "AF", "AS", "SA", 
        "NA", "NA", "AF", "AS", "AS", "EU", "EU", "AF", "EU", "NA", 
        "NA", "AF", "SA", "EU", "AF", "AF", "AF", "EU", "AF", "EU", 
        "OC", "SA", "OC", "EU", "EU", "EU", "AF", "EU", "NA", "AS", 
        "SA", "AF", "EU", "NA", "AF", "AF", "NA", "AF", "EU", "AN", 
        "NA", "OC", "AF", "SA", "AS", "AN", "NA", "EU", "NA", "EU", 
        "AS", "EU", "AS", "AS", "AS", "AS", "AS", "EU", "EU", "NA", 
        "AS", "AS", "AF", "AS", "AS", "OC", "AF", "NA", "AS", "AS", 
        "AS", "NA", "AS", "AS", "AS", "NA", "EU", "AS", "AF", "AF", 
        "EU", "EU", "EU", "AF", "AF", "EU", "EU", "AF", "OC", "EU", 
        "AF", "AS", "AS", "AS", "OC", "NA", "AF", "NA", "EU", "AF", 
        "AS", "AF", "NA", "AS", "AF", "AF", "OC", "AF", "OC", "AF", 
        "NA", "EU", "EU", "AS", "OC", "OC", "OC", "AS", "NA", "SA", 
        "OC", "OC", "AS", "AS", "EU", "NA", "OC", "NA", "AS", "EU", 
        "OC", "SA", "AS", "AF", "EU", "EU", "AF", "AS", "OC", "AF", 
        "AF", "EU", "AS", "AF", "EU", "EU", "EU", "AF", "EU", "AF", 
        "AF", "SA", "AF", "NA", "AS", "AF", "NA", "AF", "AN", "AF", 
        "AS", "AS", "OC", "AS", "AF", "OC", "AS", "EU", "NA", "OC", 
        "AS", "AF", "EU", "AF", "OC", "NA", "SA", "AS", "EU", "NA", 
        "SA", "NA", "NA", "AS", "OC", "OC", "OC", "AS", "AF", "EU", 
        "AF", "AF", "EU", "AF", "--", "--", "--", "EU", "EU", "EU", 
        "EU", "NA", "NA" 
            ];
    exports.geoip_load_shared_mem = function(file) {
    };

    exports._setup_segments = function(gi) {
    };

    exports.geoip_open = function(file, flags) {
        var gi = Object.create(GeoIP);
        gi.flags = flags;
        if (gi.flags && GEOIP_SHARED_MEMORY) {
            gi.shmid = 
    };

    exports.geoip_close = function(gi) {
    };

    exports.geoip_country_id_by_name = function(gi, name) {
    };

    exports.geoip_country_id_by_addr = function(gi, addr) {
    };

    exports.geoip_country_code_by_name = function(gi, name) {
    };

    exports.geoip_country_name_by_name = function(gi, name) {
    };

    exports.geoip_country_id_by_addr = function(gi, addr) {
    };

    exports.geoip_country_code_by_addr = function(gi, addr) {
    };

    exports.geoip_country_name_by_addr = function(gi, addr) {
    };

})();
