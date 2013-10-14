/* $NetBSD: ls.c,v 0.03 2013/10/13 23:40:40 Weiyu Exp $ */
 
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

#define NAME_SORT 1
#define TIME_SORT 2 
#define SIZE_SORT 3 

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
// int flag_S = 0; 
int flag_s = 0; 
// int flag_t = 0; 
int flag_u = 0; 
int flag_w = 0; 
int flag_1 = 0; 

int sortype = NAME_SORT;

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

  if(sortype == TIME_SORT) {
    if(flag_c)
      result = ctimecmp(*p1, *p2);
    else if(flag_u)
      result = atimecmp(*p1, *p2);
    else
      result = mtimecmp(*p1, *p2);
  }

  else if(sortype == SIZE_SORT)
    result = sizecmp(*p1, *p2);
  
  else /* sortype == NAME_SORT by default */
    result = namecmp(*p1, *p2);

  if(flag_r)
    result = -result;
  return(result);
}

void
display(FTSENT *p, FTSENT *list)
{
  FTSENT *listp;
  blkcnt_t  total_b;
  total_b = 0;

  if(flag_l || flag_n) {
    for (listp = list; listp; listp = listp->fts_link) {
      if (!(flag_a || flag_A) && listp->fts_name[0] == '.')
        continue;
      total_b += listp->fts_statp->st_blocks;
    }
    printf("total: %lld\n", total_b); 
  }

  for (listp = list; listp; listp = listp->fts_link) {
    if (!(flag_a || flag_A) && listp->fts_name[0] == '.')
      continue;
    if(flag_l || flag_n) {
      // printf("fts_number: %ld  ", listp->fts_number);
      if(flag_i)
        printf("%lld ", listp->fts_statp->st_ino);
      printpermissions(listp->fts_statp->st_mode);
      printf("  %2d", listp->fts_statp->st_nlink);

      if(flag_n)
        printf("%d %d", listp->fts_statp->st_uid, listp->fts_statp->st_gid);
      else {
        printownername(listp->fts_statp->st_uid);
        printgroupname(listp->fts_statp->st_gid);
      }
          
      /* size */
      if(flag_h)
        printhsize(listp->fts_statp->st_size);
      else if(flag_k)
        printksize(listp->fts_statp->st_size);
      else 
        printf("%6lld ", listp->fts_statp->st_size);
      /* time */
      if(flag_u)
        printime(listp->fts_statp->st_atime);
      else if(flag_c)
        printime(listp->fts_statp->st_ctime);
      else /* mtime by default */
        printime(listp->fts_statp->st_mtime);
    }
    if(flag_F) {
      printf("%s", listp->fts_name);
      printindicator(listp->fts_statp->st_mode);
    }
    else
      printf("%s\n", listp->fts_name);
  }
}

void
traverse(int argc, char *argv[], int p_options, int chp_options)
{
  FTS *ftsp;
  FTSENT *p, *chp;
  int wrap = 0;

  if ((ftsp = fts_open(argv, p_options, flag_f ? NULL : setcmp)) == NULL) {
    fprintf(stderr, "No such file or directory");
    exit(EXIT_FAILURE);
  }
  
  // chp = fts_children(ftsp, 0);
  // if (chp == NULL) {
  //   printf("no files");
  //   exit(EXIT_SUCCESS);
  // }

  while ((p = fts_read(ftsp)) != NULL) {
    switch (p->fts_info) {
    case FTS_D:
      if (p->fts_level != 0)
        break;
      if(flag_d) {
        if(flag_l || flag_n) {
          if(flag_i)
            printf("%lld ", p->fts_statp->st_ino);
          printpermissions(p->fts_statp->st_mode);
          printf("  %2d", p->fts_statp->st_nlink);

          if(flag_n)
            printf("%d %d", p->fts_statp->st_uid, p->fts_statp->st_gid);
          else {
            printownername(p->fts_statp->st_uid);
            printgroupname(p->fts_statp->st_gid);
          }

          /* size */
          if(flag_h)
            printhsize(p->fts_statp->st_size);
          else if(flag_k)
            printksize(p->fts_statp->st_size);
          else
            printf("%6lld ", p->fts_statp->st_size);
          /* time */
          if(flag_u)
            printime(p->fts_statp->st_atime);
          else if(flag_c)
            printime(p->fts_statp->st_ctime);
          else /* mtime by default */
            printime(p->fts_statp->st_mtime);
        }
        printf("%s\n", p->fts_path);
        break;
      }
      // if (!(flag_a || flag_A) && p->fts_name[0] == '.')
      //   break;

      if (wrap)
        printf("\n%s:\n", p->fts_path);
      else if (argc > 1) {
        printf("%s:\n", p->fts_path);
        wrap = 1;
      }

      chp = fts_children(ftsp, chp_options);
      display(p, chp);

    default:
      break;
    }
  }
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
  int p_options;
  int chp_options;
  // p_options = FTS_COMFOLLOW | FTS_LOGICAL | FTS_NOCHDIR;

  /* This works on BSD, but not in the stdlib on Linux. */
  setprogname(argv[0]);

   // "−AacdFfhiklnqRrSstuw1"
  while ((ch = getopt(argc, argv, "−AacdFfhiklnrStu")) != -1) {
    switch (ch) {   /* Indent the switch. */
    case 'A':
      flag_A = 1;
      break;

    case 'a':
      flag_a = 1;
      p_options |= FTS_SEEDOT;
      break;

    /*
     * The −c and −u options override each other;
     * the last one speciﬁed determines the ﬁle time used.
     */
    case 'c':
      flag_c = 1;
      flag_u = 0;
      break;

    case 'd':
      flag_d = 1;
      break;

    case 'F':
      flag_F = 1;
      break;

    case 'f':
      flag_f = 1;
      break;

    case 'h':
      flag_h = 1;
      break;

    case 'i':
      flag_i = 1;
      break;

    case 'k':
      flag_k = 1;
      break;

    case 'l':
      flag_l = 1;
      break;

    case 'n':
      flag_n = 1;
      break;

    case 'r':
      flag_r = 1;
      break;

    case 'S':
      sortype = SIZE_SORT;
      break;

    case 't':
      sortype = TIME_SORT;
      break;

    /*
     * The −c and −u options override each other;
     * the last one speciﬁed determines the ﬁle time used.
     */
    case 'u':
      flag_u = 1;
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

  traverse(argc, argv, p_options, chp_options);

  return 0;
}