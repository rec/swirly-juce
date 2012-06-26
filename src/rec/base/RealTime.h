#ifndef __REC_BASE_REALTIME__
#define __REC_BASE_REALTIME__

#include "rec/util/Range.h"

namespace rec {

template <int SAMPLES_PER_SEC> struct Samples;

struct RealTime {
  RealTime() : time_(0) {}
  RealTime(double p) : time_(p) {}
  RealTime(float p) : time_(p) {}
  RealTime(const Samples<44100>& pos);

  // We need this constructor so we can construct an "empty" RealTime(0).
  // Best to avoid using.
  explicit RealTime(int time) : time_(static_cast<double>(time)) {
    DCHECK(!time);
  }

  const RealTime operator+(RealTime p) { return time_ + p; }
  const RealTime operator-(RealTime p) { return time_ - p; }

  const RealTime operator+(double p) { return time_ + p; }
  const RealTime operator-(double p) { return time_ - p; }
  const RealTime operator+(float p) { return time_ + p; }
  const RealTime operator-(float p) { return time_ - p; }

  RealTime& operator-=(RealTime t) { time_ -= t; return *this; }
  RealTime& operator+=(RealTime t) { time_ += t; return *this; }
  RealTime& operator/=(RealTime t) { time_ /= t; return *this; }
  RealTime& operator*=(RealTime t) { time_ *= t; return *this; }

  double time_;
  operator double() const { return time_; }

 private:
  // Disallow these constructors.
  RealTime(int64 time);
  RealTime(short time);

};

}  // namespace rec

#endif  // __REC_BASE_REALTIME__
