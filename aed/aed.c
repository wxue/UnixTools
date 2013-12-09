/* $NetBSD: aed.c,v 0.01 2013/12/06 16:19:25 Weiyu Exp $ */
 
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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

/*
 * Flags
 */
int flag_d = 0;
int flag_e = 0;

 /*
 * Functions
 */
void
usage()
{
  fprintf(stderr, "Usage: aed [−deh] [−p passphrase] [−s salt]\n");
  exit(EXIT_FAILURE);
  /* NOTREACHED */
}

/*
 * Main
 */
 int 
 main(int argc, char *argv[])
 {
  int ch;
  char *passphrase, *salt;
  passphrase = NULL;
  salt = NULL;

  while ((ch = getopt(argc, argv, "dehp:s:")) != -1) {
    switch (ch) {
    case 'd':
      flag_d = 1;
      break;

    case 'e':
      flag_e = 1;
      break;

    case 'p':
      passphrase = optarg;
      // if(passphrasecheck(passphrase))
      //   exit(EXIT_FAILURE);
      break;

    case 's':
      salt = optarg;
      // if (saltcheck(salt))
      //   exit(EXIT_FAILURE);
      break;

    default:
    case '?':
    case 'h':
      usage();
    }
  }
  argc -= optind;
  argv += optind;
/******************TESTING********/
  if (flag_d)
    printf("flag_d: ON\n");
  else
    printf("flag_d: OFF\n");
  if (flag_e)
    printf("flag_e: ON\n");
  else
    printf("flag_e: OFF\n");
  printf("passphrase: %s\n", passphrase);
  printf("salt: %s\n", salt);
/************END OF TESTING*******/

  return 0;
 }