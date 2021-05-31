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


#include <System/Dispatcher.h>
#include <System/ContextGroup.h>
#include <System/Event.h>
#include <System/InterruptedException.h>
#include <System/Ipv4Address.h>
#include <System/TcpConnection.h>
#include <System/TcpConnector.h>
#include <System/TcpListener.h>
#include <System/Timer.h>
#include <gtest/gtest.h>

using namespace System;

class TcpConnectorTests : public testing::Test {
public:
  TcpConnectorTests() : event(dispatcher), listener(dispatcher, Ipv4Address("127.0.0.1"), 6666), contextGroup(dispatcher) {
  }
  
  Dispatcher dispatcher;
  Event event;
  TcpListener listener;
  ContextGroup contextGroup;
};

TEST_F(TcpConnectorTests, tcpConnector1) {
  contextGroup.spawn([&]() {
    listener.accept();
    event.set();
  });

  TcpConnector connector(dispatcher);
  contextGroup.spawn([&] { 
    connector.connect(Ipv4Address("127.0.0.1"), 6666); 
  });
  event.wait();
  dispatcher.yield();
}

TEST_F(TcpConnectorTests, tcpConnectorInterruptAfterStart) {
  contextGroup.spawn([&] { 
    ASSERT_THROW(TcpConnector(dispatcher).connect(Ipv4Address("127.0.0.1"), 6666), InterruptedException); 
  });
  contextGroup.interrupt();
}

TEST_F(TcpConnectorTests, tcpConnectorInterrupt) {
  TcpConnector connector(dispatcher);
  contextGroup.spawn([&]() {
    Timer(dispatcher).sleep(std::chrono::milliseconds(10));
    contextGroup.interrupt();
    event.set();
  });

  contextGroup.spawn([&] { 
    ASSERT_THROW(connector.connect(Ipv4Address("10.255.255.1"), 6666), InterruptedException); 
  });
  contextGroup.wait();
}

TEST_F(TcpConnectorTests, tcpConnectorUseAfterInterrupt) {
  TcpConnector connector(dispatcher);
  contextGroup.spawn([&]() {
    Timer(dispatcher).sleep(std::chrono::milliseconds(10));
    contextGroup.interrupt();
    event.set();
  });

  contextGroup.spawn([&] { 
    ASSERT_THROW(connector.connect(Ipv4Address("10.255.255.1"), 6666), InterruptedException); 
  });
  contextGroup.wait();
  contextGroup.spawn([&] { 
    ASSERT_NO_THROW(connector.connect(Ipv4Address("127.0.0.1"), 6666)); 
  });
  contextGroup.wait();
}

TEST_F(TcpConnectorTests, bindToTheSameAddressFails) {
  ASSERT_THROW(TcpListener listener2(dispatcher, Ipv4Address("127.0.0.1"), 6666), std::runtime_error);
}
