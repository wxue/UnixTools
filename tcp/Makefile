#     $NetBSD: Makefile,v 0.01 2013/09/14 22:45:01 weiyu Exp $

CC = gcc

#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -Wall

TARGET = tcp

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

# Delete the existing binary files of above code before creating new ones next time
clean:
	$(RM) $(TARGET)
