#include "rec/base/RealTime.h"
#include "rec/base/Samples.h"

namespace rec {

RealTime::RealTime(const Samples<44100>& pos) : time_(pos / 44100.0) {}

}  // namespace rec
