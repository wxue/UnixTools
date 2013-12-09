/* $NetBSD: aed.c,v 1.02 2013/12/09 05:31:01 Weiyu Exp $ */
 
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
#include <string.h>
#include <errno.h>

#include <readpassphrase.h>
#include <openssl/evp.h>

#define MAXPASSLEN 64

/*
 * Flags & Global Variables
 */
int flag_d = 0;
int flag_e = 0;

char *passphrase = NULL;

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

int
passphrasecheck()
{
  char *passbuf;
  char passbuf_re[MAXPASSLEN];
  if ((passbuf = (char*)malloc(MAXPASSLEN*sizeof(char))) == NULL)
    fprintf(stderr, "Unable to allocate memory: %s\n",
            strerror(errno));
  /* 
   * Prompt the user for appropriate passphrase(if the −p ﬂag was not speciﬁed)
   */
  while (passphrase == NULL) {
    if (readpassphrase("Please enter your passphrase: ", passbuf, 
        sizeof(passbuf), RPP_REQUIRE_TTY) == NULL)
      fprintf(stderr, "Error: Unable to read passphrase");
    if (strlen(passbuf) > MAXPASSLEN) {
      fprintf(stderr, "Passphrase should be no more than %d characters\n", 
              MAXPASSLEN);
      memset(passbuf, 0, strlen(passbuf));
      return 1; /* Need to run this function again */
    }
    if (readpassphrase("Please re-enter your passphrase: ", passbuf_re, 
        sizeof(passbuf_re), RPP_REQUIRE_TTY) == NULL)
      fprintf(stderr, "Error: Unable to read passphrase");
    /* Passphrase Match */
    if (strcmp(passbuf, passbuf_re) == 0)
      passphrase = passbuf;
  }

  if (strlen(passphrase) > MAXPASSLEN) {
    fprintf(stderr, "Passphrase should be no more than %d characters\n", 
              MAXPASSLEN);
    passphrase = NULL;
    return 1;
  }
  
  return 0;
}
/*
 * Main
 */
 int 
 main(int argc, char *argv[])
 {
  int ch;
  char *salt;
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

  /* Validation Check */

  while (passphrasecheck());


/******************TESTING********/
  if (flag_d)
    printf("flag_d: ON\n");
  else
    printf("flag_d: OFF\n");
  if (flag_e)
    printf("flag_e: ON\n");
  else
    printf("flag_e: OFF\n");
  (void)printf("passphrase: %s\n", passphrase);
  printf("salt: %s\n", salt);
/************END OF TESTING*******/

  return 0;
 }