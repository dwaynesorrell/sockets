/* disockt.c -- open an internet socket and talk dgrams into it */
/* (C) 1991 Blair P. Houghton, All Rights Reserved, copying and */
/* distribution permitted with copyright intact.		*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <math.h>

#ifdef __STDC__
extern void	perror( char * );
extern int	bind( int, struct sockaddr *, int );
extern int	socket( int, int, int );
extern int	write( int, char *, unsigned );
extern char *	strcpy( char *, char *b );
extern int	strlen( char * );
extern void	exit( int );
extern int	connect( int, struct sockaddr *, int );
extern struct hostent *	gethostbyname( char * );
extern int	fprintf( FILE *, char *, ... );
extern int	atoi( char * );
extern void	bcopy( char *, char *b, int );
#endif

char *line[] = {
    "Mary had a little lamb;\n",
    "Its fleece was white as snow;\n",
    "And everywhere that Mary went,\n",
    "She told everyone that Edison invented\nthe telephone before Bell did.\n"
};
int n_line = 4;

/*
 *  argv 0 is program name; argv 1 is remote host; argv 2 is
 *  port number of listener on remote host
 */
#ifdef __STDC__
void main( int argc, char *argv[] )
#else
main(argc,argv)
int argc; char *argv[];
#endif
{

    int plug;				/* socket to "plug" into the socket */
    struct sockaddr_in socketname;	/* mode, addr, and port	*/
					/* data for the socket	*/
    struct hostent *remote_host;	/* internet numbers, names */
    extern int n_line;
    extern char *line[];
    char buf[BUFSIZ];
    int i, sendflags;

    /* make an internet-transmitted, dgram-i/o-style, protocol-whatever plug */
    if ( (plug = socket( AF_INET, SOCK_DGRAM, 0 )) < 0 )
	perror(argv[0]);

    /* plug it into the listening socket */
    socketname.sin_family = AF_INET;
    if ( (remote_host = gethostbyname( argv[1] )) == (struct hostent *)NULL ) {
	fprintf( stderr, "%s: unknown host: %s\n",
		 argv[0], argv[1] );
	exit(__LINE__);
    }
    (void) bcopy( (char *)remote_host->h_addr, (char *) &socketname.sin_addr,
		  remote_host->h_length );
    socketname.sin_port = htons(atoi(argv[2]));

    printf("sending %d dgrams to:\n", n_line);
    printf(
"struct sockaddr_in {\n\
    sin_family		= %d\n\
    sin_addr.s_addr	= %s\n\
    sin_port 		= %d\n\
} socketname;\n"
, socketname.sin_family
, inet_ntoa(socketname.sin_addr)
/* , socketname.sin_addr.s_addr -- gives unsigned long, not struct in_addr */
, ntohs(socketname.sin_port)
    );

    /* say something into it; something historic */
    sendflags = 0;
    for ( i = 0; i < n_line; i++ ) {
	sleep(1);
	if ( sendto( plug,
		     line[i], 1+strlen(line[i]),
		     sendflags,
		     (struct sockaddr *)&socketname, sizeof socketname )
	     < 0 ) {
	    perror(argv[0]);
	    exit(__LINE__);
	}
    }
    
    /* all the socket connections are closed automatically */
    exit(0);
}
