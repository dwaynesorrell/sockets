/* isockl.c -- set up an INTERNET STREAM socket and listen on it */
/* (C) 1991 Blair P. Houghton, All Rights Reserved, copying and */
/* distribution permitted with copyright intact.		*/

/* a stream (a socket opened using SOCK_STREAM) requires the use of
listen() and accept() in a receiver, and connect() in a sender */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

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
extern int	getsockname( int, struct sockaddr *, int * );

void main( int argc, char *argv[] )
#else
main( argc, argv )
int argc; char *argv[];
#endif
{
    int sock;				/* fd for the listening socket */
    int ear;				/* fd for the working socket */
    struct sockaddr_in sockaddr;	/* sytem's location of the socket */
    struct sockaddr_in caller;		/* id of foreign calling process */
    int sockaddr_in_length = sizeof(struct sockaddr_in);
    char buf[BUFSIZ];
    int read_ret;
    int fromlen = sizeof(struct sockaddr_in);
    char acknowledgement[BUFSIZ];

    /*
     *  open a net socket, using stream (file-style i/o)
     *  mode, with protocol irrelevant ( == 0 )
     */
    if ( (sock = socket( AF_INET, SOCK_STREAM, 0 )) < 0 ) {
	char s[BUFSIZ];
	sprintf( s, "%s: can't assign fd for socket", argv[0] );
	perror(s);
	exit(__LINE__);
    }

   /*
    *  register the socket
    */
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = INADDR_ANY;	/* not choosy about who calls */
    sockaddr.sin_port = 0;

    if ( bind( sock, (struct sockaddr *) &sockaddr, sizeof sockaddr ) < 0 ) {
	char s[BUFSIZ];
	sprintf( s, "%s: can't bind socket (%d)", argv[0], sock );
	perror(s);
	exit(__LINE__);
    }

   /*
    *  get port number
    */
    if ( getsockname( sock, (struct sockaddr *) &sockaddr,
		      (int *)&sockaddr_in_length )
	 < 0 ) {
	char s[BUFSIZ];
	sprintf( s, "%s: can't get port number of socket (%d)",
		argv[0], sock );
	perror(s);
	exit(__LINE__);
    }

    printf("opened socket as fd (%d) on port (%d) for stream i/o\n",
	    sock, ntohs(sockaddr.sin_port) );

    printf(
"struct sockaddr_in {\n\
    sin_family		= %d\n\
    sin_addr.s_addr	= %d\n\
    sin_port		= %d\n\
} sockaddr;\n"
, sockaddr.sin_family
, sockaddr.sin_addr.s_addr
, ntohs(sockaddr.sin_port)
    );


    /* put an ear to the socket, listening for a knock-knock-knocking */
    listen( sock, 1 );				/* 1: only one queue slot */
    /* ear will be a temporary (non-reusable) socket different from sock */
    if ( (ear = accept( sock, (struct sockaddr *)&caller, &fromlen )) < 0 ) {
	perror(argv[0]);
	exit(__LINE__);
    }

    /* print calling process' identification */
    printf(
"struct sockaddr_in {\n\
    sin_family		= %d\n\
    sin_addr.s_addr	= %s\n\
    sin_port (!!!)	= %d\n\
} caller;\n"
, caller.sin_family
, inet_ntoa(caller.sin_addr)
/* , caller.sin_addr.s_addr -- gives an unsigned long, not a struct in_addr */
, ntohs(caller.sin_port)
    );

    /* optional ack; demonstrates bidirectionality */
    gethostname(buf, sizeof buf);
    sprintf( acknowledgement, "Welcome, from sunny %s (%s.%d)\n",
		buf,
		buf,
		ntohs(sockaddr.sin_port)
	    );
    /* write into the ear; the sock is _only_for_rendezvous_ */
    if ( write ( ear, acknowledgement, sizeof acknowledgement ) < 1 )
	perror(argv[0]);

    /* read lines until the stream closes */
    while ( (read_ret = read( ear, buf, sizeof buf )) > 0 )
	printf( "%s: read from socket as follows:\n(%s)\n", argv[0], buf ); 

    if ( read_ret < 0 ) {
	char s[BUFSIZ];
	sprintf( s, "%s: error reading socket", argv[0] );
	perror(s);
	exit(__LINE__);
    }

    /* loop ended normally:  read() returned NULL */
    exit(0);
}
