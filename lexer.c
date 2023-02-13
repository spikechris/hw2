// Authors: Vincent Lazo and Christian Manuel
#include "lexer.h"
#include "lexer_output.h"
#include "utilities.h"
// #include "token.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern FILE * fp;
// global variable for current token
token *curr;

// Requires: fname != NULL
// Requires: fname is the name of a readable file
// Initialize the lexer and start it reading from the given file name
extern void lexer_open(const char *fname)
{
	int length;

	// calls function from utilities.h/.c; in short, calls exit() function
    if (fname == NULL)
        bail_with_error(fname);

    fp = fopen(fname, "r");
    
	// catches same error as before
	if (fp == NULL)
		bail_with_error(fname);

	length = strlen(fname);

	curr = malloc(sizeof(token));

	curr->filename = malloc(sizeof(char) * (length + 1));

	curr->text = malloc(sizeof(char) * 100);

	curr->typ = malloc(sizeof(token_type));

	strcpy(curr->filename, fname);
}

// Close the file the lexer is working on
// and make this lexer be done
extern void lexer_close()
{
	free(curr->filename);
	free(curr);
	
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
		token_ptr = calloc(1, sizeof(token));

        // I'M CONFUSED IF THIS IS WHAT I'M SUPPOSED TO BE DOING, BUT I THINK WE HAVE TO POPULATE THE TOKEN FIELDS?
        token t = &token_ptr;

		// read in the next keyword/ argument from the file by setting it equal to curr->text

		// set curr->typ = curr->typ[curr->text]

        // WAIT MAYBE NOT CUZ LEXER_OUTPUT FUNCTION ALREADY DOES THAT IN LEXER_OUTPUT.C 
		// -> it doesnt, it calls this function which we have to implement ourselves

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
		return curr->filename;
    }

// return NULL if lexer is done (can change later if deemed appropriate)
	else
		return NULL;
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
