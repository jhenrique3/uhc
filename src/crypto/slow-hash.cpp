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

#include <new>

#include "hash.h"

#if defined(WIN32)
#include <Windows.h>
#else
#include <sys/mman.h>
#endif

using std::bad_alloc;

namespace Crypto {

  enum {
    MAP_SIZE = SLOW_HASH_CONTEXT_SIZE + ((-SLOW_HASH_CONTEXT_SIZE) & 0xfff)
  };

#if defined(WIN32)

  cn_context::cn_context() {
    data = VirtualAlloc(nullptr, MAP_SIZE, MEM_COMMIT, PAGE_READWRITE);
    if (data == nullptr) {
      throw bad_alloc();
    }
  }

  cn_context::~cn_context() {
    if (!VirtualFree(data, 0, MEM_RELEASE)) {
      throw bad_alloc();
    }
  }

#else

  cn_context::cn_context() {
#if !defined(__APPLE__)
    data = mmap(nullptr, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_POPULATE, -1, 0);
#else
    data = mmap(nullptr, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
#endif
    if (data == MAP_FAILED) {
      throw bad_alloc();
    }
    mlock(data, MAP_SIZE);
  }

  cn_context::~cn_context() {
    if (munmap(data, MAP_SIZE) != 0) {
      throw bad_alloc();
    }
  }

#endif

}
