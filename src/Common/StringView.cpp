// Copyright (c) 2021, Ultimate Hacking Coin
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice, this list
//    of conditions and the following disclaimer in the documentation and/or other
//    materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors may be
//    used to endorse or promote products derived from this software without specific
//    prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Parts of this file are originally copyright (c) 2011-2016 The Cryptonote developers

#include "StringView.h"
#include <limits>

namespace Common {

const StringView::Size StringView::INVALID = std::numeric_limits<StringView::Size>::max();
const StringView StringView::EMPTY(reinterpret_cast<Object*>(1), 0);
const StringView StringView::NIL(nullptr, 0);

StringView::StringView()
#ifndef NDEBUG
  : data(nullptr), size(INVALID) // In debug mode, fill in object with invalid state (undefined).
#endif
{
}

StringView::StringView(const Object* stringData, Size stringSize) : data(stringData), size(stringSize) {
  assert(data != nullptr || size == 0);
}

StringView::StringView(const std::string& string) : data(string.data()), size(string.size()) {
}

StringView::StringView(const StringView& other) : data(other.data), size(other.size) {
  assert(data != nullptr || size == 0);
}

StringView::~StringView() {
}

StringView& StringView::operator=(const StringView& other) {
  assert(other.data != nullptr || other.size == 0);
  data = other.data;
  size = other.size;
  return *this;
}

StringView::operator std::string() const {
  return std::string(data, size);
}

const StringView::Object* StringView::getData() const {
  assert(data != nullptr || size == 0);
  return data;
}

StringView::Size StringView::getSize() const {
  assert(data != nullptr || size == 0);
  return size;
}

bool StringView::isEmpty() const {
  assert(data != nullptr || size == 0);
  return size == 0;
}

bool StringView::isNil() const {
  assert(data != nullptr || size == 0);
  return data == nullptr;
}

const StringView::Object& StringView::operator[](Size index) const {
  assert(data != nullptr || size == 0);
  assert(index < size);
  return *(data + index);
}

const StringView::Object& StringView::first() const {
  assert(data != nullptr || size == 0);
  assert(size > 0);
  return *data;
}

const StringView::Object& StringView::last() const {
  assert(data != nullptr || size == 0);
  assert(size > 0);
  return *(data + (size - 1));
}

const StringView::Object* StringView::begin() const {
  assert(data != nullptr || size == 0);
  return data;
}

const StringView::Object* StringView::end() const {
  assert(data != nullptr || size == 0);
  return data + size;
}

bool StringView::operator==(StringView other) const {
  assert(data != nullptr || size == 0);
  assert(other.data != nullptr || other.size == 0);
  if (size == other.size) {
    for (Size i = 0;; ++i) {
      if (i == size) {
        return true;
      }

      if (!(*(data + i) == *(other.data + i))) {
        break;
      }
    }
  }

  return false;
}

bool StringView::operator!=(StringView other) const {
  return !(*this == other);
}

bool StringView::operator<(StringView other) const {
  assert(data != nullptr || size == 0);
  assert(other.data != nullptr || other.size == 0);
  Size count = other.size < size ? other.size : size;
  for (Size i = 0; i < count; ++i) {
    Object char1 = *(data + i);
    Object char2 = *(other.data + i);
    if (char1 < char2) {
      return true;
    }

    if (char2 < char1) {
      return false;
    }
  }

  return size < other.size;
}

bool StringView::operator<=(StringView other) const {
  return !(other < *this);
}

bool StringView::operator>(StringView other) const {
  return other < *this;
}

bool StringView::operator>=(StringView other) const {
  return !(*this < other);
}

bool StringView::beginsWith(const Object& object) const {
  assert(data != nullptr || size == 0);
  if (size == 0) {
    return false;
  }

  return *data == object;
}

bool StringView::beginsWith(StringView other) const {
  assert(data != nullptr || size == 0);
  assert(other.data != nullptr || other.size == 0);
  if (size >= other.size) {
    for (Size i = 0;; ++i) {
      if (i == other.size) {
        return true;
      }

      if (!(*(data + i) == *(other.data + i))) {
        break;
      }
    }
  }

  return false;
}

bool StringView::contains(const Object& object) const {
  assert(data != nullptr || size == 0);
  for (Size i = 0; i < size; ++i) {
    if (*(data + i) == object) {
      return true;
    }
  }

  return false;
}

bool StringView::contains(StringView other) const {
  assert(data != nullptr || size == 0);
  assert(other.data != nullptr || other.size == 0);
  if (size >= other.size) {
    Size i = size - other.size;
    for (Size j = 0; !(i < j); ++j) {
      for (Size k = 0;; ++k) {
        if (k == other.size) {
          return true;
        }

        if (!(*(data + j + k) == *(other.data + k))) {
          break;
        }
      }
    }
  }

  return false;
}

bool StringView::endsWith(const Object& object) const {
  assert(data != nullptr || size == 0);
  if (size == 0) {
    return false;
  }

  return *(data + (size - 1)) == object;
}

bool StringView::endsWith(StringView other) const {
  assert(data != nullptr || size == 0);
  assert(other.data != nullptr || other.size == 0);
  if (size >= other.size) {
    Size i = size - other.size;
    for (Size j = 0;; ++j) {
      if (j == other.size) {
        return true;
      }

      if (!(*(data + i + j) == *(other.data + j))) {
        break;
      }
    }
  }

  return false;
}

StringView::Size StringView::find(const Object& object) const {
  assert(data != nullptr || size == 0);
  for (Size i = 0; i < size; ++i) {
    if (*(data + i) == object) {
      return i;
    }
  }

  return INVALID;
}

StringView::Size StringView::find(StringView other) const {
  assert(data != nullptr || size == 0);
  assert(other.data != nullptr || other.size == 0);
  if (size >= other.size) {
    Size i = size - other.size;
    for (Size j = 0; !(i < j); ++j) {
      for (Size k = 0;; ++k) {
        if (k == other.size) {
          return j;
        }

        if (!(*(data + j + k) == *(other.data + k))) {
          break;
        }
      }
    }
  }

  return INVALID;
}

StringView::Size StringView::findLast(const Object& object) const {
  assert(data != nullptr || size == 0);
  for (Size i = 0; i < size; ++i) {
    if (*(data + (size - 1 - i)) == object) {
      return size - 1 - i;
    }
  }

  return INVALID;
}

StringView::Size StringView::findLast(StringView other) const {
  assert(data != nullptr || size == 0);
  assert(other.data != nullptr || other.size == 0);
  if (size >= other.size) {
    Size i = size - other.size;
    for (Size j = 0; !(i < j); ++j) {
      for (Size k = 0;; ++k) {
        if (k == other.size) {
          return i - j;
        }

        if (!(*(data + (i - j + k)) == *(other.data + k))) {
          break;
        }
      }
    }
  }

  return INVALID;
}

StringView StringView::head(Size headSize) const {
  assert(data != nullptr || size == 0);
  assert(headSize <= size);
  return StringView(data, headSize);
}

StringView StringView::tail(Size tailSize) const {
  assert(data != nullptr || size == 0);
  assert(tailSize <= size);
  return StringView(data + (size - tailSize), tailSize);
}

StringView StringView::unhead(Size headSize) const {
  assert(data != nullptr || size == 0);
  assert(headSize <= size);
  return StringView(data + headSize, size - headSize);
}

StringView StringView::untail(Size tailSize) const {
  assert(data != nullptr || size == 0);
  assert(tailSize <= size);
  return StringView(data, size - tailSize);
}

StringView StringView::range(Size startIndex, Size endIndex) const {
  assert(data != nullptr || size == 0);
  assert(startIndex <= endIndex && endIndex <= size);
  return StringView(data + startIndex, endIndex - startIndex);
}

StringView StringView::slice(Size startIndex, Size sliceSize) const {
  assert(data != nullptr || size == 0);
  assert(startIndex <= size && startIndex + sliceSize <= size);
  return StringView(data + startIndex, sliceSize);
}

}
