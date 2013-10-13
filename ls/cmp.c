/* $NetBSD: cmp.c,v 0.01 2013/10/10 19:39:20 Weiyu Exp $ */
 
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
#include <sys/types.h>
#include <sys/stat.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fts.h>

int
namecmp(const FTSENT *p1, const FTSENT *p2)
{
  return (strcmp(p1->fts_name, p2->fts_name));
}

int
mtimecmp(const FTSENT *p1, const FTSENT *p2)
{
  if (p2->fts_statp->st_mtime > p1->fts_statp->st_mtime)
    return (1);
  else if (p2->fts_statp->st_mtime < p1->fts_statp->st_mtime)
    return (-1);
  else
    return (strcmp(p1->fts_name, p2->fts_name));
}

int
ctimecmp(const FTSENT *p1, const FTSENT *p2)
{
  if (p2->fts_statp->st_ctime > p1->fts_statp->st_ctime)
    return (1);
  else if (p2->fts_statp->st_ctime < p1->fts_statp->st_ctime)
    return (-1);
  else
    return (strcmp(p1->fts_name, p2->fts_name));
}

int
atimecmp(const FTSENT *p1, const FTSENT *p2)
{
  if (p2->fts_statp->st_atime > p1->fts_statp->st_atime)
    return (1);
  else if (p2->fts_statp->st_atime < p1->fts_statp->st_atime)
    return (-1);
  else
    return (strcmp(p1->fts_name, p2->fts_name));
}

int
sizecmp(const FTSENT *p1, const FTSENT *p2)
{
  if (p2->fts_statp->st_size > p1->fts_statp->st_size)
    return (1);
  else if (p2->fts_statp->st_size < p1->fts_statp->st_size)
    return (-1);
  else
    return (strcmp(p1->fts_name, p2->fts_name));
}