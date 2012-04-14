#include "rec/util/FormatString.h"

namespace rec {
namespace util {

using namespace juce;

namespace {

class FormatHelper {
 public:
  FormatHelper(const String& source, StringArray* p, Array<int>* a)
      : parts_(p), args_(a), inNumber_(false), wasPercent_(false), number_(0) {
    for (int i = 0; i < source.length(); ++i)
      parseChar(source[i]);
    parseChar('\0');
    DCHECK_EQ(parts_->size(), args_->size() + 1);
  }

 private:
  void parseChar(juce_wchar ch) {
    if (inNumber_) {
      parseNumber(ch);
      if (inNumber_)
        return;
    }

    if (!ch)
      parseZero();
    else if (ch == '%')
      parsePercent();
    else
      parseAny(ch);
  }

  void parseNumber(juce_wchar ch) {
    if (isdigit(ch))
      addDigit(ch);
    else
      addArg();
  }

  void parseZero() {
    if (wasPercent_)
      prefix_ += '%';  // Trailing %
    addPart();
  }

  void parsePercent() {
    if (wasPercent_)
      prefix_ += '%';
    wasPercent_ = !wasPercent_;
  }

  void parseAny(juce_wchar ch) {
    if (wasPercent_) {
      wasPercent_ = false;
      if (isdigit(ch)) {
        addPart();
        inNumber_ = true;
        addDigit(ch);
        return;
      } else {
        prefix_ += '%';
      }
    }
    prefix_ += ch;
  }

  void addPart() {
    parts_->insert(-1, prefix_);
    prefix_ = "";
  }

  void addArg() {
    args_->add(number_ - 1);
    number_ = 0;
    inNumber_ = false;
  }

  void addDigit(juce_wchar ch) {
    number_ = 10 * number_ + (ch - '0');
    inNumber_ = true;
  }

  StringArray* parts_;
  Array<int>* args_;

  String prefix_;
  bool inNumber_;
  bool wasPercent_;
  int number_;
};

const String ERROR = "?";

inline void append(StringArray* arr, const String& s) {
  arr->insert(-1, s);
}

}  // namespace

FormatString::FormatString(const String& source) {
  FormatHelper helper(source, &parts_, &args_);
}

String FormatString::format(const StringArray& strings) const {
  int len = strings.size();
  if (len != args_.size())
    LOG(DFATAL) << "Expected " << args_.size() << " arguments, got " << len;

  StringArray result;
  for (int i = 0; i < parts_.size(); ++i) {
    append(&result, parts_[i]);
    if (i < args_.size()) {
      int a = args_[i];
      DCHECK_GE(a, 0);
      DCHECK_LT(a, len);
      append(&result, (a >= 0 && a < len) ? strings[args_[i]] : ERROR);
    }
  }
  return result.joinIntoString("");
}

String FormatString::format() const {
  StringArray a;
  return format(a);
}

String FormatString::format(const String& s1) const {
  StringArray a;
  append(&a, s1);
  return format(a);
}

String FormatString::format(const String& s1, const String& s2) const {
  StringArray a;
  append(&a, s1);
  append(&a, s2);
  return format(a);
}

String FormatString::format(const String& s1, const String& s2, const String& s3) const {
  StringArray a;
  append(&a, s1);
  append(&a, s2);
  append(&a, s3);
  return format(a);
}

}  // namespace util
}  // namespace rec

