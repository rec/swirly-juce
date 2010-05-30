#ifndef __REC_AUDIO_FORMAT_MPG123_WRITER__
#define __REC_AUDIO_FORMAT_MPG123_WRITER__

#include "audio/audio_file_formats/juce_AudioFormatWriter.h"

namespace rec {
namespace audio {
namespace format {
namespace mpg123 {

class Writer : public AudioFormatWriter {
 public:
  static Error create(OutputStream* streamToWriteTo,
                      const String& formatName,
                      double sampleRate,
                      unsigned int numberOfChannels,
                      int bitsPerSample,
                      const StringPairArray& metadata,
                      int qualityOptionIndex,
                      Writer** writer) {
    return -1;
  }

  bool write(const int** samplesToWrite, int numSamples) { return false; }

  juce_UseDebuggingNewOperator
};

}  // namespace mpg123
}  // namespace format
}  // namespace audio
}  // namespace rec

#endif  // __REC_AUDIO_FORMAT_MPG123_WRITER__

