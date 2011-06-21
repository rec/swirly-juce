#!/usr/local/bin/python

# Copyright (c) 2011 Tom Swirly
#
# This software is licensed under the terms of the artistic license,
# http://www.opensource.org/licenses/artistic-license-2.0.php
#
# Essentially, you can do as you please with it.  Have fun!

#
# Handle source control and providers.

import collections

SourceTuple = collections.namedtuple('SourceTuple', ['update', 'download'])

SOURCES = {
  'sourceforge': {
    'git': SourceTuple(
      update='git pull origin main',
      download='git clone --depth 1 git://{name}.git.sourceforge.net/gitroot/{name}/{name}'
      ),
    },

  'googlecode': {
    'svn': SourceTuple(
      download='svn checkout http://{name}.googlecode.com/svn/trunk {name}',
      update='svn update {name}'
      )
    }
  }

def getSourceCommand(command, config):
  return getattr(SOURCES[config['provider']][config['source_control']], command).format(**config)

