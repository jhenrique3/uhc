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

#define	setcontext(u)	setmcontext(&(u)->uc_mcontext)
#define	getcontext(u)	getmcontext(&(u)->uc_mcontext)

#ifdef __cplusplus
extern "C" {
#endif
#include <stdlib.h>

typedef struct mcontext mctx;
typedef struct ucontext uctx;

extern	int		swapcontext(uctx*, const uctx*);
extern	void		makecontext(uctx*, void(*)(), intptr_t);
extern	int		getmcontext(mctx*);
extern	void		setmcontext(const mctx*);

struct mcontext {
  /*
   * The first 20 fields must match the definition of
   * sigcontext. So that we can support sigcontext
   * and ucontext_t at the same time.
   */
  long	mc_onstack;		/* XXX - sigcontext compat. */
  long	mc_rdi;			/* machine state (struct trapframe) */
  long	mc_rsi;
  long	mc_rdx;
  long	mc_rcx;
  long	mc_r8;
  long	mc_r9;
  long	mc_rax;
  long	mc_rbx;
  long	mc_rbp;
  long	mc_r10;
  long	mc_r11;
  long	mc_r12;
  long	mc_r13;
  long	mc_r14;
  long	mc_r15;
  long	mc_trapno;
  long	mc_addr;
  long	mc_flags;
  long	mc_err;
  long	mc_rip;
  long	mc_cs;
  long	mc_rflags;
  long	mc_rsp;
  long	mc_ss;
  
  long	mc_len;			/* sizeof(mcontext_t) */
#define	_MC_FPFMT_NODEV		0x10000	/* device not present or configured */
#define	_MC_FPFMT_XMM		0x10002
  long	mc_fpformat;
#define	_MC_FPOWNED_NONE	0x20000	/* FP state not used */
#define	_MC_FPOWNED_FPU		0x20001	/* FP state came from FPU */
#define	_MC_FPOWNED_PCB		0x20002	/* FP state came from PCB */
  long	mc_ownedfp;
  /*
   * See <machine/fpu.h> for the internals of mc_fpstate[].
   */
  long	mc_fpstate[64];
  long	mc_spare[8];
};

struct ucontext {
  /*
   * Keep the order of the first two fields. Also,
   * keep them the first two fields in the structure.
   * This way we can have a union with struct
   * sigcontext and ucontext_t. This allows us to
   * support them both at the same time.
   * note: the union is not defined, though.
   */
  sigset_t	uc_sigmask;
  mctx	uc_mcontext;
  
  struct __ucontext *uc_link;
  stack_t		uc_stack;
  int		__spare__[8];
};
  
#ifdef __cplusplus
}
#endif
