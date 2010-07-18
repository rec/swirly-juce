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
        offsets->add(trackValue);
    }

    offsets->add(leadOut);
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

// Returns the sum of the decimal digits in n.
inline int sumOfDigits(int n) {
    int sum = 0;
    while (n > 0) {
        sum = sum + (n % 10);
        n = n / 10;
    }
    return sum;
}

#define convertEarly false

// Compute the CDDB id from an array of offsets.
//
// convertEarly models the fact that when you compute the total length, you can
// either take the difference between first and last frames and then convert to
// seconds, or you can convert the first and last frame to seconds and take the
// difference, which I call "convertEarly".
//
// Conceptually, "convertEarly" isn't right - you want to do your calculations
// as soon as possible - but
//

// My educated guess is that 37 times in 75 these two computations
// will differ by 1 second.
//
// http://www.cs.princeton.edu/introcs/51data/CDDB.java.html
inline int getCDDBId(const Array<int>& offsets) {
    // One CD frame is 1/75 of a second.
    static const int FPS = 75;
    int checksum = 0;
    int tracks = offsets.size() - 1;

    for (int i = 0; i < tracks; ++i)
      checksum += sumOfDigits(offsets[i] / FPS);

    int length = convertEarly
      ? (offsets[tracks] / FPS - offsets[0] / FPS)
      : ((offsets[tracks] - offsets[0]) / FPS);

    // CCLLLLTT: checksum, length, tracks
    return ((checksum & 0xFF) << 24) | (length << 8) | tracks;
}

// Compute the CDDB id from a volume with a .TOC.plist.
// Returns NULL on success, otherwise a const char* representing an error.
inline const char* getCDDBId(const File& volume, int* id) {
    Array<int> offsets;
    if (const char* error = getTrackOffsets(volume, &offsets))
      return error;

    *id = getCDDBId(offsets);
    return NULL;
}
