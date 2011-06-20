#!/usr/local/bin/python

# Copyright (c) 2011 Tom Swirly
#
# This software is licensed under the terms of the artistic license,
# http://www.opensource.org/licenses/artistic-license-2.0.php
#
# Essentially, you can do as you please with it.  Have fun!

#
# Return the root directory for our workspace.
#

import os.path
import sys

LEVELS = 3

def _getRoot():
  f = os.path.abspath(sys.argv[0])
  for i in range(LEVELS):
    f = os.path.split(f)[0]

  return f

_ROOT = _getRoot()

def root():
  return _ROOT
