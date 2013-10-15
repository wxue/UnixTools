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
#include <string.h>
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
printmodes(mode_t md)
{
  char buf[10];
  (void)strmode(md, buf);
  printf("%s", buf);
}

char*
getownername(uid_t uid)
{
  struct passwd * pwdp;

  if ((pwdp = getpwuid(uid)) == NULL) {
    fprintf(stderr, "Failed to get owner name from uid: %d\n",
            uid);
    exit(EXIT_FAILURE);
  } 
  else 
    return(pwdp->pw_name);
}

char*
getgroupname(gid_t gid)
{
  struct group *grp;

  if ((grp = getgrgid(gid)) == NULL) {
    fprintf(stderr, "Failed to get group name from gid: %-8d\n", 
            gid);
    exit(EXIT_FAILURE);
  }
  else
    return(grp->gr_name);
}

void
printtime(time_t ptime)
{
  struct tm *ts;
  char buf[80];

  ts = localtime(&ptime);
  strftime(buf, sizeof(buf), "%b %d %H:%M ", ts);
  printf("%s ", buf);
}

void
printhsize(off_t size)
{
  if(size/1152921504606846976 > 1024) {
    fprintf(stderr, "File size too huge.");
    exit(EXIT_FAILURE);
  }

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
  printf( (S_ISLNK(md))   ? "@" :
          (S_ISDIR(md))   ? "/" :
          (md & S_IXOTH)  ? "*" :
          (S_ISSOCK(md))  ? "=" :
          (S_ISFIFO(md))  ? "|" :
          (S_ISWHT(md))   ? "%%" :
          "?");
}

void
maxprint(long long value, int maxvalue)
{
  if(maxvalue/100000000000)
    printf("%12lld ", value);
  else if(maxvalue/10000000000)
    printf("%11lld ", value);
  else if(maxvalue/1000000000)
    printf("%10lld ", value);
  else if(maxvalue/100000000)
    printf("%9lld ", value);
  else if(maxvalue/10000000)
    printf("%8lld ", value);
  else if(maxvalue/1000000)
    printf("%7lld ", value);
  else if(maxvalue/100000)
    printf("%6lld ", value);
  else if(maxvalue/10000)
    printf("%5lld ", value);
  else if(maxvalue/1000)
    printf("%4lld ", value);
  else if(maxvalue/100)
    printf("%3lld ", value);
  else
    printf("%2lld ", value);

}

void
maxlenprint(char* value, int maxlen)
{
  char* p;
  int lack;
  p=(char*)malloc(maxlen*sizeof(char));
  if(p != NULL) {
    lack = maxlen - strlen(value);
    strcpy(p, value);
    while(lack != 0) {
      strcat(p, " ");
      lack--;
    }
    printf("%s ", p);
  }
  else {
    fprintf(stderr, "malloc error!\n");
    exit(EXIT_FAILURE);
  }
  free(p);
  p=NULL;
}