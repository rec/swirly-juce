#include <stdlib.h>
#include <sys/types.h>

#include "rec/audio/format/mpg123/Reader.h"
#include "rec/audio/format/mpg123/NewHandle.h"
#include "rec/audio/format/mpg123/Format.h"

namespace rec {
namespace audio {
namespace format {
namespace mpg123 {

#include "libmpg123/mpg123.h"

Reader::Reader(InputStream* in, const String& formatName, mpg123_handle* mh)
  : AudioFormatReader(in, formatName),
    mh_(mh),
    buffer_(NULL),
    size_(0),
    allocated_(0) {
}

Reader::~Reader()	{
  mpg123_close(mh_);
  mpg123_delete(mh_);
  free(buffer_);
}

Error Reader::create(InputStream* in, const String& name, Reader** reader) {
  Error e;
  mpg123_handle* mh = NULL;
  Reader* r = NULL;

  long sampleRate;
  int numChannels, encoding;

  if ((e = newHandle(in, &mh)) ||
      (r = new Reader(in, name, mh)) ||
      (e = mpg123_getformat(mh, &sampleRate, &numChannels, &encoding)) ||
      !(r->bitsPerSample = getBitsPerSample(encoding)) ||
      !(r->copier_ = getCopier(encoding)) ||
      numChannels > MPG123_STEREO) {
    mpg123_delete(mh);
    delete r;

    return e ? e : MPG123_ERR;
  }

  r->sampleRate = int(sampleRate);
  r->lengthInSamples = mpg123_length(mh);
  r->usesFloatingPointData = (encoding & MPG123_ENC_FLOAT);
  r->numChannels = numChannels;

  getMp3Tags(mh, &r->metadataValues);  // TODO: check errors and...?

  *reader = r;
  return MPG123_OK;
}

bool Reader::readSamples(int** dest, int destChannels, int destOffset,
                         int64 startSampleInFile, int numSamples) {
  if (mpg123_seek(mh_, startSampleInFile, SEEK_SET))
    return false;

  int64 bytesPerSample = this->bitsPerSample / 8;
  size_ = numSamples * numChannels * bytesPerSample;

  if (allocated_ < size_) {
    if (buffer_)
      free(buffer_);

    buffer_ = malloc(size_);
    allocated_ = size_;
  }

  size_t bytesCopied;
  Error e = mpg123_read(mh_, (uchar*) buffer_, size_, &bytesCopied);
  if (e != MPG123_DONE && e != MPG123_OK)
    return false;

  int64 sourceSize = bytesCopied / (bytesPerSample);
  copier_(dest, destChannels, destOffset, buffer_, numChannels, sourceSize);
  return (bytesCopied == size_);
}

}  // namespace mpg123
}  // namespace format
}  // namespace audio
}  // namespace rec

