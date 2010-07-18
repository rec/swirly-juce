#include <stdio.h>
#include <string.h>

#include <gtest/gtest.h>

#include "juce_amalgamated.h"

#include "rec/base/scoped_ptr.h"
#include "native/mac/juce_mac_AudioCDReader_helpers.h"

#ifdef __JUCE_MAC_AUDIOCDREADER_HELPERS_JUCEHEADER__

// Don't run this test unless we have the upated JUCE.

TEST(AudioCDReaderHelper, getElementForKey) {
  XmlDocument doc("<dict><key>foo</key><value>bar</value></dict>");
  scoped_ptr<XmlElement> xml(doc.getDocumentElement());
  EXPECT_TRUE(!getElementForKey(*xml, "bar"));
  EXPECT_EQ(getElementForKey(*xml, "foo")->getFirstChildElement()->getText(),
            "bar");
}

TEST(AudioCDReaderHelper, getIntValueForKey) {
  XmlDocument doc("<dict><key>foo</key><value>123</value></dict>");
  scoped_ptr<XmlElement> xml(doc.getDocumentElement());
  EXPECT_EQ(getIntValueForKey(*xml, "bar"), -1);
  EXPECT_EQ(getIntValueForKey(*xml, "foo"), 123);
}

namespace {

// This CD is Kate Bush's "Never Forever".  Interestingly enough, there are
// half-a-dozen versions of this album, only different by a few seconds on one
// track or another.
//
// http://www.freedb.org/freedb/rock/9608bd0b is the specific CD used here.

const int TRACK_COUNT = 11;

// The offset of each track from the start, in frames.
int TRACK_LENGTHS[TRACK_COUNT + 1] = {
    183,
    15240,
    28113,
    44215,
    61385,
    80298,
    99478,
    114173,
    126925,
    130820,
    144218,
    168953
};

const int CDDB_ID = /* 0x9608bd0b; //*/ 0x8a08ca0b;

}

TEST(AudioCDReaderHelper, getTrackOffsets) {
  Array<int> offsets;
  EXPECT_STREQ(NULL, getTrackOffsets(File("../../../data"), &offsets));

  EXPECT_EQ(offsets.size(), TRACK_COUNT + 1);

  for (int i = 0; i < TRACK_COUNT; ++i)
    EXPECT_EQ(offsets[i], TRACK_LENGTHS[i]);
}

TEST(AudioCDReaderHelper, CDDBIdComputation) {
  int id;
  EXPECT_STREQ(NULL, getCDDBId(File("../../../data"), &id));
  EXPECT_EQ(CDDB_ID, id) << std::hex << CDDB_ID << ", " << id
                         << "\n" << std::dec;
}

TEST(AudioCDReaderHelper, CDDBIdEndToEnd) {
  int id = getCDDBId(Array<int>(TRACK_LENGTHS, TRACK_COUNT + 1));
  EXPECT_EQ(CDDB_ID, id) << std::hex << CDDB_ID << ", " << id
                         << "\n" << std::dec;
}

#endif
