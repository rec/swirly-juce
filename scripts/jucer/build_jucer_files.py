#!/usr/local/bin/python
#
# Handle .jucer files!
#
# Copyright (c) 2010 Tom Swirly
# This software is licensed under the terms of the artistic license,
# http://www.opensource.org/licenses/artistic-license-2.0.php
#
# Essentially, you can do as you please with it.  Have fun!

import os

import jucer
import library


LIBRARIES = (
  library.Library('rec/src', header_only=True),
  library.Library('rec/genfiles/proto', header_only=True),
  library.Library('juce', header_only=True),

  library.Library('libcddb', libraries=['cddb']),
  library.Library('gflags'),
  library.Library('glog'),
  library.Library('gtest', libraries=['gtest', 'gtest_main']),
  library.Library('iconv', header_needed=False),
  library.Library('mpg123', include=''),
  library.Library('protobuf'),

  library.Library('../..', header_only=True, relative=False),
)


def build(*names):
  for name in names:
    root = os.getenv('ROOT')
    doc = '%s/rec/projects/%s/%s.jucer' % (root, name, name)
    results = jucer.Jucer(doc, 'test' in name, LIBRARIES, root).toxml()
    open(doc, 'w').write(results)
    print 'Written', doc


build('console', 'slow', 'tests')
