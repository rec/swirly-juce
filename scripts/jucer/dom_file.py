#!/usr/local/bin/python
#
# Create elements for Jucer-style DOM trees.
#
# Copyright (c) 2010 Tom Swirly
# This software is licensed under the terms of the artistic license,
# http://www.opensource.org/licenses/artistic-license-2.0.php
#
# Essentially, you can do as you please with it.  Have fun!


import random
import xml.dom.minidom

class DomFile:
  """Creates XML DOM elements that have Jucer-style 9-character random IDs."""

  CHARS = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ'
  """Legal characters for Jucer ID."""

  ID_SIZE = 9
  """Length of Jucer ID."""

  def __init__(self, filename):
    self.dom = xml.dom.minidom.parse(open(filename))
    self.documentElement = self.dom.documentElement


  def create(self, xmlName, **attributes):
    """Create a new named XML element with a JUCER ID and attributes."""

    element = self.dom.createElement(xmlName)

    id = ''.join(random.choice(DomFile.CHARS) for i in xrange(DomFile.ID_SIZE))
    element.setAttribute('id', id)

    for k, v in attributes.iteritems():
      element.setAttribute(k, v)

    return element


  def element(self, *tag_names):
    """Get an element in the DOM tree by a sequence of tag_names.  If there are
       multiple tag names with the same tag, selects the first at each stage."""

    node = self.documentElement

    for n in tag_names:
      node = node.getElementsByTagName(n)[0]

    return node
