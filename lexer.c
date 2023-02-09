// Authors: Vincent Lazo and Christian Manuel
#include "lexer.h"
#include "lexer_output.c"

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
// Return the next token in the input file, advancing in the input
extern token lexer_next()
{
    if (!lexer_done())
    {
        // I'M CONFUSED IF THIS IS WHAT I'M SUPPOSED TO BE DOING, BUT I THINK WE HAVE TO POPULATE THE TOKEN FIELDS?
        token t = calloc(1, sizeof(token));
        // WAIT MAYBE NOT CUZ LEXER_OUTPUT FUNCTION ALREADY DOES THAT IN LEXER_OUTPUT.C

        // NOT SURE WHAT RETURN SHOULD BE YET
        return t;
    }
}

// Requires: !lexer_done()
// Return the name of the current file
extern const char *lexer_filename()
{
     if (!lexer_done())
    {
        // NOT SURE WHAT RETURN SHOULD BE YET
        return;
    }   
}

// Requires: !lexer_done()
// Return the line number of the next token
extern unsigned int lexer_line()
{
    if (!lexer_done())
    {
        // NOT SURE WHAT RETURN SHOULD BE YET
        return;
    }
}

// Requires: !lexer_done()
// Return the column number of the next token
extern unsigned int lexer_column()
{
    if (!lexer_done())
    {
        // NOT SURE WHAT RETURN SHOULD BE YET
        return;
    }    
}

// HERE WE'RE SUPPOSED TO CALL LEXER OUTPUT
int main(int argc, char **argv)
{
    char * filename = argv[1];
    lexer_open(filename);

    // maybe not this, cuz it's static, just leaving it here tho
    // lexer_print_output_header();
    lexer_output();
    

    lexer_close();
    return 0;
}
