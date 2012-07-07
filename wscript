import Options
import Utils
import os
from os import popen, unlink, symlink, getcwd
from os import name as platform
from os.path import exists
from shutil import rmtree

srcdir = "."
blddir = "build"
VERSION = "0.1"

# Bundled geoip library
BUNDLED_GEOIP_VERSION = '1.4.8'
BUNDLED_GEOIP = 'GeoIP-%s' % BUNDLED_GEOIP_VERSION
BUNDLED_GEOIP_TAR = 'GeoIP-%s.tar.gz' % BUNDLED_GEOIP_VERSION

def set_options(opt):
  opt.tool_options("compiler_cxx")
  opt.add_option( '--geoip'
                , action='store'
                , default=None
                , help='Directory prefix containing geoip "lib" and "include" files'
                , dest='shared_geoip_dir'
                )

  opt.add_option( '--shared-geoip'
                , action='store_true'
                , default=False
                , help='Build dynamically against external install of geoip (default False - build uses internal copy)'
                , dest='shared_geoip'
                )

def configure_geoip():
    if not Options.options.shared_geoip:
        Utils.pprint('GREEN','configuring internal geoip dep')
        os.chdir('deps')
        if not os.fs.exists(BUNDLED_GEOIP):
            os.system('tar xzf %s' % BUNDLED_GEOIP_TAR)
        os.chdir(BUNDLED_GEOIP)
        cxxflags = ''
        if os.environ.has_key('CFLAGS'):
            cxxflags += os.environ['CFLAGS']
            cxxflags += ' '
        if os.environ.has_key('CXXFLAGS'):
            cxxflags += os.environ['CXXFLAGS']
        # LINKFLAGS appear to be picked up automatically...
        os.system("CFLAGS='%s -fPIC -O3 -DNDEBUG -Wall' ./configure --disable-dependency-tracking --enable-static --disable-shared" % cxxflags)
        os.chdir('../../')

def configure(conf):
    conf.check_tool("compiler_cxx")
    conf.check_tool("node_addon")
    configure_geoip()

    geoip_includes = []
    geoip_libpath  = []
    auto_configured = False
    o = Options.options
    geoip_dir = o.shared_geoip_dir
    shared_geoip = o.shared_geoip
    
    if geoip_dir:
        norm_path = os.path.realpath(geoip_dir)
        if norm_path.endswith('lib') or norm_path.endswith('include'):
            norm_path = os.path.dirname(norm_path)
        
        geoip_includes = [os.path.join('%s' % norm_path,'include'),
                           os.path.join('%s' % norm_path,'lib/geoip/include')
                          ]
        geoip_libpath  = [os.path.join('%s' % norm_path,'lib')]
    
    elif shared_geoip:
        if not auto_configured:
            # reasonable defaults for searching
            geoip_includes = ['/usr/local/include',
                               '/usr/local/lib/geoip/include',
                               '/usr/include',
                               '/usr/lib/geoip/include',
                               ]
            geoip_libpath  = ['/usr/local/lib','/usr/lib']
    
    elif not shared_geoip:
        auto_configured = True
        geoip_includes = ['-I../deps/%s/libGeoIP' % BUNDLED_GEOIP]
        geoip_libpath  = ['-L../deps/%s/libGeoIP/.libs' % BUNDLED_GEOIP]
        
    if not auto_configured:
        if not conf.check_cxx(lib='geoip', header_name='GeoIP.h',
                              uselib_store='GEOIP',
                              includes=geoip_includes,
                              libpath=geoip_libpath):
            conf.fatal("\n\n  Cannot find libGeoIP, required for GeoIP,\n  please install from:\n  'http://nih.at/geoip/'\n  (see README.md for more info)\n")
        else:
            Utils.pprint('GREEN', 'Sweet, found viable libGeoIP dependency')
    
        # strip paths that don't exist, turn into proper flags
        new_inc = []
        for i in geoip_includes:
           if os.fs.exists(i):
               new_inc.append('-I%s' % i)
        geoip_includes = new_inc
    
        new_inc = []
        for i in geoip_libpath:
           if os.fs.exists(i):
               new_inc.append('-L%s' % i)
        geoip_libpath = new_inc

    linkflags = []
    if os.environ.has_key('LINKFLAGS'):
        linkflags = os.environ['LINKFLAGS'].split(' ')
    linkflags.extend(geoip_libpath)
    if shared_geoip:
        if '-lGeoIP' not in linkflags:
            linkflags.append('-lGeoIP')
    elif Options.platform == 'darwin':
        linkflags.append('-Wl,-search_paths_first')
    
    conf.env.append_value("LINKFLAGS", linkflags)
    
    cxxflags = geoip_includes

    conf.env.append_value("CXXFLAGS", cxxflags)
    
    #ldflags = []
    #conf.env.append_value("LDFLAGS", ldflags)

def build_geoip():
    if not Options.options.shared_geoip:
        os.chdir('deps/%s' % BUNDLED_GEOIP)
        os.system('make')
        os.chdir('../../')

def build(bld):
  obj = bld.new_task_gen("cxx", "shlib", "node_addon")
  build_geoip()
  obj.uselib = "GEOIP"
  obj.target = "geoip"
  obj.source = ['src/init.cc', 
                'src/netspeed.cc', 
                'src/country6.cc', 
                'src/country.cc',
                'src/region.cc',
                'src/city6.cc',
                'src/city.cc', 
                'src/org.cc', 
                'src/utils.cc', 
                'src/global.cc']

  obj.lib = ["GeoIP"]

def clean(bld):
    if not Options.options.shared_geoip:
      rmtree('deps/%s' % BUNDLED_GEOIP, ignore_errors = True)
      rmtree('build', ignore_errors = True)

def link(bld):
  # HACK to get binding.node out of build directory.
  # better way to do this?
  if Options.commands['clean']:
    if exists(getcwd() + '/geoip.node'): unlink('geoip.node')
  else:
    if exists(getcwd() + '/build/default/geoip.node') and not exists(getcwd() + 'geoip.node'):
      symlink(getcwd()+'/build/default/geoip.node', getcwd() + 'geoip.node')
