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


#include "PaymentServiceJsonRpcMessages.h"
#include "Serialization/SerializationOverloads.h"

namespace PaymentService {

void Reset::Request::serialize(CryptoNote::ISerializer& serializer) {
  serializer(viewSecretKey, "viewSecretKey");
}

void Reset::Response::serialize(CryptoNote::ISerializer& serializer) {
}

void GetViewKey::Request::serialize(CryptoNote::ISerializer& serializer) {
}

void GetViewKey::Response::serialize(CryptoNote::ISerializer& serializer) {
  serializer(viewSecretKey, "viewSecretKey");
}

void GetStatus::Request::serialize(CryptoNote::ISerializer& serializer) {
}

void GetStatus::Response::serialize(CryptoNote::ISerializer& serializer) {
  serializer(blockCount, "blockCount");
  serializer(knownBlockCount, "knownBlockCount");
  serializer(lastBlockHash, "lastBlockHash");
  serializer(peerCount, "peerCount");
}

void GetAddresses::Request::serialize(CryptoNote::ISerializer& serializer) {
}

void GetAddresses::Response::serialize(CryptoNote::ISerializer& serializer) {
  serializer(addresses, "addresses");
}

void CreateAddress::Request::serialize(CryptoNote::ISerializer& serializer) {
  bool hasSecretKey = serializer(spendSecretKey, "spendSecretKey");
  bool hasPublicKey = serializer(spendPublicKey, "spendPublicKey");

  if (hasSecretKey && hasPublicKey) {
    //TODO: replace it with error codes
    throw RequestSerializationError();
  }
}

void CreateAddress::Response::serialize(CryptoNote::ISerializer& serializer) {
  serializer(address, "address");
}

void DeleteAddress::Request::serialize(CryptoNote::ISerializer& serializer) {
  if (!serializer(address, "address")) {
    throw RequestSerializationError();
  }
}

void DeleteAddress::Response::serialize(CryptoNote::ISerializer& serializer) {
}

void GetSpendKeys::Request::serialize(CryptoNote::ISerializer& serializer) {
  if (!serializer(address, "address")) {
    throw RequestSerializationError();
  }
}

void GetSpendKeys::Response::serialize(CryptoNote::ISerializer& serializer) {
  serializer(spendSecretKey, "spendSecretKey");
  serializer(spendPublicKey, "spendPublicKey");
}

void GetBalance::Request::serialize(CryptoNote::ISerializer& serializer) {
  serializer(address, "address");
}

void GetBalance::Response::serialize(CryptoNote::ISerializer& serializer) {
  serializer(availableBalance, "availableBalance");
  serializer(lockedAmount, "lockedAmount");
}

void GetBlockHashes::Request::serialize(CryptoNote::ISerializer& serializer) {
  bool r = serializer(firstBlockIndex, "firstBlockIndex");
  r &= serializer(blockCount, "blockCount");

  if (!r) {
    throw RequestSerializationError();
  }
}

void GetBlockHashes::Response::serialize(CryptoNote::ISerializer& serializer) {
  serializer(blockHashes, "blockHashes");
}

void TransactionHashesInBlockRpcInfo::serialize(CryptoNote::ISerializer& serializer) {
  serializer(blockHash, "blockHash");
  serializer(transactionHashes, "transactionHashes");
}

void GetTransactionHashes::Request::serialize(CryptoNote::ISerializer& serializer) {
  serializer(addresses, "addresses");

  if (serializer(blockHash, "blockHash") == serializer(firstBlockIndex, "firstBlockIndex")) {
    throw RequestSerializationError();
  }

  if (!serializer(blockCount, "blockCount")) {
    throw RequestSerializationError();
  }

  serializer(paymentId, "paymentId");
}

void GetTransactionHashes::Response::serialize(CryptoNote::ISerializer& serializer) {
  serializer(items, "items");
}

void TransferRpcInfo::serialize(CryptoNote::ISerializer& serializer) {
  serializer(type, "type");
  serializer(address, "address");
  serializer(amount, "amount");
}

void TransactionRpcInfo::serialize(CryptoNote::ISerializer& serializer) {
  serializer(state, "state");
  serializer(transactionHash, "transactionHash");
  serializer(blockIndex, "blockIndex");
  serializer(timestamp, "timestamp");
  serializer(isBase, "isBase");
  serializer(unlockTime, "unlockTime");
  serializer(amount, "amount");
  serializer(fee, "fee");
  serializer(transfers, "transfers");
  serializer(extra, "extra");
  serializer(paymentId, "paymentId");
}

void GetTransaction::Request::serialize(CryptoNote::ISerializer& serializer) {
  if (!serializer(transactionHash, "transactionHash")) {
    throw RequestSerializationError();
  }
}

void GetTransaction::Response::serialize(CryptoNote::ISerializer& serializer) {
  serializer(transaction, "transaction");
}

void TransactionsInBlockRpcInfo::serialize(CryptoNote::ISerializer& serializer) {
  serializer(blockHash, "blockHash");
  serializer(transactions, "transactions");
}

void GetTransactions::Request::serialize(CryptoNote::ISerializer& serializer) {
  serializer(addresses, "addresses");

  if (serializer(blockHash, "blockHash") == serializer(firstBlockIndex, "firstBlockIndex")) {
    throw RequestSerializationError();
  }

  if (!serializer(blockCount, "blockCount")) {
    throw RequestSerializationError();
  }

  serializer(paymentId, "paymentId");
}

void GetTransactions::Response::serialize(CryptoNote::ISerializer& serializer) {
  serializer(items, "items");
}

void GetUnconfirmedTransactionHashes::Request::serialize(CryptoNote::ISerializer& serializer) {
  serializer(addresses, "addresses");
}

void GetUnconfirmedTransactionHashes::Response::serialize(CryptoNote::ISerializer& serializer) {
  serializer(transactionHashes, "transactionHashes");
}

void WalletRpcOrder::serialize(CryptoNote::ISerializer& serializer) {
  bool r = serializer(address, "address");
  r &= serializer(amount, "amount");

  if (!r) {
    throw RequestSerializationError();
  }
}

void SendTransaction::Request::serialize(CryptoNote::ISerializer& serializer) {
  serializer(sourceAddresses, "addresses");

  if (!serializer(transfers, "transfers")) {
    throw RequestSerializationError();
  }

  serializer(changeAddress, "changeAddress");

  if (!serializer(fee, "fee")) {
    throw RequestSerializationError();
  }

  if (!serializer(anonymity, "anonymity")) {
    throw RequestSerializationError();
  }

  bool hasExtra = serializer(extra, "extra");
  bool hasPaymentId = serializer(paymentId, "paymentId");

  if (hasExtra && hasPaymentId) {
    throw RequestSerializationError();
  }

  serializer(unlockTime, "unlockTime");
}

void SendTransaction::Response::serialize(CryptoNote::ISerializer& serializer) {
  serializer(transactionHash, "transactionHash");
}

void CreateDelayedTransaction::Request::serialize(CryptoNote::ISerializer& serializer) {
  serializer(addresses, "addresses");

  if (!serializer(transfers, "transfers")) {
    throw RequestSerializationError();
  }

  serializer(changeAddress, "changeAddress");

  if (!serializer(fee, "fee")) {
    throw RequestSerializationError();
  }

  if (!serializer(anonymity, "anonymity")) {
    throw RequestSerializationError();
  }

  bool hasExtra = serializer(extra, "extra");
  bool hasPaymentId = serializer(paymentId, "paymentId");

  if (hasExtra && hasPaymentId) {
    throw RequestSerializationError();
  }

  serializer(unlockTime, "unlockTime");
}

void CreateDelayedTransaction::Response::serialize(CryptoNote::ISerializer& serializer) {
  serializer(transactionHash, "transactionHash");
}

void GetDelayedTransactionHashes::Request::serialize(CryptoNote::ISerializer& serializer) {
}

void GetDelayedTransactionHashes::Response::serialize(CryptoNote::ISerializer& serializer) {
  serializer(transactionHashes, "transactionHashes");
}

void DeleteDelayedTransaction::Request::serialize(CryptoNote::ISerializer& serializer) {
  if (!serializer(transactionHash, "transactionHash")) {
    throw RequestSerializationError();
  }
}

void DeleteDelayedTransaction::Response::serialize(CryptoNote::ISerializer& serializer) {
}

void SendDelayedTransaction::Request::serialize(CryptoNote::ISerializer& serializer) {
  if (!serializer(transactionHash, "transactionHash")) {
    throw RequestSerializationError();
  }
}

void SendDelayedTransaction::Response::serialize(CryptoNote::ISerializer& serializer) {
}

}
