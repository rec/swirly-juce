#ifndef __REC_BASE_SAMPLES__
#define __REC_BASE_SAMPLES__

#include "rec/base/RealTime.h"

namespace rec {

template <int SAMPLES_PER_SEC>
struct Samples {
  Samples() {}

  // Conversion from integer types means "a sample count".
  Samples(int64 p) : position_(p) {}
  Samples(int p) : position_(p) {}
  Samples(short p) : position_(p) {}

  Samples(RealTime time) : position_(toSampleCount(time)) {}

  Samples& operator++() { ++position_; return *this; }
  Samples& operator--() { --position_; return *this; }

  Samples operator++(int) { return position_++; }
  Samples operator--(int) { return position_--; }

  Samples& operator-=(Samples p) { position_ -= p; return *this; }
  Samples& operator+=(Samples p) { position_ += p; return *this; }

  const Samples operator+(Samples p) { return position_ + p; }
  const Samples operator-(Samples p) { return position_ - p; }
  const Samples operator+(int p) { return position_ + p; }
  const Samples operator-(int p) { return position_ - p; }
  const Samples operator+(int64 p) { return position_ + p; }
  const Samples operator-(int64 p) { return position_ - p; }

  operator int64() const { return position_; }

  double toRealTime() const {
    return static_cast<double>(position_) / static_cast<double>(SAMPLES_PER_SEC);
  }

  template <typename Type>
  int64 toSampleCount(Type t) const {
    return static_cast<int64>(SAMPLES_PER_SEC * t);
  }

  // TODO: we shouldn't need this, but Juce sometimes wants ints.  Bug Jules!
  int toInt() const { return static_cast<int>(position_); }

 private:
  int64 position_;

 private:
  // Disallow these two constructors.
  Samples(float time);
  Samples(double time);
};

}  // namespace rec

#endif  // __REC_BASE_SAMPLES__
