#include "juce_amalgamated.h"
#include "rec/audio/format/mpg123/Mpg123.h"

namespace rec {
namespace audio {
namespace format {
namespace mpg123 {

#include "libmpg123/mpg123.h"

namespace {

class Initializer {
 public:
  Initializer() { initialize(); }

  Error initialize() {
    if (!running_) {
      initError_ = mpg123_init();
      running_ = true;
    }
    return initError_;
  }

  void exit() {
    if (running_) {
      running_ = false;
      if (!initError_)
        mpg123_exit();
    }
  }

  Error initError_;
  bool running_;

  static Initializer& instance() {
    // C guarantees thread-safe construction of this variable, so we're
    // guaranteed that that first mpg123_init() is atomic - but if you turn it
    // off and on, all bets are off!
    static Initializer i;
    return i;
  }
};

}  // namespace

Error initializeOnce() {
  return Initializer::instance().initError_;
}

void exitOnce() {
  Initializer::instance().exit();
}

StringArray getFileExtensions() {
  const tchar* const extensions[] = {
    JUCE_T(".mp3"), JUCE_T(".mp2"), JUCE_T(".mp1"), 0
  };
  return StringArray(extensions);
}

Array<int> getSampleRates() {
  const long* rates;
  size_t ratesCount;
  mpg123_rates(&rates, &ratesCount);

  Array<int> r(rates, ratesCount);
  r.add(0);

  return r;
}

Array<int> getBitDepths() {
  static const int depths[] = {8, 16, 32, 64, 0};
  return Array<int>(depths);
}

int getBitsPerSample(int encoding) {
  return
    (encoding & MPG123_ENC_16) ? 16 :
    (encoding & MPG123_ENC_32) ? 32 :
    (encoding & MPG123_ENC_8) ? 8 :
    (encoding & MPG123_ENC_FLOAT_64) ? 64 :
    0;
}

inline String toString(const mpg123_string* s) {
  return String(s->p, s->fill);
}

Error getMp3Tags(mpg123_handle* mh, StringPairArray* metadata) {
  if (!mpg123_meta_check(mh))
    return MPG123_ERR;

  mpg123_id3v1 *v1;
  mpg123_id3v2 *v2;

  if (Error e = mpg123_id3(mh, &v1, &v2))
    return e;

  if (v2) {
    metadata->set("title", toString(v2->title));
    metadata->set("artist", toString(v2->artist));
    metadata->set("album", toString(v2->album));
    metadata->set("year", toString(v2->year));
    metadata->set("genre", toString(v2->genre));
    metadata->set("comment", toString(v2->comment));
    return MPG123_OK;
  }

  if (v1) {
    metadata->set("title", String(v1->title, 30));
    metadata->set("artist", String(v1->artist, 30));
    metadata->set("album", String(v1->album, 30));
    metadata->set("year", String(v1->year, 4));
    metadata->set("comment", String(v1->comment, 30));
    metadata->set("genre", String((const char*) &v1->genre, 1));
    return MPG123_OK;
  }

  return MPG123_ERR;
}

}  // namespace mpg123
}  // namespace format
}  // namespace audio
}  // namespace rec
