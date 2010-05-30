#ifndef __REC_AUDIO_FORMAT_MPG123_FORMAT__
#define __REC_AUDIO_FORMAT_MPG123_FORMAT__

#include "audio/audio_file_formats/juce_AudioFormat.h"
#include "rec/audio/format/mpg123/Mpg123.h"
#include "rec/audio/format/mpg123/Reader.h"
#include "rec/audio/format/mpg123/Writer.h"

namespace rec {
namespace audio {
namespace format {
namespace mpg123 {

class Format : public AudioFormat {
 public:
	Format() : AudioFormat(TRANS("MP3 Audio file"), getFileExtensions()) {}

	~Format() {}

  virtual AudioFormatReader* createReaderFor(InputStream* sourceStream,
                                             bool deleteStreamIfOpeningFails) {
    Reader* r = NULL;
    if (Reader::create(sourceStream, getFormatName(), &r)) {
      // TODO: error handling
      if (deleteStreamIfOpeningFails)
        delete sourceStream;
    }

    return r;
  }

	virtual AudioFormatWriter* createWriterFor(OutputStream* streamToWriteTo,
                                             double sampleRateToUse,
                                             unsigned int numberOfChannels,
                                             int bitsPerSample,
                                             const StringPairArray& metadata,
                                             int qualityOptionIndex) {
    Writer* w = NULL;
    if (getPossibleBitDepths().contains(bitsPerSample)) {
      Writer::create(streamToWriteTo, getFormatName(), sampleRateToUse, numberOfChannels,
                     bitsPerSample, metadata, qualityOptionIndex, &w);
      // TODO: error handling
    }
    return w;
  }

	virtual const Array<int> getPossibleSampleRates() { return getSampleRates(); }
	virtual const Array<int> getPossibleBitDepths() { return getBitDepths(); }

	virtual bool canDoStereo() { return true; }
	virtual bool canDoMono() { return true; }
	virtual bool isCompressed() { return true; }

	juce_UseDebuggingNewOperator

 private:
  DISALLOW_COPY_AND_ASSIGN(Format);
};

}  // namespace mpg123
}  // namespace format
}  // namespace audio
}  // namespace rec

#endif // __REC_AUDIO_FORMAT_MPG123_FORMAT__
