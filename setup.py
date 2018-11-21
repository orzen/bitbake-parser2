#!/usr/bin/python

import os
import re
from distutils.core import setup, Extension

SOURCE_PATTERN = re.compile("\S+\.c")

def locate_filenames(filename):
    filenames = []

    for root, dirs, files in os.walk('.'):
        if filename in files:
            filenames.append((root, filename))

    return filenames


def find_extensions():
    def find_sources(filename):
        with open(filename) as file:
            lines = file.read()
            return re.findall(SOURCE_PATTERN, lines)

    extensions = []
    makefiles = locate_filenames('Makefile')

    for path, makefile in makefiles:
            sources = find_sources(os.path.join(path, makefile))

            path = path.strip('./')
            sources = [ os.path.join(path, source) for source in sources ]

            extension = Extension(path.strip('./'),
                                  sources)
            extensions.append(extension)

    return extensions



version = '0.0.1'

if __name__ == '__main__':
    ext = find_extensions()
    print("EXT '{}'".format(str(ext)))

#parser = Extension('cbitbake/parser',
#                   sources = ['cbitbake'])
#
#
#setup(name = 'cbitbake',
#      version = version,
#      description = 'C implementation of bitbake parser',
#      ext_modules = [])
