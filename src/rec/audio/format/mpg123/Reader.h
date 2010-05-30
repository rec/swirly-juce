#ifndef __REC_AUDIO_FORMAT_MPG123_READER__
#define __REC_AUDIO_FORMAT_MPG123_READER__

#include <stdlib.h>
#include <sys/types.h>

#include "juce_amalgamated.h"
#include "audio/audio_file_formats/juce_AudioFormatReader.h"
#include "rec/audio/format/mpg123/Copier.h"
#include "rec/audio/format/mpg123/Mpg123.h"
#include "rec/base/basictypes.h"

namespace rec {
namespace audio {
namespace format {
namespace mpg123 {

#include "libmpg123/mpg123.h"

class Reader : public AudioFormatReader {
 public:
  static Error create(InputStream* const in, const String& name, Reader** r);

	~Reader();
	virtual bool readSamples(int** destSamples, int numDestChannels,
                           int startOffsetInDestBuffer,
                           int64 startSampleInFile, int numSamples);

  juce_UseDebuggingNewOperator

 private:
	mpg123_handle* mh_;
  void* buffer_;
  size_t size_, allocated_;
  Copier copier_;
  int bytesPerSample_;

  Reader(InputStream* in, const String& name, mpg123_handle* mh);

  DISALLOW_COPY_AND_ASSIGN(Reader);
};

}  // namespace mpg123
}  // namespace format
}  // namespace audio
}  // namespace rec

#endif  // __REC_AUDIO_FORMAT_MPG123_READER__
