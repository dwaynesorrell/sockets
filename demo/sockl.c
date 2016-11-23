/* sockl.c -- set up a UNIX STREAM socket and listen on it */
/* (C) 1991 Blair P. Houghton, All Rights Reserved, copying and */
/* distribution permitted with copyright intact.		*/

/* a stream (a socket opened using SOCK_STREAM) requires the use of
listen() and accept() in a receiver, and connect() in a sender */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
/* include sockl.h only after <sys/socket.h> */
#include "sockl.h" 

#ifdef __STDC__
extern void	exit( int );
extern void	perror( char * );
extern int	printf( char *, ... );
extern int	bind( int, struct sockaddr *, int );
extern int	socket( int, int, int );
extern int	read( int, char *, unsigned );
extern char *	strcpy( char *, char *b );
extern int	fcntl( int, int, int );
extern int	accept( int, struct sockaddr *, int * );
extern int	listen( int, int );
extern int	unlink( char * );

void main( int argc, char *argv[] )
#else
main(argc,argv)
int argc; char *argv[];
#endif
{
    int sock;				/* fd for the actual socket */
    int ear;				/* fd for the working socket */
    struct sockaddr_un sockaddr;	/* sytem's location of the socket */
    char buf[BUFSIZ];
    int read_ret;
    int f_ret;

    /*
     *  open a unix (local) socket, using stream (file-style i/o)
     *  mode, with protocol irrelevant ( == 0 ) (the protocol is
     *  generally determined by the connection style: tcp for stream,
     *  udp for datagrams, but this is not immutable nor all the
     *  protocols for these styles).
     */
    if ( (sock = socket( AF_UNIX, SOCK_STREAM, 0 )) < 0 ) {
	char s[BUFSIZ];
	sprintf( s, "%s: can't assign fd for socket", argv[0] );
	perror(s);
	exit(__LINE__);
    }

    /* place a filename in the filesystem for other processes to find */
    sockaddr.sun_family = AF_UNIX;
    (void) strcpy( sockaddr.sun_path, SOCKET_PATH_NAME );

    if ( bind( sock, (struct sockaddr *) &sockaddr, sizeof sockaddr ) < 0 ) {
	char s[BUFSIZ];
	sprintf( s, "%s: can't bind socket (%d) to pathname (%s)",
		argv[0], sock, sockaddr.sun_path );
	perror(s);
	exit(__LINE__);
    }

    printf("opened socket as fd (%d) at path (%s) for stream i/o\n",
	    sock, sockaddr.sun_path);

    /* put an ear to the socket, listening for a knock-knock-knocking */
    listen( sock, 1 );				/* 1: only one queue slot */
    /* ear will be a temporary (non-reusable) socket different from sock */
    if ( (ear = accept( sock, (struct sockaddr *)NULL, (int *)NULL )) < 0 ) {
	perror(argv[0]);
	exit(__LINE__);
    }

    /* read lines and print until the stream closes */
    while ( (read_ret = read( ear, buf, sizeof buf )) > 0 )
	printf( "\n%s: read from socket as follows:\n(%s)", argv[0], buf ); 

    if ( read_ret < 0 ) {
	char s[BUFSIZ];
	sprintf( s, "%s: error reading socket", argv[0] );
	perror(s);
	exit(__LINE__);
    }

   /*
    *  lots of things will close automatically: sock, ear;
    *  but, this one must be done cleanly:
    */

    /* rm filename */
    unlink( SOCKET_PATH_NAME );

    /* loop ended normally:  read() returned NULL */
    exit(0);
}

