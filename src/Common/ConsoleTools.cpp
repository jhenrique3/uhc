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

#include "ConsoleTools.h"

#include <stdio.h>

#ifdef _WIN32
#include <Windows.h>
#include <io.h>
#else
#include <iostream>
#include <unistd.h>
#endif

namespace Common { namespace Console { 

bool isConsoleTty() {
#if defined(WIN32)
  static bool istty = 0 != _isatty(_fileno(stdout));
#else
  static bool istty = 0 != isatty(fileno(stdout));
#endif
  return istty;
}

void setTextColor(Color color) {
  if (!isConsoleTty()) {
    return;
  }

  if (color > Color::BrightMagenta) {
    color = Color::Default;
  }

#ifdef _WIN32

  static WORD winColors[] = {
    // default
    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
    // main
    FOREGROUND_BLUE,
    FOREGROUND_GREEN,
    FOREGROUND_RED,
    FOREGROUND_RED | FOREGROUND_GREEN,
    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
    FOREGROUND_GREEN | FOREGROUND_BLUE,
    FOREGROUND_RED | FOREGROUND_BLUE,
    // bright
    FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    FOREGROUND_RED | FOREGROUND_INTENSITY,
    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY
  };

  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), winColors[static_cast<size_t>(color)]);

#else

  static const char* ansiColors[] = {
    // default
    "\033[0m",
    // main
    "\033[0;34m",
    "\033[0;32m",
    "\033[0;31m",
    "\033[0;33m",
    "\033[0;37m",
    "\033[0;36m",
    "\033[0;35m",
    // bright
    "\033[1;34m",
    "\033[1;32m",
    "\033[1;31m",
    "\033[1;33m",
    "\033[1;37m",
    "\033[1;36m",
    "\033[1;35m"
  };

  std::cout << ansiColors[static_cast<size_t>(color)];

#endif

}

}}
