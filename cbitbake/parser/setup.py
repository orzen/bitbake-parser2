import os
import re
import subprocess
from distutils.core import setup, Extension

def locate_files(path):
    sources = []
    makefile = False

    for root, dirs, files in os.walk(path):
        sources = [f for f in files if f.endswith('.c')]
        makefile = True if "Makefile" in files else False

    return makefile, sources

def makefile_pkg_configs(file_path):
    with open(file_path) as f:
        re.match('\s*PKGS\s*=\s*(.*)', f.read())
        return m.group(1).split(' ') if m else None

def pkg_config_cflags(pkg_configs):
    return [subprocess.check_output(['pkg-config', '--cflags', pkg_config]).decode('utf-8').strip('\n') for pkg_config in pkg_configs]

def pkg_config_libs(pkg_configs):
    return [subprocess.check_output(['pkg-config', '--libs', pkg_config]).decode('utf-8').strip('\n') for pkg_config in pkg_configs]

def include_dirs_from_cflags(cflags):
    return [flag[2:] for flag in cflags if flag.startswith('-I')]

def libraries_from_libs(libs):
    return [lib[2:] for lib in libs if lib.startswith('-l')]

def construct_extension(path):
    cflags = []
    libs = []

    makefile, sources = locate_files(path)

    if makefile:
        pkg_configs = makefile_pkg_configs(os.path.join(path, 'Makefile'))

        cflags = pkg_config_cflags(pkg_configs)
        libs = pkg_config_libs(pkg_configs)

    return Extension(os.path.basename(path),
                     include_dirs = include_dirs_from_cflags(cflags),
                     libraries = libraries_from_libs(libs),
                     sources = sources)


version = '0.0.1'

#parser = Extension('parser',
#                   include_dirs = [
#                       '/usr/include/glib-2.0',
#                       '/usr/lib/x86_64-linux-gnu/glib-2.0/include'
#                       ],
#                   libraries = ['glib-2.0'],
#                   sources = [
#                       'args.c',
#                       'files.c',
#                       'lex.yy.c',
#                       'main.c',
#                       'parser.tab.c',
#                       'python_bindings.c',
#                       'python_utils.c',
#                       'pyobj_d.c',
#                       'pyobj_match_mock.c',
#                       'utils.c'
#                       ])

parser = construct_extension(os.path.abspath('.'))

setup(name = 'cparser',
      version = version,
      description = 'C implementation of bitbake parser',
      ext_modules = [parser])
