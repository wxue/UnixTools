/* $NetBSD: aed.c,v 1.04 2013/12/09 19:30:00 Weiyu Exp $ */
 
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
#include <string.h>
#include <ctype.h>
#include <sys/types.h>

// #include <bsd/unistd.h>
#include <bsd/readpassphrase.h>
#include <openssl/evp.h>
#include <sys/file.h>

// #if defined(__APPLE__) && defined(__MACH__)
//   #include <readpassphrase.h>
//   #define COMMON_DIGEST_FOR_OPENSSL
//   #include <CommonCrypto/CommonDigest.h>
//   #define SHA1 CC_SHA1

//   #include <CommonCrypto/CommonHMAC.h>
//   #define HMAC CCHmac

// #else
//   #include <bsd/readpassphrase.h>
//   #include <openssl/evp.h>
// #endif

#define MAXPASSLEN 64
#define INITIAL -1
#define ENCRYPTION 1
#define DECRYPTION 0
#define BUFSIZE 256 /* multiple of the block size */

/*
 * Flags & Global Variables
 */
int task_type = INITIAL;

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
  int ttyfd;
  char *passbuf;
  char passbuf_re[MAXPASSLEN];
  if ((passbuf = (char*)malloc(MAXPASSLEN*sizeof(char))) == NULL) {
    fprintf(stderr, "Unable to allocate memory: %s\n",
            strerror(errno));
    exit(EXIT_FAILURE);
  }
  if ((ttyfd=open("/dev/tty", O_RDWR)) < 0) {
    perror("open tty: ");
    exit(EXIT_FAILURE);
  }
  if (flock(ttyfd, LOCK_EX) != 0) {
    perror("flock: ");
    exit(EXIT_FAILURE);
  }
  /* 
   * Prompt the user for appropriate passphrase(if the −p ﬂag was not specified)
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
    if (task_type == DECRYPTION) {
      passphrase = passbuf;
      break;
    }
    if (readpassphrase("Please re-enter your passphrase: ", passbuf_re, 
        sizeof(passbuf_re), RPP_REQUIRE_TTY) == NULL)
      fprintf(stderr, "Error: Unable to read passphrase");
    /* Passphrase Match */
    if (strcmp(passbuf, passbuf_re) == 0)
      passphrase = passbuf;
  }
  if (flock(ttyfd, LOCK_UN) != 0) {
    perror("flock: ");
    exit(EXIT_FAILURE);
  }
  (void)close(ttyfd);
  if (strlen(passphrase) > MAXPASSLEN) {
    fprintf(stderr, "Passphrase should be no more than %d characters\n", 
              MAXPASSLEN);
    passphrase = NULL;
    return 1;
  }
  
  return 0;
}

int
saltcheck(char *salt)
{
  int i;
  if (strlen(salt) != 8)
    return 1;
  for (i=0; i<strlen(salt); i++) {
    if (!isxdigit(salt[i]))
      return 1;
  }
  return 0;
}

int
asteriskpassphrase()
{
  int i;
  char *asteriskbuf;
  if ((asteriskbuf = (char*)malloc((MAXPASSLEN+1)*sizeof(char))) == NULL) {
    fprintf(stderr, "Unable to allocate memory: %s\n",
            strerror(errno));
    return 1;
  }

  for (i=0; i<strlen(passphrase); i++) {
    asteriskbuf[i] = passphrase[i];
    passphrase[i] = '*';
  }
  asteriskbuf[i] = '\0';
  passphrase = asteriskbuf;
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
  unsigned char key[EVP_MAX_KEY_LENGTH], iv[EVP_MAX_IV_LENGTH];
  EVP_CIPHER_CTX ctx;
  int buflen, buflen_final;
  unsigned char buf_raw[BUFSIZE], buf[BUFSIZE*2];
  int readlen = 0;

  /* prevents leaking the data into the process table */
  // setproctitle("%s", argv[0]);

  while ((ch = getopt(argc, argv, "dehp:s:")) != -1) {
    switch (ch) {
    case 'd':
      task_type = DECRYPTION;
      break;

    case 'e':
      task_type = ENCRYPTION;
      break;

    case 'p':
      passphrase = optarg;
      break;

    case 's':
      salt = optarg;
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
  if (task_type == INITIAL) {
    fprintf(stderr, "Decryption/Encryption is not specified\n");
    usage();
  }
  while (passphrasecheck());
  /* prevents leaking the data into the process table */
  if (asteriskpassphrase()) {
    fprintf(stderr, "Asterisk passphrase error\n");
    exit(EXIT_FAILURE);
  }

  if ((salt!=NULL) && saltcheck(salt)) {
    fprintf(stderr, "The salt needs to be exactly 8 hexadecimal characters\n");
    exit(EXIT_FAILURE);
  }

  /* Encryption and Decryption */
  if(EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), (unsigned char*)salt, (unsigned char*)passphrase, 
                    MAXPASSLEN, 1, key, iv) == 0)
    fprintf(stderr, "EVP_BytesToKey Error\n");

  while ((readlen = read(STDIN_FILENO, buf_raw, BUFSIZE)) != 0) {
    EVP_CIPHER_CTX_init(&ctx);
    EVP_CIPHER_CTX_set_padding(&ctx, task_type);
    if (EVP_CipherInit_ex(&ctx, EVP_aes_256_cbc(), NULL, key, iv, task_type) == 0) {
      fprintf(stderr, "EVP_CipherInit_ex Error\n");
      exit(EXIT_FAILURE);
    }
    if (EVP_CipherUpdate(&ctx, buf, &buflen, buf_raw, readlen) == 0) {
      fprintf(stderr, "EVP_CipherUpdate Error\n");      
      exit(EXIT_FAILURE);
    }
    if (readlen == BUFSIZE) {
      if (write(STDOUT_FILENO, buf, buflen) != buflen) {
        perror("write: ");
        exit(EXIT_FAILURE);
      }
      EVP_CIPHER_CTX_cleanup(&ctx);
    }
  }
  if (EVP_CipherFinal_ex(&ctx, buf+buflen, &buflen_final) == 0) {
    if (task_type == ENCRYPTION)
      fprintf(stderr, "Encryption: EVP_CipherFinal_ex Error\n");
    else
      fprintf(stderr, "Decryption: EVP_CipherFinal_ex Error\n");
    exit(EXIT_FAILURE);
  }
  if (write(STDOUT_FILENO, buf, buflen+buflen_final) != buflen+buflen_final) {
    perror("write: ");
    exit(EXIT_FAILURE);
  }
  EVP_CIPHER_CTX_cleanup(&ctx);

/******************TESTING********/
  // if (task_type == DECRYPTION)
  //   printf("flag_d: ON\n");
  // else
  //   printf("flag_d: OFF\n");
  // if (task_type == ENCRYPTION)
  //   printf("flag_e: ON\n");
  // else
  //   printf("flag_e: OFF\n");
  // printf("passphrase: %s\n", passphrase);
  // printf("salt: %s\n", salt);
  // for (i=0; i<32; i++)
  //   printf("key[%d]: %x\n", i, key[i]);
/************END OF TESTING*******/

  return 0;
 }