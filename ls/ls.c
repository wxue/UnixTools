/* $NetBSD: ls.c,v 0.01 2013/09/27 20:50:02 Weiyu Exp $ */
 
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
#include <errno.h>
#include <string.h>
#include <fts.h>

/*
 * Flags
 */

/* List all entries except for ‘.’ and ‘..’. Always set for the super-user. */
int flag_A = 0; 

/* Include directory entries whose names begin with a dot ( ‘.’ ) . */
int flag_a = 0; 

/* Use time when ﬁle status was last changed, instead of time of last 
   modiﬁcation of the ﬁle for sorting ( −t ) or printing ( −l ) . */
int flag_c = 0; 

/* Directories are listed as plain ﬁles (not searched recursively) and symbolic
   links in the argument list are not indirected through. */
int flag_d = 0; 

/* Display a slash ( ‘/’ ) immediately after each pathname that is a directory,
   an asterisk ( ‘∗’) after each that is executable, an at sign ( ‘@’ ) after
   each symbolic link, a percent sign ( ‘%’ ) after each whiteout, an equal
   sign ( ‘=’ ) after each socket, and a vertical bar ( ‘|’ ) after each that 
   is a FIFO. */
int flag_F = 0; 

/* Output is not sorted. */
int flag_f = 0; 

/* Modiﬁes the −s and −l options, causing the sizes to be reported in bytes
   displayed in a human readable format. Overrides −k. */
int flag_h = 0; 

/* For each ﬁle, print the ﬁle’s ﬁle serial number (inode number). */
int flag_i = 0; 

/* Modiﬁes the −s option, causing the sizes to be reported in kilobytes. The 
   rightmost of the −k and -h ﬂags overrides the previous ﬂag. See also −h. */
int flag_k = 0; 

/* (The lowercase letter “ell”). List in long format. (See below.) A total sum
   for all the ﬁle sizes is output on a line before the long listing. */
int flag_l = 0; 

/* The same as −l, except that the owner and group IDs are displayed
   numerically rather than converting to a owner or group name. */
int flag_n = 0; 

/* Force printing of non-printable characters in ﬁle names as the character
   ‘?’; this is the default when output is to a terminal. */
int flag_q = 0; 

/* Recursively list subdirectories encountered. */
int flag_R = 0; 

/* Reverse the order of the sort to get reverse lexicographical order or the
   smallest or oldest entries ﬁrst. */
int flag_r = 0; 

/* Sort by size, largest ﬁle ﬁrst. */
int flag_S = 0; 

/* Display the number of ﬁle system blocks actually used by each ﬁle, in units
   of 512 bytes or BLOCKSIZE (see ENVIRONMENT) where partial units are rounded
   up to the next integer value. If the output is to a terminal, a total sum
   for all the ﬁle sizes is output on a line before the listing. */
int flag_s = 0; 

/* Sort by time modiﬁed (most recently modiﬁed ﬁrst) before sorting the
   operands by lexicographical order. */
int flag_t = 0; 

/* Use time of last access, instead of last modiﬁcation of the ﬁle for sorting
   ( −t ) or printing ( −l ). */
int flag_u = 0; 

/* Force raw printing of non-printable characters. This is the default when
   output is not to a terminal. */
int flag_w = 0; 

/* (The numeric digit “one”). Force output to be one entry per line. This is
   the default when output is not to a terminal. */
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

void
traverse(char *argv, int fts_options)
{
  FTS *ftsp;
  FTSENT *p, *chp;

  if ((ftsp = fts_open(argv, fts_options, NULL)) == NULL) {
    // fprintf(stderr, "");
    exit(EXIT_FAILURE);
  }
  chp = fts_children(ftsp, 0);
  printf("chp: %s\n", chp);
  if (chp == NULL) {
    printf("no files");
    exit(EXIT_SUCCESS);
  }
  
  while ((p = fts_read(ftsp)) != NULL) {
    switch (p->fts_info) {
    case FTS_D:
      if (p->fts_level != 1)
        break;
      printf("%s\n", p->fts_name);
      break;
    case FTS_F:
      if (!flag_a && p->fts_name[0] == '.')
        break;
      printf("%s\n", p->fts_name);
      break;
    case FTS_DOT:
      if (p->fts_level != 1)
        break;
      printf("%s\n", p->fts_name);
      break;
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
  int fts_options;
  // fts_options = FTS_COMFOLLOW | FTS_LOGICAL | FTS_NOCHDIR;

  /* This works on BSD, but not in the stdlib on Linux. */
  setprogname(argv[0]);

   // "−AacdFfhiklnqRrSstuw1"
  while ((ch = getopt(argc, argv, "−a")) != -1) {
    switch (ch) {   /* Indent the switch. */
    // case 'A':    Don't indent the case. 
    //   flag_A = 1;
    //   /* FALLTHROUGH */
    case 'a':
      flag_a = 1;
      fts_options |= FTS_SEEDOT;
      break;
    // case '1':
    //   errno = 0;
    //   num = strtol(optarg, &ep, 10);
    //   if (num <= 0 || *ep != '\0' || (errno == ERANGE &&
    //       (num == LONG_MAX || num == LONG_MIN)) )
    //     errx(1, "illegal number -- %s", optarg);
    //   break;
    // case 'd':
  
    default:
    case '?':
      usage();
    }
  }
  argc -= optind;
  argv += optind;

  // printf("%s\n", *argv);
  // printf("flag_a = %d\n", flag_a);

  if (*argv == NULL)
    *argv = "./";

  traverse(argv, fts_options);

  // /* check the target directory whether is given or not */
  // if (*argv == NULL)
  //   dp = opendir("./");
  // else
  //   dp = opendir(*argv);

  // if (dp != NULL) {
  //     while ((dirp = readdir(dp)) != NULL ) {
  //       puts(dirp->d_name);
  //     }
  //     (void) closedir (dp);
  //   }
  // else
  //   perror ("Couldn't open the directory");
  return 0;
}