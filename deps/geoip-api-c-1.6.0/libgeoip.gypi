{
  'variables': { 'target_arch%': 'ia32' },

  'target_defaults': {
    'default_configuration': 'Debug',
    'configurations': {
      'Debug': {
        'defines': [ 'DEBUG', '_DEBUG' ],
        'msvs_settings': {
          'VCCLCompilerTool': {
            'RuntimeLibrary': 1, # static debug
          },
        },
      },
      'Release': {
        'defines': [ 'NDEBUG' ],
        'msvs_settings': {
          'VCCLCompilerTool': {
            'RuntimeLibrary': 0, # static release
          },
        },
      }
    },
    'msvs_settings': {
      'VCLinkerTool': {
        'GenerateDebugInformation': 'true',
      },
    },
    'include_dirs': [
      'libGeoIP',
      'libGeoIP/.deps'
    ],
    'defines': [
      'PACKAGE_NAME="GeoIP"',
      'PACKAGE_TARNAME="GeoIP"',
      'PACKAGE_VERSION="1.6.0"',
      'PACKAGE_STRING="GeoIP 1.6.0"',
      'PACKAGE_BUGREPORT="support@maxmind.com"',
      'PACKAGE_URL=""',
      'STDC_HEADERS=1',
      'HAVE_SYS_TYPES_H=1',
      'HAVE_SYS_STAT_H=1',
      'HAVE_STDLIB_H=1',
      'HAVE_STRING_H=1',
      'HAVE_MEMORY_H=1',
      'HAVE_STRINGS_H=1',
      'HAVE_INTTYPES_H=1',
      'HAVE_STDINT_H=1',
      'HAVE_UNISTD_H=1',
      '__EXTENSIONS__=1',
      '_ALL_SOURCE=1',
      '_GNU_SOURCE=1',
      '_POSIX_PTHREAD_SEMANTICS=1',
      '_TANDEM_SOURCE=1',
      'PACKAGE="GeoIP"',
      'VERSION="1.6.0"',
      'HAVE_DLFCN_H=1',
      'LT_OBJDIR=".libs/"',
      'HAVE_USHORT_TYPEDEF=1',
      'LITTLE_ENDIAN_HOST=1',
      'HAVE_STDINT_H=1',
      'HAVE_ZLIB_H=1',
      'HAVE_GETOPT=1',
      'HAVE_GETTIMEOFDAY=1',
      'HAVE_PREAD=1',
      'HAVE_VASPRINTF=1',
      'HAVE_VSNPRINTF=1',
      'HAVE_VSPRINTF=1',
      'HAVE_GETHOSTBYNAME=1',
      'HAVE_MEMSET=1',
      'HAVE_SOCKET=1',
      'HAVE_STRCHR=1',
      'HAVE_STRDUP=1',
      'HAVE_STRSTR=1',
      'HAVE_STDLIB_H=1',
      'HAVE_MALLOC=1',
      'HAVE_STDLIB_H=1',
      'HAVE_REALLOC=1',
      'GEOIPDATADIR="/tmp/"'
      ],
    'conditions': [
      ['OS=="win"', {
        'defines': [
        ]
      }]
    ],
  },

  'targets': [{
    'target_name': 'libgeoip',
    'product_prefix': 'lib',
    'type': 'static_library',
    'sources': [
      'libGeoIP/GeoIP.c',
      'libGeoIP/GeoIPCity.c',
      'libGeoIP/timeZone.c',
      'libGeoIP/regionName.c',
      'libGeoIP/GeoIP_deprecated.c'
      ]
  }]
}
