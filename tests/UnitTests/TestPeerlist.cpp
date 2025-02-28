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


#include "gtest/gtest.h"

#include "Common/Util.h"

#include "P2p/PeerListManager.h"
#include "P2p/PeerListManager.cpp"

using namespace CryptoNote;

#define MAKE_IP( a1, a2, a3, a4 )	(a1|(a2<<8)|(a3<<16)|(a4<<24))


TEST(peer_list, peer_list_general)
{
  CryptoNote::PeerlistManager plm;
  plm.init(false);
#define ADD_GRAY_NODE(ip_, port_, id_, last_seen_) {  PeerlistEntry ple; ple.last_seen=last_seen_;ple.adr.ip = ip_; ple.adr.port = port_; ple.id = id_;plm.append_with_peer_gray(ple);}  
#define ADD_WHITE_NODE(ip_, port_, id_, last_seen_) {  PeerlistEntry ple;ple.last_seen=last_seen_; ple.adr.ip = ip_; ple.adr.port = port_; ple.id = id_;plm.append_with_peer_white(ple);}  

#define PRINT_HEAD(step) {std::list<PeerlistEntry> bs_head; bool r = plm.get_peerlist_head(bs_head, 100);std::cout << "step " << step << ": " << bs_head.size() << std::endl;}

  ADD_GRAY_NODE(MAKE_IP(123,43,12,1), 8080, 121241, 34345);
  ADD_GRAY_NODE(MAKE_IP(123,43,12,2), 8080, 121241, 34345);
  ADD_GRAY_NODE(MAKE_IP(123,43,12,3), 8080, 121241, 34345);
  ADD_GRAY_NODE(MAKE_IP(123,43,12,4), 8080, 121241, 34345);
  ADD_GRAY_NODE(MAKE_IP(123,43,12,5), 8080, 121241, 34345);

  ADD_WHITE_NODE(MAKE_IP(123,43,12,1), 8080, 121241, 34345);
  ADD_WHITE_NODE(MAKE_IP(123,43,12,2), 8080, 121241, 34345);
  ADD_WHITE_NODE(MAKE_IP(123,43,12,3), 8080, 121241, 34345);
  ADD_WHITE_NODE(MAKE_IP(123,43,12,4), 8080, 121241, 34345);

  size_t gray_list_size = plm.get_gray_peers_count();
  ASSERT_EQ(gray_list_size, 1);

  std::list<PeerlistEntry> bs_head;
  bool r = plm.get_peerlist_head(bs_head, 100);
  std::cout << bs_head.size() << std::endl;
  ASSERT_TRUE(r);

  ASSERT_EQ(bs_head.size(), 4);


  ADD_GRAY_NODE(MAKE_IP(123,43,12,5), 8080, 121241, 34345);
  ASSERT_EQ(plm.get_gray_peers_count(), 1);
  ASSERT_EQ(plm.get_white_peers_count(), 4);
}


TEST(peer_list, merge_peer_lists)
{
  //([^ \t]*)\t([^ \t]*):([^ \t]*) \tlast_seen: d(\d+)\.h(\d+)\.m(\d+)\.s(\d+)\n
  //ADD_NODE_TO_PL("\2", \3, 0x\1, (1353346618 -(\4*60*60*24+\5*60*60+\6*60+\7 )));\n
  PeerlistManager plm;
  plm.init(false);
  std::list<PeerlistEntry> outer_bs;


}
