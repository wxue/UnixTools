/* $NetBSD: tcp.c,v 0.02 2013/09/13 17:52:03 weiyu Exp $ */
 
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
 * 3. All advertising materials mentioning features or use of this 
 *    software must display the following acknowledgement:
 *    This product includes software developed by the Wikimedia Foundation.
 * 4. Neither the name of the Wikimedia Foundation nor the names of 
 *    its contributors may be used to endorse or promote products derived 
 *    from this software without specific prior written permission.
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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

/* Provide a declaration for 'exit' */  
#include <stdlib.h>

/* Provide a declaration for 'strerror' */
#include <string.h>

#define BUFFSIZE 20480

/*
 * Functions
 */
void
checkArgc(int argc){
  if (argc != 3){
    printf("Usage: tcp source_file target_file\n");
    printf("       tcp source_file ... target_directory\n");
    exit(EXIT_FAILURE);
  }
}

int
createFile(char *src_file, char *trgt_file) {
  int fd;

/* 
 * Set O_TRUNC: If the file exists and is a regular file, and the file is  
 * successfully opened O_RDWR or O_WRONLY, its length is truncated to 0 and  
 * the mode and owner are unchanged.
 */
  if ((fd = open(trgt_file, O_RDWR | O_CREAT | O_TRUNC, 
                 S_IRUSR | S_IWUSR)) == -1) {
/* 
 * If trget_file is a directory, give it the same name as  
 * its source file.
 */
 /* Errno 21: Can't open a directory. */
    if (errno == 21){
      (void)close(fd);
      strcpy(trgt_file+strlen(trgt_file),"/");
      strcpy(trgt_file+strlen(trgt_file),src_file);
      if ((fd = open(trgt_file, O_RDWR | O_CREAT, 
                     S_IRUSR | S_IWUSR)) == -1){
        fprintf(stderr, "Unable to create 'trgt_file': %s\n", 
                strerror(errno));
        exit(EXIT_FAILURE);
      }
    }
    else{
        fprintf(stderr, "Unable to create 'trgt_file': %s\n", 
                strerror(errno));
        exit(EXIT_FAILURE);   
    }
  }
  return fd;
}

int
openFile(char *src_file){
  int fd;
  if ((fd = open(src_file, O_RDONLY | O_APPEND)) == -1) {
    fprintf(stderr, "Unable to open '%s': %s\n", src_file, 
            strerror(errno));
    exit(EXIT_FAILURE);
  }
  return fd;
}

/*
 * Main
 */
int 
main(int argc, char *argv[])
{
  int fd_src, fd_trgt, n;
  char buf[BUFFSIZE];
  /* This works on BSD, but not in the stdlib on Linux. */
  // setprogname(argv[0]);

/* Check the command and show the correct usage if meets any error */
  checkArgc(argc);

/*
 * Create a target file in the directory as declared in the command or
 * in the current directory if path is given. 
 * Then get its file descriptor.
 */
  fd_trgt = createFile(argv[1], argv[2]);

/* Open the source file and get its file descriptor*/
  fd_src = openFile(argv[1]);

/*
 * Read from source file and write its content to
 * target file. Using buff to adjust the byte size of 
 * each read and write.
 */
  while ((n = read(fd_src, buf, BUFFSIZE)) > 0 )
    if ( write(fd_trgt, buf, n) != n ) {
      fprintf(stderr, "write error\n");
      exit(1);
    }
  return 0;
}