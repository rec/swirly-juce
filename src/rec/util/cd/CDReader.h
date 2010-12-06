#ifndef __REC_UTIL_CD_CDREADER__
#define __REC_UTIL_CD_CDREADER__

#include "rec/base/base.h"

namespace rec {
namespace util {
namespace cd {

AudioCDReader* getCDReader(const string& idString);
int getAudioTrackIndex(const AudioCDReader& reader, int track);
AudioFormatReader* createCDTrackReader(const string& idString, int track);
int getAudioTrackCount(const AudioCDReader& reader);

}  // namespace cd
}  // namespace util
}  // namespace rec

#endif  // __REC_UTIL_CD_CDREADER__
