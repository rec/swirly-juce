#ifndef __REC_AUDIO_FORMAT_MPG123_REPLACE_READER__
#define __REC_AUDIO_FORMAT_MPG123_REPLACE_READER__

#include <stdlib.h>
#include <sys/types.h>

#include "juce_amalgamated.h"

#include "rec/base/scoped_ptr.h"
#include "rec/audio/format/mpg123/mpg123.h"

namespace rec {
namespace audio {
namespace format {
namespace mpg123 {

#include "libmpg123/mpg123.h"
  
namespace {

ssize_t read(void *inputStream, void *buf, size_t nbytes) {
  return reinterpret_cast<InputStream*>(inputStream)->read(buf, nbytes);
}

off_t seek(void *inputStream, off_t offset, int whence) {
  InputStream* in = reinterpret_cast<InputStream*>(inputStream);

  switch (whence) {
    case SEEK_CUR:
      offset += in->getPosition();
      break;

    case SEEK_SET:
      break;

    case SEEK_END: {
      offset = in->getTotalLength() - offset;
      if (offset < 0)
        return -1;

      break;
    }

    default:
      return -1;
  }

  return in->setPosition(offset) ? offset : -1;
}

}  // namespace

Error newHandle(InputStream* in, mpg123_handle** result) {
  if (Error e = initializeOnce())
    return e;

  Error e;
  mpg123_handle* mh = mpg123_new(NULL, &e);
  if (e)
    return e;

  scoped_ptr<mpg123_handle> mhs(mh);

  if (Error e = mpg123_replace_reader_handle(mh, read, seek, NULL))
    return e;

  if (Error e = mpg123_open_handle(mh, in))
    return e;

  if (Error e = mpg123_scan(mh))
    return e;

  return MPG123_OK;
}



}  // namespace mpg123
}  // namespace format
}  // namespace audio
}  // namespace rec

#endif  // __REC_AUDIO_FORMAT_MPG123_REPLACE_READER__
