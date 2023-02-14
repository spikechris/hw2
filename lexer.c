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

	curr->line = 0;
	curr->column = 0;

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

void look_for_symbol(char buffer[], int len, int i)
{
	// only do fseek(fp, [index], SEEK_SET); whenever we hit the next possible symbol, not for
	// whitespace or newline
	while (1)
	{
		fscanf(fp, "%c", &buffer[i]);
		if (buffer[i] == ' ')
		{
			len = strlen(buffer);
			
			for (int k = 0; k < len-2; k++)
			{
				curr->text[k] = buffer[k];
			}

			break;
		}
		
		else if (buffer[i] == '\n')
		{
			len = strlen(buffer);
			
			for (int k = 0; k < len-2; k++)
			{
				curr->text[k] = buffer[k];
			}

			curr->line++;
			curr->column = 0;
			break;
		}

		curr->column++;
		i++;
	}
}

// Requires: !lexer_done()
// Return the next token in the input file, advancing in the input
extern token lexer_next()
{
	char buffer[1000];
	token ret;
	int len = 0, i = 0;
	
	// check for comment, if so skip to next line

	// go until next token can be identified
	look_for_symbol(buffer, len, i);

	// at this point, we have our next string without whitespace in, so we can use it to 
	// identify what token we have

	if (strcmp(curr->text[0], "#") == 0)
	{
		fgets(buffer, 1000, fp);
		curr->line++;
	}

	ret.typ = curr->typ;
	ret.line = curr->line;
	ret.column = curr->column;
	return ret;
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
        return curr->line;
    }
}

// Requires: !lexer_done()
// Return the column number of the next token
extern unsigned int lexer_column()
{
    if (!lexer_done())
    {
        // NOT SURE WHAT RETURN SHOULD BE YET
        return curr->column;
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
