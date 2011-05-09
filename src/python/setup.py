#!/usr/bin/env python

from distutils.core import setup, Extension

opt = ['-Wall']

setup(name = 'motion',
      version = '0.1',
      author='Guillaume Roguez',
      description = 'Python wrapper for the MOTION library',
      url='http://www.yomgui.fr/yiki/doku.php/dev:sfm:start',
      platforms=['morphos', 'linux'],
      ext_modules = [Extension('motion',
                               ['module.c'],
                               include_dirs = ['..', '../matchers', '../detectors'],
                               library_dirs = ['..'],
                               libraries = ['motion', 'png'],
                               extra_compile_args = opt)],
      )
