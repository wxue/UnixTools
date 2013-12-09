#     $NetBSD: Makefile,v 0.01 2013/09/23 17:41:21 weiyu Exp $

# compiler
CC = gcc

#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -Wall

# .h files
# INCLUDES = ./
INCLUDES = 

# library paths(for those not under the path /usr/lib)
# LFLAGS = -L/home/newhall/lib  -L../lib
LFLAGS = 

# define any libraries to link into executable:
#   if I want to link in libraries (libx.so or libx.a) I use the -llibname 
#   option, something like (this will link in libmylib.so and libm.so:
# LIBS = -lmylib -lm
LIBS = -lbsd -lcrypto
# LIBS =

# C source files
SRCS = aed.c

#
# Below we are replacing the suffix .c of all words in the macro SRCS
# with the .o suffix
#
OBJS = $(SRCS:.c=.o)

# name of the executable file 
MAIN = aed

#
# Where the magic happens :P
#

.PHONY: depend clean

all: $(MAIN)

$(MAIN): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file) 
# (see the gnu make manual section about automatic variables)
.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

# Delete the existing binary files of above code before creating new ones 
# next time
clean:
	$(RM) *.o *~ $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it
