/* $NetBSD: net.c,v 0.01 2013/11/03 18:40:40 Weiyu Exp $ */
 
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
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h> 
#include <arpa/inet.h>

void
startsws(char *i_address, int p_port)
{    
  int bufsize = 1024;
  char *buffer = malloc(bufsize);    

  int socket_fd, newsocket_fd;
  socklen_t addrlen;
  struct sockaddr_in address;
 
  if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) > 0) {
    printf("Socket created: %d\n", socket_fd);
  }
  
  /* SET socket address/host machine/port number */ 
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr(i_address);
  address.sin_port = htons(p_port);

  if (bind(socket_fd, (struct sockaddr *) &address, 
          sizeof(address)) == 0) {
    printf("Binding Socket: %d | Port: %d | Address: %s \n\n", socket_fd, p_port, i_address);
  }
   
   
  while (1) {    
    /* 
     * Listen on the socket for connections,
     * and set the size of backlog queue to 5. 
     */
    if (listen(socket_fd, 5) < 0) {    
      perror("server: listen");    
      exit(1);
    }
   
    /* 
     * Block process until a client connects to the server. 
     */   
    if ((newsocket_fd=accept(socket_fd, (struct sockaddr *) &address,
                          &addrlen)) < 0) {    
      perror("server: accept");    
      exit(1);
    }

    /*
     * Client Connection Information
     */
    if (newsocket_fd > 0) {
      printf("-----------------------");
      printf("Client~ INFO");
      printf("-----------------------\n");
    }
    recv(newsocket_fd, buffer, bufsize, 0);
    printf("%s\n", buffer);
    write(newsocket_fd, buffer, 12);
    close(newsocket_fd);
  }
  free(buffer);
  close(socket_fd);
}