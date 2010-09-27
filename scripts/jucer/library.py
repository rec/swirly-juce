#!/usr/local/bin/python

# Representation of external libraries for Juce.
#
# Copyright (c) 2010 Tom Swirly
# This software is licensed under the terms of the artistic license,
# http://www.opensource.org/licenses/artistic-license-2.0.php
#
# Essentially, you can do as you please with it.  Have fun!


class Library:
  """Represents an external library or facility within the Jucer."""

  def __init__(self, name, **kwds):
    """Construct with a name, and the following keywords and default:
      header_only=False,      # Is this library only used for its headers?
      libraries=[self.name],  # A list of library names for this library.
      include='/include',     # The subdirectory for finding include files.
      relative=True,          # Is this path relative to the config directory?
    """
    self.name = name
    self.kwds = kwds


  def get(self, **kwds):
    for k, v in kwds.iteritems():
      return self.kwds.get(k, v)

  def link(self, root, config):
    if self.get(header_only=False):
      return False

    libraries = ['-l%s' % lib for lib in self.get(libraries=[self.name])]
    if not self.get(system=False):
      libraries.insert(0, '-L%s/%s/%s/lib' % (root, config, self.name))

    return ' '.join(libraries)


  def header(self, root, config):
    if not self.get(header_needed=True):
      return ''

    elif not self.get(relative=True):
      return self.name

    elif self.get(header_only=False):
      return '%s/%s' % (root, self.name)

    else:
      return '%s/%s/%s%s' % (root, config, self.name,
                             self.get(include='/include'))


def getHeaders(libraries, root, config):
  return filter(None, (i.header(root, config) for i in libraries))


def getLinks(libraries, root, config):
  return filter(None, (i.link(root, config) for i in libraries))




