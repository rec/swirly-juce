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

Error Reader::create(InputStream* in, const String& name, Reader** r) {
  mpg123_handle* mh;
  if (Error e = newHandle(in, &mh))
    return e;

  long sampleRate;
  int encoding;
  int numChannels;
  if (Error e = mpg123_getformat(mh, &sampleRate, &numChannels, &encoding)) {
    mpg123_delete(mh);
    return e;
  }

  int bitsPerSample = getBitsPerSample(encoding);
  Copier copier = getCopier(encoding);

  if (!(bitsPerSample && copier && numChannels <= MPG123_STEREO)) {
    mpg123_delete(mh);
    return MPG123_ERR;
  }

  *r = new Reader(in, name, mh);

  (*r)->copier_ = copier;
  (*r)->sampleRate = int(sampleRate);
  (*r)->bitsPerSample = bitsPerSample;
  (*r)->bytesPerSample_ = bitsPerSample / 8;
  (*r)->numChannels = numChannels;
  (*r)->lengthInSamples = mpg123_length(mh);
  (*r)->usesFloatingPointData = (encoding & MPG123_ENC_FLOAT);
  getMp3Tags(mh, &(*r)->metadataValues);  // TODO: check errors and...?

  return MPG123_OK;
}

bool Reader::readSamples(int** dest, int destChannels, int destOffset,
                         int64 startSampleInFile, int numSamples) {
  if (mpg123_seek(mh_, startSampleInFile, SEEK_SET))
    return false;

  size_ = numSamples * numChannels * bytesPerSample_;

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

  int64 sourceSize = bytesCopied / bytesPerSample_;
  copier_(dest, destChannels, destOffset, buffer_, numChannels, sourceSize);
  return (bytesCopied == size_);
}

}  // namespace mpg123
}  // namespace format
}  // namespace audio
}  // namespace rec

