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


#pragma once

#include "NetworkConfiguration.h"
#include "Process.h"
#include "TestNode.h"

namespace System {
class Dispatcher;
}

namespace CryptoNote {
class Currency;
}

namespace Tests {

enum class Topology {
  Ring,
  Line,
  Star
};


class TestNetworkBuilder {
public:

  TestNetworkBuilder(size_t nodeCount, Topology topology = Topology::Line, uint16_t rpcBasePort = 9200, uint16_t p2pBasePort = 9000);

  TestNetworkBuilder& setDataDirectory(const std::string& dataDir);
  TestNetworkBuilder& setBlockchain(const std::string& blockchainDir);
  TestNetworkBuilder& setTestnet(bool isTestnet);

  std::vector<TestNodeConfiguration> build();

private:

  TestNodeConfiguration buildNodeConfiguration(size_t index);

  uint16_t rpcBasePort;
  uint16_t p2pBasePort;
  Topology topology;
  size_t nodeCount;
  std::string baseDataDir;
  std::string blockchainLocation;
  bool testnet;
};


class TestNetwork {

public:

  TestNetwork(System::Dispatcher& dispatcher, const CryptoNote::Currency& currency);

  void addNodes(const std::vector<TestNodeConfiguration>& nodes);
  void addNode(const TestNodeConfiguration& cfg);
  void waitNodesReady();
  void shutdown();

  TestNode& getNode(size_t index);

private:

  std::unique_ptr<TestNode> startDaemon(const TestNodeConfiguration& cfg);

  std::vector<std::pair<std::unique_ptr<TestNode>, TestNodeConfiguration>> nodes;
  System::Dispatcher& m_dispatcher;
  const CryptoNote::Currency& m_currency;
  std::vector<Process> m_daemons;
};

}
