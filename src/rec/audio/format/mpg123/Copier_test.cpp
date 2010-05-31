#include <gtest/gtest.h>
#include "rec/audio/format/mpg123/Copier.h"

namespace rec {
namespace audio {
namespace format {
namespace mpg123 {

template <typename Number>
void testIdentity() {
  Number samples[3][2] = {{1, 11}, {2, 12}, {3, 13}};
  Number destBase[2][3];
  Number* dest[2] = {destBase[0], destBase[1]};
  getCopier(MPG123_ENC_SIGNED_32)((int**) dest, 2, 0, samples[0], 2, 3);

  for (int c = 0; c < 2; ++c) {
    for (int s = 0; s < 3; ++s)
      EXPECT_EQ(samples[s][c], destBase[c][s]) << c << ", " << s;
  }
}

TEST(RecAudioFormatMpg123Copier, IntIdentity) {
  testIdentity<int>();
}

TEST(RecAudioFormatMpg123Copier, FloatIdentity) {
  testIdentity<float>();
}

}  // namespace mpg123
}  // namespace format
}  // namespace audio
}  // namespace rec
