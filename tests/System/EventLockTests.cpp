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


#include <System/Context.h>
#include <System/Dispatcher.h>
#include <System/Event.h>
#include <System/EventLock.h>
#include <gtest/gtest.h>

using namespace System;

TEST(EventLockTests, eventLockIsLocking) {
  Dispatcher dispatcher;
  Event event(dispatcher);
  bool done = false;
  Context<> context(dispatcher, [&]() {
    EventLock lock(event);
    done = true;
  });

  ASSERT_FALSE(done);
  dispatcher.yield();
  ASSERT_FALSE(done);
  event.set();
  dispatcher.yield();
  ASSERT_TRUE(done);
}

TEST(EventLockTests, eventLockIsNotLocking) {
  Dispatcher dispatcher;
  Event event(dispatcher);
  event.set();
  bool done = false;
  Context<> context(dispatcher, [&]() {
    EventLock lock(event);
    done = true;
  });

  ASSERT_FALSE(done);
  dispatcher.yield();
  ASSERT_TRUE(done);
}

TEST(EventLockTests, eventLockIsUnlockOnlyOnce) {
  Dispatcher dispatcher;
  Event event(dispatcher);
  auto i = 0;
  Context<> context(dispatcher, [&]() {
    EventLock lock(event);
    i++;
    dispatcher.yield();
    i++;
  });

  Context<> contextSecond(dispatcher, [&]() {
    EventLock lock(event);
    i += 2;
    dispatcher.yield();
    i += 2;
  });
  
  event.set();
  dispatcher.yield();
  ASSERT_EQ(i, 1);
  dispatcher.yield();
  ASSERT_EQ(i, 2);
  dispatcher.yield();
  ASSERT_EQ(i, 4);
  dispatcher.yield();
  ASSERT_EQ(i, 6);
}
