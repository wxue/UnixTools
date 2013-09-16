/* $NetBSD: tcpm.c,v 0.01 2013/09/15 22:02:03 weiyu Exp $ */
 
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
#include <stdlib.h>
#include <string.h>

#include <sys/mman.h>

/*
 * Functions
 */
void
checkArgc(int argc){
  if (argc != 3){
    printf("Usage: tcpm source_file target_file\n");
    printf("       tcpm source_file ... target_directory\n");
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

int
getSize(int fd){
  struct stat stat_src;
  if (fstat(fd, &stat_src) == -1){
    fprintf(stderr, "Unable to get file source status: %s\n", 
            strerror(errno));
    exit(EXIT_FAILURE);
  }
  return stat_src.st_size;
}

/*
 * Main
 */
int 
main(int argc, char *argv[])
{
  int fd_src, fd_trgt, file_size;
  char *addr_src; char *addr_trgt;
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

/* Get file size. */
  file_size = getSize(fd_src);

/* Make the target file a hole file, same size with the source file.*/
  char buf[file_size];
  if (write(fd_trgt, buf, file_size) != file_size){
    perror("Error writing hole-target_file");
    return 1;
  }

/* mmap both source file and target file. */
  addr_src = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd_src, 0);
  addr_trgt = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED,
                   fd_trgt, 0);

/* length + 1 for the string's end-char '\0' */
  memcpy(addr_trgt, addr_src, file_size+1);

/* unmap the files */
  munmap(addr_src, file_size);
  munmap(addr_trgt, file_size);
  return 0;
}