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

#include "BlockchainIndices.h"

#include "Common/StringTools.h"
#include "CryptoNoteCore/CryptoNoteTools.h"
#include "CryptoNoteCore/CryptoNoteFormatUtils.h"
#include "BlockchainExplorer/BlockchainExplorerDataBuilder.h"
#include "CryptoNoteBasicImpl.h"

namespace CryptoNote {

bool PaymentIdIndex::add(const Transaction& transaction) {
  Crypto::Hash paymentId;
  Crypto::Hash transactionHash = getObjectHash(transaction);
  if (!BlockchainExplorerDataBuilder::getPaymentId(transaction, paymentId)) {
    return false;
  }

  index.emplace(paymentId, transactionHash);

  return true;
}

bool PaymentIdIndex::remove(const Transaction& transaction) {
  Crypto::Hash paymentId;
  Crypto::Hash transactionHash = getObjectHash(transaction);
  if (!BlockchainExplorerDataBuilder::getPaymentId(transaction, paymentId)) {
    return false;
  }

  auto range = index.equal_range(paymentId);
  for (auto iter = range.first; iter != range.second; ++iter){
    if (iter->second == transactionHash) {
      index.erase(iter);
      return true;
    }
  }

  return false;
}

bool PaymentIdIndex::find(const Crypto::Hash& paymentId, std::vector<Crypto::Hash>& transactionHashes) {
  bool found = false;
  auto range = index.equal_range(paymentId);
  for (auto iter = range.first; iter != range.second; ++iter){
    found = true;
    transactionHashes.emplace_back(iter->second);
  }
  return found;
}

void PaymentIdIndex::clear() {
  index.clear();
}


void PaymentIdIndex::serialize(ISerializer& s) {
  s(index, "index");
}

bool TimestampBlocksIndex::add(uint64_t timestamp, const Crypto::Hash& hash) {
  index.emplace(timestamp, hash);
  return true;
}

bool TimestampBlocksIndex::remove(uint64_t timestamp, const Crypto::Hash& hash) {
  auto range = index.equal_range(timestamp);
  for (auto iter = range.first; iter != range.second; ++iter) {
    if (iter->second == hash) {
      index.erase(iter);
      return true;
    }
  }

  return false;
}

bool TimestampBlocksIndex::find(uint64_t timestampBegin, uint64_t timestampEnd, uint32_t hashesNumberLimit, std::vector<Crypto::Hash>& hashes, uint32_t& hashesNumberWithinTimestamps) {
  uint32_t hashesNumber = 0;
  if (timestampBegin > timestampEnd) {
    //std::swap(timestampBegin, timestampEnd);
    return false;
  }
  auto begin = index.lower_bound(timestampBegin);
  auto end = index.upper_bound(timestampEnd);

  hashesNumberWithinTimestamps = static_cast<uint32_t>(std::distance(begin, end));

  for (auto iter = begin; iter != end && hashesNumber < hashesNumberLimit; ++iter){
    ++hashesNumber;
    hashes.emplace_back(iter->second);
  }
  return hashesNumber > 0;
}

void TimestampBlocksIndex::clear() {
  index.clear();
}

void TimestampBlocksIndex::serialize(ISerializer& s) {
  s(index, "index");
}

bool TimestampTransactionsIndex::add(uint64_t timestamp, const Crypto::Hash& hash) {
  index.emplace(timestamp, hash);
  return true;
}

bool TimestampTransactionsIndex::remove(uint64_t timestamp, const Crypto::Hash& hash) {
  auto range = index.equal_range(timestamp);
  for (auto iter = range.first; iter != range.second; ++iter) {
    if (iter->second == hash) {
      index.erase(iter);
      return true;
    }
  }

  return false;
}

bool TimestampTransactionsIndex::find(uint64_t timestampBegin, uint64_t timestampEnd, uint64_t hashesNumberLimit, std::vector<Crypto::Hash>& hashes, uint64_t& hashesNumberWithinTimestamps) {
  uint32_t hashesNumber = 0;
  if (timestampBegin > timestampEnd) {
    //std::swap(timestampBegin, timestampEnd);
    return false;
  }
  auto begin = index.lower_bound(timestampBegin);
  auto end = index.upper_bound(timestampEnd);

  hashesNumberWithinTimestamps = static_cast<uint32_t>(std::distance(begin, end));

  for (auto iter = begin; iter != end && hashesNumber < hashesNumberLimit; ++iter) {
    ++hashesNumber;
    hashes.emplace_back(iter->second);
  }
  return hashesNumber > 0;
}

void TimestampTransactionsIndex::clear() {
  index.clear();
}

void TimestampTransactionsIndex::serialize(ISerializer& s) {
  s(index, "index");
}

GeneratedTransactionsIndex::GeneratedTransactionsIndex() : lastGeneratedTxNumber(0) {

}

bool GeneratedTransactionsIndex::add(const Block& block) {
  uint32_t blockHeight = boost::get<BaseInput>(block.baseTransaction.inputs.front()).blockIndex;

  if (index.size() != blockHeight) {
    return false;
  } 

  bool status = index.emplace(blockHeight, lastGeneratedTxNumber + block.transactionHashes.size() + 1).second; //Plus miner tx
  if (status) {
    lastGeneratedTxNumber += block.transactionHashes.size() + 1;
  }
  return status;
}

bool GeneratedTransactionsIndex::remove(const Block& block) {
  uint32_t blockHeight = boost::get<BaseInput>(block.baseTransaction.inputs.front()).blockIndex;

  if (blockHeight != index.size() - 1) {
    return false;
  }

  auto iter = index.find(blockHeight);
  assert(iter != index.end());
  index.erase(iter);

  if (blockHeight != 0) {
    iter = index.find(blockHeight - 1);
    assert(iter != index.end());
    lastGeneratedTxNumber = iter->second;
  } else {
    lastGeneratedTxNumber = 0;
  }
  
  return true;
}

bool GeneratedTransactionsIndex::find(uint32_t height, uint64_t& generatedTransactions) {
  if (height > std::numeric_limits<uint32_t>::max()) {
    return false;
  }
  auto iter = index.find(height);
  if (iter == index.end()) {
    return false;
  }
  generatedTransactions = iter->second;
  return true;
}

void GeneratedTransactionsIndex::clear() {
  index.clear();
}

void GeneratedTransactionsIndex::serialize(ISerializer& s) {
  s(index, "index");
  s(lastGeneratedTxNumber, "lastGeneratedTxNumber");
}

bool OrphanBlocksIndex::add(const Block& block) {
  Crypto::Hash blockHash = get_block_hash(block);
  uint32_t blockHeight = boost::get<BaseInput>(block.baseTransaction.inputs.front()).blockIndex;
  index.emplace(blockHeight, blockHash);
  return true;
}

bool OrphanBlocksIndex::remove(const Block& block) {
  Crypto::Hash blockHash = get_block_hash(block);
  uint32_t blockHeight = boost::get<BaseInput>(block.baseTransaction.inputs.front()).blockIndex;
  auto range = index.equal_range(blockHeight);
  for (auto iter = range.first; iter != range.second; ++iter) {
    if (iter->second == blockHash) {
      index.erase(iter);
      return true;
    }
  }

  return false;
}

bool OrphanBlocksIndex::find(uint32_t height, std::vector<Crypto::Hash>& blockHashes) {
  if (height > std::numeric_limits<uint32_t>::max()) {
    return false;
  }
  bool found = false;
  auto range = index.equal_range(height);
  for (auto iter = range.first; iter != range.second; ++iter) {
    found = true;
    blockHashes.emplace_back(iter->second);
  }
  return found;
}

void OrphanBlocksIndex::clear() {
  index.clear();
}

}
