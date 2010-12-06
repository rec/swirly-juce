#include <glog/logging.h>

#include "rec/util/cd/CDReader.h"


using namespace juce;

namespace rec {
namespace util {
namespace cd {

AudioCDReader* getCDReader(const string& idString) {
  int id = String(idString.c_str()).getHexValue32();
  StringArray names = AudioCDReader::getAvailableCDNames();
  int size = names.size();
  for (int i = 0; i < size; ++i) {
    scoped_ptr<AudioCDReader> reader(AudioCDReader::createReaderForCD(i));
    if (!reader)
      LOG(ERROR) << "Couldn't create reader for " << names[i].toCString();
    else if (reader->getCDDBId() == id)
      return reader.transfer();
  }
  LOG(ERROR) << "Couldn't find an AudioCDReader for ID " << id;
  return NULL;
}

int getAudioTrackIndex(const AudioCDReader& reader, int audioTrack) {
  for (int i = 0, audioTracks = 0; i < reader.getNumTracks(); ++i) {
    if (reader.isTrackAudio(i) && audioTracks++ == audioTrack)
      return i;
  }
  return -1;
}

int getAudioTrackCount(const AudioCDReader& reader) {
  int audioTracks = 0;
  for (int i = 0; i < reader.getNumTracks(); ++i) {
    if (reader.isTrackAudio(i))
        ++audioTracks;
  }
  return audioTracks;
}

AudioFormatReader* createCDTrackReader(const string& idString, int track) {
  scoped_ptr<AudioCDReader> reader(getCDReader(idString));
  if (!reader) {
    LOG(ERROR) << "Couldn't create reader for " << idString;
    return NULL;
  }

  int trackIndex = getAudioTrackIndex(*reader, track);
  if (trackIndex == -1) {
    LOG(ERROR) << "No track " << track << " in " << idString;
    return NULL;
  }

  int begin = reader->getPositionOfTrackStart(trackIndex);
  int end = reader->getPositionOfTrackStart(trackIndex + 1);
  return new AudioSubsectionReader(reader.transfer(), begin, end - begin, true);
}

}  // namespace cd
}  // namespace util
}  // namespace rec
