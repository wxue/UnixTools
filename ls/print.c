/* $NetBSD: print.c,v 0.01 2013/10/05 20:10:00 Weiyu Exp $ */
 
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

#include <stdio.h>
#include <stdlib.h>
#include <fts.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

#include "print.h"

void
printpermissions(mode_t md)
{
  printf( (S_ISARCH1(md)) ? "a" :
          (S_ISARCH2(md)) ? "A" :
          (S_ISBLK(md))   ? "b" :
          (S_ISCHR(md))   ? "c" :
          (S_ISDIR(md))   ? "d" :
          (S_ISLNK(md))   ? "l" :
          (S_ISSOCK(md))  ? "s" :
          (S_ISFIFO(md))  ? "p" :
          (S_ISWHT(md))   ? "w" :
          (S_ISREG(md))   ? "-" :
          "?");
  // strmode() is another way to do this

  printf( (md & S_IRUSR) ? "r" : "-");
  printf( (md & S_IWUSR) ? "w" : "-");
  printf( (md & S_IXUSR) ? "x" : "-");
  printf( (md & S_IRGRP) ? "r" : "-");
  printf( (md & S_IWGRP) ? "w" : "-");
  printf( (md & S_IXGRP) ? "x" : "-");
  printf( (md & S_IROTH) ? "r" : "-");
  printf( (md & S_IWOTH) ? "w" : "-");
  printf( (md & S_IXOTH) ? "x" : "-");
}

void
printownername(uid_t uid)
{
  struct passwd * pwdp;

  if ((pwdp = getpwuid(uid)) == NULL) {
    fprintf(stderr, "Failed get owner name from uid: %d\n",
            uid);
    exit(EXIT_FAILURE);
  } 
  else 
    printf(" %s ", pwdp->pw_name);
}

void
printgroupname(gid_t gid)
{
  struct group *grp;

  if ((grp = getgrgid(gid)) == NULL) {
    fprintf(stderr, "Failed get group name from gid: %-8d\n", 
            gid);
    exit(EXIT_FAILURE);
  }
  else
    printf(" %-8.8s", grp->gr_name);
}

void
printmtime(time_t mtime)
{
  struct tm *ts;
  char buf[80];

  ts = localtime(&mtime);
  strftime(buf, sizeof(buf), "%b %d %H:%M ", ts);
  printf("%s", buf);
}