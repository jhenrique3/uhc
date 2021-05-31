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


#include "RpcServerConfig.h"
#include "Common/CommandLine.h"
#include "CryptoNoteConfig.h"

namespace CryptoNote {

  namespace {

    const std::string DEFAULT_RPC_IP = "127.0.0.1";
    const uint16_t DEFAULT_RPC_PORT = RPC_DEFAULT_PORT;

    const command_line::arg_descriptor<std::string> arg_rpc_bind_ip = { "rpc-bind-ip", "", DEFAULT_RPC_IP };
    const command_line::arg_descriptor<uint16_t> arg_rpc_bind_port = { "rpc-bind-port", "", DEFAULT_RPC_PORT };
  }


  RpcServerConfig::RpcServerConfig() : bindIp(DEFAULT_RPC_IP), bindPort(DEFAULT_RPC_PORT) {
  }

  std::string RpcServerConfig::getBindAddress() const {
    return bindIp + ":" + std::to_string(bindPort);
  }
  
  void RpcServerConfig::initOptions(boost::program_options::options_description& desc) {
    command_line::add_arg(desc, arg_rpc_bind_ip);
    command_line::add_arg(desc, arg_rpc_bind_port);
  }

  void RpcServerConfig::init(const boost::program_options::variables_map& vm)  {
    bindIp = command_line::get_arg(vm, arg_rpc_bind_ip);
    bindPort = command_line::get_arg(vm, arg_rpc_bind_port);
  }

}
