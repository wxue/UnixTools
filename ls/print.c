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
#include <math.h>
// #include <libutil.h>

#include "print.h"

#define B_BYTES 0
#define K_BYTES 1
#define M_BYTES 2
#define G_BYTES 3
#define T_BYTES 4
#define P_BYTES 5
#define E_BYTES 6

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
    fprintf(stderr, "Failed to get owner name from uid: %d\n",
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
    fprintf(stderr, "Failed to get group name from gid: %-8d\n", 
            gid);
    exit(EXIT_FAILURE);
  }
  else
    printf(" %-8.8s", grp->gr_name);
}

void
printime(time_t ptime)
{
  struct tm *ts;
  char buf[80];

  ts = localtime(&ptime);
  strftime(buf, sizeof(buf), "%b %d %H:%M ", ts);
  printf("%s", buf);
}

void
printhsize(off_t size)
{
  // char hbuf[19];
  // humanize_number(hbuf, sizeof(hbuf), size, "B", HN_AUTOSCALE, HN_DECIMAL);
  // printf("%s\n", hbuf);

  if(size/1152921504606846976)
    printf("%4lldE ", size/1152921504606846976);
  else if(size/1125899906842624)
    printf("%4lldP ", size/1125899906842624);
  else if(size/1099511627776)
    printf("%4lldT ", size/1099511627776);
  else if(size/1073741824)
    printf("%4lldG ", size/1073741824);
  else if(size/1048576)
    printf("%4lldM ", size/1048576);
  else if(size/1024)
    printf("%4lldK ", size/1024);
  else
    printf("%4lldB ", size);
}

void
printksize(off_t size)
{
  if(size != 0 && size/1024 == 0)
    printf("   1K ");
  else
    printf("%4lldK ", size/1024);
}

void
printindicator(mode_t md)
{
  printf( (S_ISLNK(md))   ? "@\n" :
          (S_ISDIR(md))   ? "/\n" :
          (md & S_IXOTH)  ? "*\n" :
          (S_ISSOCK(md))  ? "=\n" :
          (S_ISFIFO(md))  ? "|\n" :
          (S_ISWHT(md))   ? "%%\n" :
          "\n");
}