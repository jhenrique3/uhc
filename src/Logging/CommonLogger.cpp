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

#include "CommonLogger.h"

namespace Logging {

namespace {

std::string formatPattern(const std::string& pattern, const std::string& category, Level level, boost::posix_time::ptime time) {
  std::stringstream s;

  for (const char* p = pattern.c_str(); p && *p != 0; ++p) {
    if (*p == '%') {
      ++p;
      switch (*p) {
      case 0:
        break;
      case 'C':
        s << category;
        break;
      case 'D':
        s << time.date();
        break;
      case 'T':
        s << time.time_of_day();
        break;
      case 'L':
        s << ILogger::LEVEL_NAMES[level];
        break;
      default:
        s << *p;
      }
    } else {
      s << *p;
    }
  }

  return s.str();
}

}

void CommonLogger::operator()(const std::string& category, Level level, boost::posix_time::ptime time, const std::string& body) {
  if (level <= logLevel && disabledCategories.count(category) == 0) {
    std::string body2 = body;
    if (!pattern.empty()) {
      size_t insertPos = 0;
      if (!body2.empty() && body2[0] == ILogger::COLOR_DELIMETER) {
        size_t delimPos = body2.find(ILogger::COLOR_DELIMETER, 1);
        if (delimPos != std::string::npos) {
          insertPos = delimPos + 1;
        }
      }

      body2.insert(insertPos, formatPattern(pattern, category, level, time));
    }

    doLogString(body2);
  }
}

void CommonLogger::setPattern(const std::string& pattern) {
  this->pattern = pattern;
}

void CommonLogger::enableCategory(const std::string& category) {
  disabledCategories.erase(category);
}

void CommonLogger::disableCategory(const std::string& category) {
  disabledCategories.insert(category);
}

void CommonLogger::setMaxLevel(Level level) {
  logLevel = level;
}

CommonLogger::CommonLogger(Level level) : logLevel(level), pattern("%D %T %L [%C] ") {
}

void CommonLogger::doLogString(const std::string& message) {
}

}
