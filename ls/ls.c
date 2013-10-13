/* $NetBSD: ls.c,v 0.02 2013/10/05 20:10:00 Weiyu Exp $ */
 
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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <err.h>
#include <errno.h>
#include <string.h>
#include <fts.h>

#include "print.h"
#include "cmp.h"

/*
 * Flags
 */

int flag_A = 0; 
int flag_a = 0; 
int flag_c = 0; 
int flag_d = 0; 
int flag_F = 0; 
int flag_f = 0; 
int flag_h = 0; 
int flag_i = 0; 
int flag_k = 0; 
int flag_l = 0; 
int flag_n = 0; 
int flag_q = 0; 
int flag_R = 0; 
int flag_r = 0; 
int flag_S = 0; 
int flag_s = 0; 
int flag_t = 0; 
int flag_u = 0; 
int flag_w = 0; 
int flag_1 = 0; 

/*
 * Functions
 */
void
usage()
{
  fprintf(stderr, "Usage: %s [ −AacdFfhiklnqRrSstuw1] [file . . .]\n",
          getprogname());
  exit(EXIT_FAILURE);
  /* NOTREACHED */
}

int
setcmp(const FTSENT **p1, const FTSENT **p2)
{
  int result;

  if(flag_c)
    result = ctimecmp(*p1, *p2);
  if(flag_u)
    result = atimecmp(*p1, *p2);
  if(flag_t)
    result = mtimecmp(*p1, *p2);
  else
    result = namecmp(*p1, *p2);
  
  if(flag_r)
    result = -result;
  return(result);
}

void
traverse(int argc, char *argv[], int fts_options)
{
  FTS *ftsp;
  FTSENT *p, *chp;
  blkcnt_t  total_b;
  total_b = 0;

  if ((ftsp = fts_open(argv, fts_options, flag_f ? NULL : setcmp)) == NULL) {
    fprintf(stderr, "No such file or directory");
    exit(EXIT_FAILURE);
  }
  
  chp = fts_children(ftsp, 0);
  if (chp == NULL) {
    printf("no files");
    exit(EXIT_SUCCESS);
  }

  while ((p = fts_read(ftsp)) != NULL) {
    switch (p->fts_info) {
    case FTS_DOT:
      if (flag_A && !flag_a)
        break;
      /* FALLTHROUGH */ 
    case FTS_SL:
      /* FALLTHROUGH */
    case FTS_D:
      if(flag_d) {
          if (p->fts_level != 0)
            break;
          if(flag_l) {
          printpermissions(p->fts_statp->st_mode);
          printf("  %2d", p->fts_statp->st_nlink);
          printownername(p->fts_statp->st_uid);
          printgroupname(p->fts_statp->st_gid);
          printf("%6lld ", p->fts_statp->st_size);
          printmtime(p->fts_statp->st_mtime);
          }
          printf("%s\n", p->fts_path);
        }
      /* FALLTHROUGH */
    case FTS_F:
      if(flag_d)
        break;
      if (p->fts_level != 1)
        break;
      if (!(flag_a || flag_A) && p->fts_name[0] == '.')
        break;
      if (flag_l) {
        printpermissions(p->fts_statp->st_mode);
        printf("  %2d", p->fts_statp->st_nlink);
        printownername(p->fts_statp->st_uid);
        printgroupname(p->fts_statp->st_gid);
        printf("%6lld ", p->fts_statp->st_size);
        printmtime(p->fts_statp->st_mtime);
        total_b += p->fts_statp->st_blocks;
      }
      printf("%s\n", p->fts_name);
      break;

    default:
      break;
    }
  }
  if(flag_l && !flag_d)
    printf("total: %lld\n", total_b);
  fts_close(ftsp);

  exit(EXIT_SUCCESS);
}

/*
 * Main
 */
int 
main(int argc, char *argv[])
{
  // DIR *dp;
  // struct dirent *dirp;
  int ch;
  int fts_options;
  // fts_options = FTS_COMFOLLOW | FTS_LOGICAL | FTS_NOCHDIR;

  /* This works on BSD, but not in the stdlib on Linux. */
  setprogname(argv[0]);

   // "−AacdFfhiklnqRrSstuw1"
  while ((ch = getopt(argc, argv, "−Aacdflrtu")) != -1) {
    switch (ch) {   /* Indent the switch. */
    case 'A':
      flag_A = 1;
      fts_options |= FTS_SEEDOT;
      break;

    case 'a':
      flag_a = 1;
      fts_options |= FTS_SEEDOT;
      break;

    case 'c':
      flag_c = 1;
      /*
       * The −c and −u options override each other; they both override -t;
       * the last one speciﬁed(-c/-u) determines the ﬁle time used.
       */
      if(flag_t)
        flag_t = 0;
      if(flag_u)
        flag_u = 0;
      break;

    case 'd':
      flag_d = 1;
      break;

    case 'f':
      flag_f = 1;
      break;

    case 'l':
      flag_l = 1;
      break;

    case 'r':
      flag_r = 1;
      break;

    case 't':
      flag_t = 1;
      break;

    case 'u':
      flag_u = 1;
      /*
       * The −c and −u options override each other; they both override -t;
       * the last one speciﬁed(-c/-u) determines the ﬁle time used.
       */
      if(flag_t)
        flag_t = 0;
      if(flag_c)
        flag_c = 0;
      break;

    // case '1':
    //   errno = 0;
    //   num = strtol(optarg, &ep, 10);
    //   if (num <= 0 || *ep != '\0' || (errno == ERANGE &&
    //       (num == LONG_MAX || num == LONG_MIN)) )
    //     errx(1, "illegal number -- %s", optarg);
    //   break;
    // case 'd':
      /* FALLTHROUGH */
  
    default:
    case '?':
      usage();
    }
  }
  argc -= optind;
  argv += optind;

  if (*argv == NULL)
    *argv = ".";

  /* 
   * Override Flags 
   */
  
  /* sudo@UID:0 */
  if (!getuid())
    flag_A = 1;

  traverse(argc, argv, fts_options);

  return 0;
}