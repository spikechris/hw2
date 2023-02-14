// Authors: Vincent Lazo and Christian Manuel
#include "lexer.h"
#include "lexer_output.h"
#include "utilities.h"
// #include "token.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
// not sure to fix errors
// #include "utilities.c"
// #include "lexer_output.c"
// #include "token.c"

#define deb 1

// this line was giving me errors so Ima try this
// extern FILE * fp;
FILE * fp;
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

	//curr->text = malloc(sizeof(char) * 100);
    curr->text = NULL;


	//curr->typ = malloc(sizeof(token_type));

	curr->line = 1;
	curr->column = 1;

	//strcpy(curr->filename, fname);
}

// Close the file the lexer is working on
// and make this lexer be done
extern void lexer_close()
{
	if (curr != NULL)
    {
        if (curr->filename != NULL)
            free(curr->filename);
        if (curr->text != NULL)
            free(curr->text);
        free(curr);
    }
	//free(curr);
	
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

void look_for_symbol()
{
    int len = 0, i = 0;
    char buffer[1000];
    //curr = malloc(sizeof(char));
	// only do fseek(fp, [index], SEEK_SET); whenever we hit the next possible symbol, not for
	// whitespace or newline
	while (1)
	{
		fscanf(fp, "%c", &buffer[i]);
		if (buffer[i] == ' ')
		{
			buffer[i] = '\0';
			len = strlen(buffer);
            curr->text = malloc((len)* sizeof(char));

			if (deb)
            	printf("buffer is: --%s-- \nlen = %d\n", buffer, len);
			int k;
			for (k = 0; k < len+1; k++)
			{
				curr->text[k] = buffer[k];
			}
            // curr->text[len] = '\0';
			if (deb)
	            printf("Curr text is: <%s>\n", curr->text);

			break;
		}
		
		else if (buffer[i] == '\n')
		{
			buffer[i] = '\0';
			len = strlen(buffer);
			int k;
			for (k = 0; k < len+1; k++)
			{
				curr->text[k] = buffer[k];
			}
            // curr->text[k] = '\0';
            //printf("BUFFER text is: <%s>\n", buffer);
           // printf("Curr text is: <%s>\n", curr->text);

			curr->line++;
			curr->column = 1;
			break;
		}

        //ADDED TO GO BACKWARDS FOR , AND ;
 		else if (buffer[i] == ',' || buffer[i] == ';')
		{
			if (i > 0)
			{				
				ungetc(';', fp);
			}
			len = strlen(buffer) - 1;
			// int k;
			// for (k = 0; k < len-2; k++)
			// {
			// 	curr->text[k] = buffer[k];
			// }
            // strcpy(curr->text, buffer);
            //curr->text[k] = '\0';

           // printf("Curr text IN THE FINAL ELSEIF is: <%s>\n", curr->text);

			//curr->line++;
			//curr->column = 1;
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
	token *ret;
	ret = malloc(sizeof(token));
	int a = 0;
	
	// check for comment, if so skip to next line

	// go until next token can be identified
	//look_for_symbol(buffer, len, i);
    look_for_symbol();

	// at this point, we have our next string without whitespace in, so we can use it to 
	// identify what token we have

	// if (strcmp(curr->text, "#") == 0)
	// {
	// 	fgets(buffer, 1000, fp);
	// 	curr->line++;
	// }
	
    if (1)
    {
        if (strcmp(curr->text, ".") == 0) 
        {
            curr->typ = periodsym;
        }
        
        else if (strcmp(curr->text, "const") == 0) 
        {
            curr->typ = constsym;
        }
        
        else if (strcmp(curr->text, ";") == 0) 
        {
            curr->typ = semisym;
        }
        
        else if (strcmp(curr->text, ",") == 0) 
        {
            curr->typ = commasym;
        }
        
        else if (strcmp(curr->text, "var") == 0) 
        {
            curr->typ = varsym;
        }
        
        else if (strcmp(curr->text, "procedure") == 0) 
        {
            curr->typ = procsym;
        }
        
        else if (strcmp(curr->text, ":=") == 0) 
        {
            curr->typ = becomessym;
        }
        
        else if (strcmp(curr->text, "call") == 0) 
        {
            curr->typ = callsym;
        }
        
        else if (strcmp(curr->text, "begin") == 0) 
        {
            curr->typ = beginsym;
        }
        
        else if (strcmp(curr->text, "end") == 0) 
        {
            curr->typ = endsym;
        }
        
        else if (strcmp(curr->text, "if") == 0) 
        {
            curr->typ = ifsym;
        }
        
        else if (strcmp(curr->text, "then") == 0) 
        {
            curr->typ = thensym;
        }
        
        else if (strcmp(curr->text, "else") == 0) 
        {
            curr->typ = elsesym;
        }
        
        else if (strcmp(curr->text, "while") == 0) 
        {
            curr->typ = whilesym;
        }
        
        else if (strcmp(curr->text, "do") == 0) 
        {
            curr->typ = dosym;
        }
        
        else if (strcmp(curr->text, "read") == 0) 
        {
            curr->typ = readsym;
        }
        
        else if (strcmp(curr->text, "write") == 0) 
        {
            curr->typ = writesym;
        }
        
        else if (strcmp(curr->text, "skip") == 0) 
        {
            curr->typ = skipsym;
        }
        
        else if (strcmp(curr->text, "odd") == 0) 
        {
            curr->typ = oddsym;
        }
        
        else if (strcmp(curr->text, "(") == 0) 
        {
            curr->typ = lparensym;
        }
        
        else if (strcmp(curr->text, ")") == 0) 
        {
            curr->typ = rparensym;
        }
        
        // ****
        else if (strcmp(curr->text, ")") == 0) 
        {
            curr->typ = identsym;
        }
        
        // WHAT TO PUT FOR HERE?
        else if (strcmp(curr->text, ")") == 0) 
        {
            curr->typ = numbersym;
        }
        
        else if (strcmp(curr->text, "=") == 0) 
        {
            curr->typ = eqsym;
        }
        
        else if (strcmp(curr->text, "<>") == 0) 
        {
            curr->typ = neqsym;
        }
        
        else if (strcmp(curr->text, "<") == 0) 
        {
            curr->typ = lessym;
        }
        
        else if (strcmp(curr->text, "<=") == 0) 
        {
            curr->typ = leqsym;
        }
        
        else if (strcmp(curr->text, ">") == 0) 
        {
            curr->typ = gtrsym;
        }
        
        else if (strcmp(curr->text, ">=") == 0) 
        {
            curr->typ = geqsym;
        }
        
        else if (strcmp(curr->text, "+") == 0) 
        {
            curr->typ = plussym;
        }
            
        else if (strcmp(curr->text, "-") == 0) 
        {
            curr->typ = minussym;
        }
        
        else if (strcmp(curr->text, "*") == 0) 
        {
            curr->typ = multsym;
        }
        
        else if (strcmp(curr->text, "/") == 0) 
        {
            curr->typ = divsym;
        }
        
        // here we assume valid string such as "ab" "12" and "3c"
        else
        {
            a = atoi(curr->text);
            if (a != 0) // CHANGED FROM NULL BUT CHANGE LATER
            {
                if (a > SHRT_MAX)
                    curr->value = a;
                curr->typ = numbersym;
				
                // ELSE ERROR
            }
            else
			{
                curr->typ = identsym;
			}
        }
    }
    
	int len2 = strlen(curr->text) - 1;

	ret->text = malloc (sizeof(char) * (len2 + 1));
    ret->typ = curr->typ;
	ret->line = curr->line;
	ret->column = curr->column;
	strcpy(ret->text, curr->text);

    //free(curr->text);
	return *ret;
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
    // printf("hi\n"); // we've reached here

    // maybe not this, cuz it's static, just leaving it here tho
    // lexer_print_output_header();
    lexer_output();
    
    
    //printf("after output\n");
    fflush(stdout);
    lexer_close();
    return 0;
}
