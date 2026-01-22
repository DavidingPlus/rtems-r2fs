#
# Hello world Waf script
#

from __future__ import print_function

rtems_version = "6"

try:
    import rtems_waf.rtems as rtems
except ImportError:
    print('error: no rtems_waf git submodule')
    import sys
    sys.exit(1)

def init(ctx):
    rtems.init(ctx, version=rtems_version, long_commands=True)

def bsp_configure(conf, arch_bsp):
    # Add BSP specific configuration checks
    pass

def options(opt):
    rtems.options(opt)

def configure(conf):
    rtems.configure(conf, bsp_configure=bsp_configure)

def build(bld):
    rtems.build(bld)

    all_sources = bld.path.ant_glob('src/**/*.c', excl='**/test_*.c **/*_test.c') + bld.path.ant_glob('test/**/*.c') + bld.path.ant_glob('third_party/**/*.c')

    include_paths = [
        bld.path.find_dir('src').abspath(),
        bld.path.find_dir('test').abspath(),
        bld.path.find_dir('third_party').abspath()
    ]

    bld(features = 'c cprogram',
        target = 'main.exe',
        cflags = '-g',
        includes = include_paths,
        source = ['configure.c'] + all_sources
    )
