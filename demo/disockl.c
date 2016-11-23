/* sockl.c -- set up an INTERNET DGRAM socket and listen on it */
/* (C) 1991 Blair P. Houghton, All Rights Reserved, copying and */
/* distribution permitted with copyright intact.		*/

/* a DGRAM receiver is thoroughly independent of the sender */

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
extern int	recvfrom( int, char *, int, int, struct sockaddr *, int );

void main( int argc, char *argv[] )
#else
main( argc, argv )
int argc; char *argv[];
#endif
{
    int sock;				/* fd for the socket */
    struct sockaddr_in sockaddr;	/* sytem's location of the socket */
    struct sockaddr_in caller;		/* id of foreign calling process */
    int sockaddr_in_length = sizeof(struct sockaddr_in);
    char buf[BUFSIZ];
    int read_ret;
    int fromlen = sizeof(struct sockaddr_in);
    char acknowledgement[BUFSIZ];

    /*
     *  open a net socket, using dgram (packetized, nonconnected)
     *  mode, with protocol irrelevant ( == 0 )
     */
    if ( (sock = socket( AF_INET, SOCK_DGRAM, 0 )) < 0 ) {
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
    sockaddr.sin_port = 0;			/* ??? why 0? */

    if ( bind( sock, (struct sockaddr *) &sockaddr, sizeof sockaddr ) < 0 ) {
	char s[BUFSIZ];
	sprintf( s, "%s: can't bind socket (%d)", argv[0], sock );
	perror(s);
	exit(__LINE__);
    }

   /*
    *  get port number
    */
    if ( getsockname(sock, (struct sockaddr *) &sockaddr, (int *)&sockaddr_in_length)
	 < 0 ) {
	char s[BUFSIZ];
	sprintf( s, "%s: can't get port number of socket (%d)",
		argv[0], sock );
	perror(s);
	exit(__LINE__);
    }

    printf("opened socket as fd (%d) on port (%d) for dgram i/o\n",
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
    fflush(stdout);

    /* read and print lines until the cows come home */
    while ( (read_ret = recvfrom( sock, buf, sizeof buf,
				  0, (struct sockaddr *) &caller,
				  sizeof caller)
	    ) > 0 ) {
	printf( "%s: read (from caller (%s, %d)) socket as follows:\n(%s)\n",
		argv[0],
		inet_ntoa(caller.sin_addr),
		ntohs(caller.sin_port),
		buf ); 
	fflush(stdout);
    }

    if ( read_ret < 0 ) {
	char s[BUFSIZ];
	sprintf( s, "%s: error reading socket", argv[0] );
	perror(s);
	exit(__LINE__);
    }

    /* loop ended normally:  read() returned NULL */
    exit(0);
}
