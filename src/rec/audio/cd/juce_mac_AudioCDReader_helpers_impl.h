// This will be a .cpp file once I know how to do this for one platform.

// Apple's plist format shows a profound lack of understanding of how to put
// together an XML document.  Here's an example for a CD, some parts omitted.
//
// <plist version="1.0">
// <dict>
//
// [other key/data pairs here]
//
// 	<key>Sessions</key>
// 	<array>
// 		<dict>
//
//    [other key/data pairs here]
//
// 			<key>Track Array</key>
// 			<array>
// 				<dict>
//
//          [other key/data pairs here]
//
// 					<key>Start Block</key>
// 					<integer>150</integer>
// 				</dict>
//
// 				<dict>
//
//          [other key/data pairs here]
//
// 					<key>Start Block</key>
// 					<integer>2029</integer>
// 				</dict>
//
//        [more dicts here]
//
// 			</array>
// 		</dict>
// 	</array>
// </dict>
// </plist>

inline const XmlElement* getElementForKey(const XmlElement& xml,
                                          const String& key) {
    forEachXmlChildElementWithTagName(xml, child, "key")
    {
      if (child->getAllSubText() == key)
          return child->getNextElement();
    }
    return NULL;
}

inline int getIntValueForKey(const XmlElement& xml,
                             const String& key,
                             int dflt) {
    const XmlElement* block = getElementForKey(xml, key);
    return block ? int(strtol(block->getAllSubText().toCString(), NULL, 10)) : dflt;
}

inline const XmlElement* getFirstNamedElement(const XmlElement& xml, const String& name) {
    forEachXmlChildElementWithTagName(xml, child, name)
         return child;
    return NULL;
}

// Get the track offsets for a CD given an XmlElement representing its TOC.Plist.
// Returns NULL on success, otherwise a const char* representing an error.
inline const char* getTrackOffsets(XmlDocument* xmlDocument,
                                   Array<int>* offsets) {
    ScopedPointer<XmlElement> xml(xmlDocument->getDocumentElement());
    if (!xml)
        return "Couldn't parse XML in file";

    const XmlElement* dict = getFirstNamedElement(*xml, "dict");
    if (!dict)
        return "Couldn't get top level dictionary";

    const XmlElement* sessions = getElementForKey(*dict, "Sessions");
    if (!sessions)
        return "Couldn't find sessions key";

    const XmlElement* session = sessions->getFirstChildElement();
    if (!session)
        return "Couldn't find first session";

    int leadOut = getIntValueForKey(*session, "Leadout Block");
    if (leadOut < 0)
        return "Couldn't find Leadout Block";

    const XmlElement* trackArray = getElementForKey(*session, "Track Array");
    if (!trackArray)
        return "Couldn't find Track Array";

    forEachXmlChildElement(*trackArray, track)
    {
        int trackValue = getIntValueForKey(*track, "Start Block");
        if (trackValue < 0)
            return "Couldn't find Start Block in the track";
        offsets->add(trackValue * AudioCDReader::SAMPLES_PER_FRAME);
    }

    offsets->add(leadOut * AudioCDReader::SAMPLES_PER_FRAME);
    return NULL;
}

// Get the track offsets for a CD given a file representing the root volume for
// that CD.
//
// Returns NULL on success, otherwise a const char* representing an error.
inline const char* getTrackOffsets(const File& volume,
                                   Array<int>* offsets) {
    File toc = volume.getChildFile(".TOC.plist");
    if (!toc.exists())
        return "No TOC file";

    XmlDocument doc(toc);
    return getTrackOffsets(&doc, offsets);
}
