// Copyright 2016 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIB_FTL_TIME_TIME_DELTA_H_
#define LIB_FTL_TIME_TIME_DELTA_H_

#include <stdint.h>
#include <time.h>

namespace ftl {

// A TimeDelta represents the difference between two time points.

class TimeDelta {
 public:
  TimeDelta() = default;

  static constexpr TimeDelta Zero() { return TimeDelta(); }
  static constexpr TimeDelta FromNanoseconds(int64_t nanos) {
    return TimeDelta(nanos);
  }
  static constexpr TimeDelta FromMicroseconds(int64_t micros) {
    return FromNanoseconds(micros * 1000);
  }
  static constexpr TimeDelta FromMilliseconds(int64_t millis) {
    return FromMicroseconds(millis * 1000);
  }
  static constexpr TimeDelta FromSeconds(int64_t seconds) {
    return FromMilliseconds(seconds * 1000);
  }

  int64_t ToNanoseconds() const { return delta_; }
  int64_t ToMicroseconds() const { return ToNanoseconds() / 1000; }
  int64_t ToMilliseconds() const { return ToMicroseconds() / 1000; }
  int64_t ToSeconds() const { return ToMilliseconds() / 1000; }

  double ToNanosecondsF() const { return delta_; }
  double ToMicrosecondsF() const { return delta_ / 1000.0; }
  double ToMillisecondsF() const { return delta_ / (1000.0 * 1000.0); }
  double ToSecondsF() const { return delta_ / (1000.0 * 1000.0 * 1000.0); }

  TimeDelta operator-(TimeDelta other) const {
    return TimeDelta::FromNanoseconds(delta_ - other.delta_);
  }

  TimeDelta operator+(TimeDelta other) const {
    return TimeDelta::FromNanoseconds(delta_ + other.delta_);
  }

  bool operator==(TimeDelta other) const { return delta_ == other.delta_; }
  bool operator!=(TimeDelta other) const { return delta_ != other.delta_; }
  bool operator<(TimeDelta other) const { return delta_ < other.delta_; }
  bool operator<=(TimeDelta other) const { return delta_ <= other.delta_; }
  bool operator>(TimeDelta other) const { return delta_ > other.delta_; }
  bool operator>=(TimeDelta other) const { return delta_ >= other.delta_; }

  static constexpr TimeDelta FromTimespec(struct timespec ts) {
    return TimeDelta::FromSeconds(ts.tv_sec) +
           TimeDelta::FromNanoseconds(ts.tv_nsec);
  }
  constexpr struct timespec ToTimespec() {
    struct timespec ts {};
    constexpr int64_t kNanosecondsPerSecond = 1000000000ll;
    ts.tv_sec = static_cast<time_t>(ToSeconds());
    ts.tv_nsec = delta_ % kNanosecondsPerSecond;
    return ts;
  }

 private:
  // Private, use one of the FromFoo() types
  explicit constexpr TimeDelta(int64_t delta) : delta_(delta) {}

  int64_t delta_ = 0;
};

}  // namespace ftl

#endif  // LIB_FTL_TIME_TIME_DELTA_H_