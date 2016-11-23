# Makefile for socket demo collection
#(C) 1991 Blair P. Houghton, All Rights Reserved, copying and
#distribution permitted with copyright intact.

# gcc screws up gethostbyname() with my library; try it on yours
#CC = gcc -ansi -pedantic -Wall

# debugging is more fun if the symbols and trace-hooks are intact...
CFLAGS = -g

# shar'ring is rather specific to the sort of shar(1local) one possesses...
SHAR = /usr/local/bin/xshar -vsMDc
#SHAR = shar

all: disock isock sock


# unix-local-connection example
sock: sockl sockt

sockl: sockl.c sockl.h
	$(CC) $(CFLAGS) -o sockl sockl.c

sockt: sockt.c sockl.h
	$(CC) $(CFLAGS) -o sockt sockt.c

sockl.h:


# internet-connection example
isock: isockl isockt

isockl: isockl.c
	$(CC) $(CFLAGS) -o isockl isockl.c

isockt: isockt.c
	$(CC) $(CFLAGS) -o isockt isockt.c


# internet-connection example using disconnected transmittal
disock: disockl disockt

disockl: disockl.c
	$(CC) $(CFLAGS) -o disockl disockl.c

disockt: disockt.c
	$(CC) $(CFLAGS) -o disockt disockt.c


man: sockl.1
	nroff -man sockl.nroff-man > sockl.1
sockl.1:


shar:
	$(SHAR) README sockl.nroff-man Makefile		\
sockl.c sockt.c isockl.c isockt.c disockl.c disockt.c	\
sockl.h > sockl.shar


clean:
	rm -f sockl sockt isockl isockt disockl disockt sockl.1
