/* *	This wee program is the first phase of inserting *	a large table into the RESOURCE fork of the *	assembler.  The table is (here) loaded into  *	two files,  *		instrs.d - optable + string offsets (in string file) *		instrs.s - strings file. *	 *	Written *		Rob Burrowes	Apr 19 1988 *	ReWritten *		Paul Reddy		Apr 20 1988 */#include <strings.h>#include <fcntl.h>#include <as.h>#include <stdio.h>#include <string.h>#define DATARESOURCE 		"instrs.d"#define STRINGSRESOURCE		"instrs.s"#define	OP(name, eopcode, popcode, nargs, arg1, arg2, arg3, arg4, arg5, arg6) \	{ \		name, popcode, nargs, arg1, arg2, arg3, arg4, arg5, arg6, \		(nargs == 0 ? INST0:INSTn), eopcode \	}#define	PSEUDO(name, type, tag) \	{ \		name, type, 0,   0, 0, 0, 0, 0, 0, \		tag, CORE \	}struct Instab instructtab[] = {#include "instrs.as"PSEUDO("\0\0\0\0\0\0\0\0\0\0", 0, 0),PSEUDO(0, 0, 0)};struct Instab *instab;main(){struct Instab *op;int offset = 1;int length;int fd;int numelts = 1 ;int xxx = 0;	instab = instructtab;	fd = open( STRINGSRESOURCE, O_WRONLY|O_CREAT|O_TRUNC);	for(op = instab; op->I_name; op++, numelts++)	{	xxx++;		/*		 *	stupid instruction table has nulls at the start of		 *	the names of each operand. God I hope they're all the		 *	same length.  		 */			if( write(fd,(char *) op->I_name, length = (strlen( &op->I_name[8] ) + 9))				!= length )		{			fprintf(stderr, "cant write!!!" );			return( -1234 );		}		op->I_name = (char *)offset;		offset += length;	}fprintf(stderr," installed %d  elements into strings table\n", xxx);		close(fd);	fd = open( DATARESOURCE, O_WRONLY | O_CREAT | O_TRUNC); 	write(fd,(char *)instab,sizeof( instructtab )); fprintf(stderr," sizeof( instructtab ) =  %d\n", sizeof( instructtab ));		close(fd);} 