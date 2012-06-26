#include <gtest/gtest.h>

#include "rec/util/FormatString.h"

namespace rec {
namespace util {
namespace {

TEST(FormatString, Trivial) {
  FormatString s("");
  EXPECT_EQ(str(s.format()), "");
}

TEST(FormatString, OnePart) {
  FormatString s("foo");
  EXPECT_EQ("foo", str(s.format()));
}

TEST(FormatString, TwoParts) {
  FormatString s("foo %1 baz");
  EXPECT_EQ("foo bar baz", str(s.format("bar")));
}

TEST(FormatString, ThreeParts) {
  FormatString s("foo %1 baz %2");
  EXPECT_EQ("foo bar baz bing", str(s.format("bar", "bing")));
}

TEST(FormatString, ThreePartsTwoEmpty) {
  FormatString s("foo %1%2");
  EXPECT_EQ("foo barbing", str(s.format("bar", "bing")));
}

TEST(FormatString, MissingNumber) {
  FormatString s("foo %hello");
  EXPECT_EQ("foo %hello", str(s.format()));
}

TEST(FormatString, EscapedPercent) {
  FormatString s("foo %%123");
  EXPECT_EQ("foo %123", str(s.format()));
}

TEST(FormatString, PercentAtEnd) {
  FormatString s("foo %1 baz %");
  EXPECT_EQ("foo bar baz %", str(s.format("bar")));
}

}  // namespace
}  // namespace util
}  // namespace rec
