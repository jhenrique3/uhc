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

#include "CommonTypes.h"
#include "IStreamSerializable.h"
#include "Serialization/ISerializer.h"
#include <vector>
#include <map>

namespace CryptoNote {

class SynchronizationState : public IStreamSerializable {
public:

  struct CheckResult {
    bool detachRequired;
    uint32_t detachHeight;
    bool hasNewBlocks;
    uint32_t newBlockHeight;
  };

  typedef std::vector<Crypto::Hash> ShortHistory;

  explicit SynchronizationState(const Crypto::Hash& genesisBlockHash) {
    m_blockchain.push_back(genesisBlockHash);
  }

  ShortHistory getShortHistory(uint32_t localHeight) const;
  CheckResult checkInterval(const BlockchainInterval& interval) const;

  void detach(uint32_t height);
  void addBlocks(const Crypto::Hash* blockHashes, uint32_t height, uint32_t count);
  uint32_t getHeight() const;
  const std::vector<Crypto::Hash>& getKnownBlockHashes() const;

  // IStreamSerializable
  virtual void save(std::ostream& os) override;
  virtual void load(std::istream& in) override;

  // serialization
  CryptoNote::ISerializer& serialize(CryptoNote::ISerializer& s, const std::string& name);

private:

  std::vector<Crypto::Hash> m_blockchain;
};

}
