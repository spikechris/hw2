
#include "lexer.h"

extern FILE * fp;

// Requires: fname != NULL
// Requires: fname is the name of a readable file
// Initialize the lexer and start it reading from the given file name
extern void lexer_open(const char *fname)
{
    if (fname == NULL)
        return;
    fp = fopen(fname, "r");
    // NOT SURE WHAT RETURN SHOULD BE YET
	if (fp == NULL)
		return;
}

// Close the file the lexer is working on
// and make this lexer be done
extern void lexer_close()
{
    fclose(fp);
}

// Is the lexer's token stream finished
// (either at EOF or not open)?
extern bool lexer_done()
{
    // if the FILE pointer isn't open
 	if (fp == NULL)
		return false;
    else
        return (feof(fp));
    
}

// Requires: !lexer_done()
// Return the next token in the input file,
// advancing in the input
extern token lexer_next()
{
    if (!lexer_done())
    {
        return;
    }
}

// Requires: !lexer_done()
// Return the name of the current file
extern const char *lexer_filename()
{
     if (!lexer_done())
    {
        return;
    }   
}

// Requires: !lexer_done()
// Return the line number of the next token
extern unsigned int lexer_line()
{
    if (!lexer_done())
    {
        return;
    }
}

// Requires: !lexer_done()
// Return the column number of the next token
extern unsigned int lexer_column()
{
    if (!lexer_done())
    {
        return;
    }    
}
