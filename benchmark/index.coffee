'use strict'

GEOIP_COUNTRY_CODES = [
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
]

GEOIP_COUNTRY_CODES3 = [
	"","AP","EU","AND","ARE","AFG","ATG","AIA","ALB","ARM","ANT","AGO","AQ","ARG",
	"ASM","AUT","AUS","ABW","AZE","BIH","BRB","BGD","BEL","BFA","BGR","BHR","BDI",
	"BEN","BMU","BRN","BOL","BRA","BHS","BTN","BV","BWA","BLR","BLZ","CAN","CC",
	"COD","CAF","COG","CHE","CIV","COK","CHL","CMR","CHN","COL","CRI","CUB","CPV",
	"CX","CYP","CZE","DEU","DJI","DNK","DMA","DOM","DZA","ECU","EST","EGY","ESH",
	"ERI","ESP","ETH","FIN","FJI","FLK","FSM","FRO","FRA","FX","GAB","GBR","GRD",
	"GEO","GUF","GHA","GIB","GRL","GMB","GIN","GLP","GNQ","GRC","GS","GTM","GUM",
	"GNB","GUY","HKG","HM","HND","HRV","HTI","HUN","IDN","IRL","ISR","IND","IO",
	"IRQ","IRN","ISL","ITA","JAM","JOR","JPN","KEN","KGZ","KHM","KIR","COM","KNA",
	"PRK","KOR","KWT","CYM","KAZ","LAO","LBN","LCA","LIE","LKA","LBR","LSO","LTU",
	"LUX","LVA","LBY","MAR","MCO","MDA","MDG","MHL","MKD","MLI","MMR","MNG","MAC",
	"MNP","MTQ","MRT","MSR","MLT","MUS","MDV","MWI","MEX","MYS","MOZ","NAM","NCL",
	"NER","NFK","NGA","NIC","NLD","NOR","NPL","NRU","NIU","NZL","OMN","PAN","PER",
	"PYF","PNG","PHL","PAK","POL","SPM","PCN","PRI","PSE","PRT","PLW","PRY","QAT",
	"REU","ROU","RUS","RWA","SAU","SLB","SYC","SDN","SWE","SGP","SHN","SVN","SJM",
	"SVK","SLE","SMR","SEN","SOM","SUR","STP","SLV","SYR","SWZ","TCA","TCD","TF",
	"TGO","THA","TJK","TKL","TLS","TKM","TUN","TON","TUR","TTO","TUV","TWN","TZA",
	"UKR","UGA","UM","USA","URY","UZB","VAT","VCT","VEN","VGB","VIR","VNM","VUT",
	"WLF","WSM","YEM","YT","SRB","ZAF","ZMB","MNE","ZWE","A1","A2","O1",
	"ALA","GGY","IMN","JEY","BLM","MAF"
]

GEOIP_COUNTRY_NAMES = [
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
	"Kazakstan", "Lao People's Democratic Republic", "Lebanon", "Saint Lucia",
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
]

#
# maxmind DB
#
buffer = null

GEOIP_TYPE = 1
GEOIP_RECORD_LEN = 3
GEOIP_COUNTRY_BEGIN = 16776960

#
# highly optimized lookup helpers
#
seekCountry = seekCountry3 = (ip32) ->

	offset = 0
	for depth in [31..0]
		pos = 6 * offset
		pos += 3 if ip32 & (1 << depth)
		offset = buffer[pos] + (buffer[pos + 1] << 8) + (buffer[pos + 2] << 16)
		return offset - GEOIP_COUNTRY_BEGIN if offset >= GEOIP_COUNTRY_BEGIN
	0

seekCountry4 = (ip32) ->

	offset = 0
	for depth in [31..0]
		pos = 8 * offset
		pos += 4 if ip32 & (1 << depth)
		offset = buffer[pos] + (buffer[pos + 1] << 8) + (buffer[pos + 2] << 16) + (buffer[pos + 3] << 24)
		return offset - GEOIP_COUNTRY_BEGIN if offset >= GEOIP_COUNTRY_BEGIN
	0

#
# given IPv4 address, return country id (if full is falsy) or full record
#
getLocation = (ipaddr, full) ->

	# convert IP to int32
	p = String(ipaddr).split('.')
	ip32 = +p[0] * 16777216 + +p[1] * 65536 + +p[2] * 256 + +p[3]

	# get country id
	id = seekCountry ip32
	#return 0 if id <= 0
	return undefined if id <= 0

	# if DB is not basic, seek to the location record
	if GEOIP_TYPE > 1
		offset = id + (2 * GEOIP_RECORD_LEN) * GEOIP_COUNTRY_BEGIN
		id = buffer[offset]
		#console.log id + GEOIP_COUNTRY_BEGIN, offset

	# return just id unless `full` is truthy
	return id unless full

	# compose country stuff
	record =
		id: id
		country_code: GEOIP_COUNTRY_CODES[id]
		country_code3: GEOIP_COUNTRY_CODES3[id]
		country_name: GEOIP_COUNTRY_NAMES[id]

	# mixin additional info, if available
	if GEOIP_TYPE > 1
		# region name
		b = e = offset + 1
		e++ while buffer[e]
		record.region_name = buffer.toString 'utf8', b, e
		# city name
		b = e = e + 1
		e++ while buffer[e]
		record.city_name = buffer.toString 'utf8', b, e
		# postal code
		b = e = e + 1
		e++ while buffer[e]
		record.postal_code = buffer.toString 'utf8', b, e
		# latitude and longitude
		b = e + 1
		n = buffer[b] + (buffer[b + 1] << 8) + (buffer[b + 2] << 16)
		b += 3
		record.latitude = (n/10000.0).toFixed(6) - 180
		n = buffer[b] + (buffer[b + 1] << 8) + (buffer[b + 2] << 16)
		b += 3
		record.longitude = (n/10000.0).toFixed(6) - 180
		# finer location, if available
		if GEOIP_TYPE is 2
			#if record.country_code is 'US'
			if id is 225 # US
				n = buffer[b] + (buffer[b + 1] << 8) + (buffer[b + 2] << 16)
				record.dma_code = record.metro_code = Math.floor n / 1000
				record.area_code = n % 1000

	#
	record

module.exports = (filename = "./GeoIP.dat") ->

	# load db
	buffer = require('fs').readFileSync filename
	buflen = buffer.length
	#
	console.log "DB #{filename} loaded (length = #{buflen})"

	# determine db type, offsets and record length
	for i in [0..19]
		pos = buflen - i - 3
		if buffer[pos] is 255 and buffer[pos + 1] is 255 and buffer[pos + 2] is 255
			GEOIP_TYPE = buffer[pos + 3]
			GEOIP_TYPE -= 105 if GEOIP_TYPE >= 106
			GEOIP_COUNTRY_BEGIN = 16700000 if GEOIP_TYPE is 7
			GEOIP_COUNTRY_BEGIN = 16000000 if GEOIP_TYPE is 3
			if GEOIP_TYPE in [2, 4, 5, 6, 9]
				# custom offset
				GEOIP_COUNTRY_BEGIN = buffer[pos + 4] + (buffer[pos + 5] << 8) + (buffer[pos + 6] << 16)
				# 4-byte record?
				if GEOIP_TYPE in [4, 5]
					GEOIP_RECORD_LEN = 4
					seekCountry = seekCountry4

	#console.log GEOIP_TYPE, GEOIP_COUNTRY_BEGIN, GEOIP_RECORD_LEN

	# export lookup function
	getLocation
