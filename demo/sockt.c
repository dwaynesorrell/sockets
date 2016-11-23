/* sockt.c -- open socket and talk into it */
/* (C) 1991 Blair P. Houghton, All Rights Reserved, copying and */
/* distribution permitted with copyright intact.		*/

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "sockl.h"

#ifdef __STDC__
extern void	perror( char * );
extern int	bind( int, struct sockaddr *, int );
extern int	socket( int, int, int );
extern int	write( int, char *, unsigned );
extern char *	strcpy( char *, char *b );
extern int	strlen( char * );
extern void	exit( int );
extern int	connect( int, struct sockaddr *, int );
#endif

char *line[] = {
    "Mary had a little lamb;\n",
    "Her fleece was white as snow;\n",
    "And everywhere that Mary went,\n",
    "She told everyone that Edison invented the telephone before Bell did.\n"
};
int n_line = 4;

#ifdef __STDC__
void main( int argc, char *argv[] )
#else
main(argc,argv)
int argc; char *argv[];
#endif
{

    int plug;				/* socket to "plug" into the socket */
    struct sockaddr_un socketname;	/* mode and path data for the socket */
    extern int n_line;
    extern char *line[];
    int i;

    /* make a local-unix, stream-i/o, protocol-whatever plug */
    if ( (plug = socket( AF_UNIX, SOCK_STREAM, 0 )) < 0 )
	perror(argv[0]);

    /* plug it into the listening socket */
    socketname.sun_family = AF_UNIX;
    strcpy( socketname.sun_path, SOCKET_PATH_NAME );
    if ( connect( plug, (struct sockaddr *) &socketname,
		  sizeof socketname ) < 0 ) {
	perror(argv[0]);
	exit(__LINE__);
    }

    /* say something into it; something historic */
    for ( i = 0; i < n_line; i++ )
	write( plug, line[i], strlen(line[i]) );
}
