/* $NetBSD: print.h,v 0.01 2013/10/05 20:10:00 Weiyu Exp $ */
 
/* Copyright (c) 2013, Weiyu Xue
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met:
 * 1. Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in 
 *    the documentation and/or other materials provided with the distribution.
 * 3. The names of its contributors may be used to endorse or promote 
 *    products derived from this software without specific prior written 
 *    permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY WEIYU XUE "AS IS" AND ANY EXPRESS OR 
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL WEIYU XUE BE LIABLE FOR ANY DIRECT, 
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef _S_IFWHT
#define _S_IFWHT  0160000   /* whiteout */
#endif
#ifndef S_ISWHT(m)
#define  S_ISWHT(m)  ((m & _S_IFMT) == _S_IFWHT)
#endif

#ifndef _S_ARCH1
#define _S_ARCH1  0200000   /* Archive state 1, ls -l shows 'a' */
#endif
#define S_ISARCH1(m) ((m & _S_ARCH1) == _S_ARCH1)

#ifndef _S_ARCH2
#define _S_ARCH2  0400000   /* Archive state 2, ls -l shows 'A' */
#endif
#define S_ISARCH2(m) ((m & _S_ARCH2) == _S_ARCH2)


void printpermissions(mode_t);
void printownername(uid_t);
void printgroupname(gid_t);
void printmtime(time_t);
void printindicator(mode_t);