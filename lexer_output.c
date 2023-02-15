/* $Id: lexer_output.c,v 1.2 2023/01/31 06:45:02 leavens Exp $ */
#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

// Requires: lexer is not done
// Print a message about the file name of the lexer's input
// And print a heading for the lexer's output.
// Both are printed on stdout.
static void lexer_print_output_header()
{
    printf("Tokens from file %s\n", lexer_filename());
    printf("Number Name       Line Column Text/Value\n");
}

// Print information about the token t to stdout
// followed by a newline
static void lexer_print_token(token t)
{
    printf("%-6d %-10s %-4d %-6d", t.typ, ttyp2str(t.typ), t.line, t.column);
    
	if (t.typ == numbersym)
	{
		printf(" %d\n", t.value);
    } 
	else
	{
		if (t.text != NULL)
		{
			//if (t.typ == eofsym)
			//printf("hioutput\n");
	    	printf(" \"%s\"\n", t.text);
		}
		else
		{
	    	printf("\n");
		}
    }
}

void lexer_output()
{
	int i = 0;
    lexer_print_output_header();
    while (!lexer_done())
	{
		//printf("Iteration: %d\n", i);
		//i++;
		fflush(stdout);		
		token t = lexer_next();
		lexer_print_token(t);
    }
}
