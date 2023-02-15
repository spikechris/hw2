// Authors: Vincent Lazo and Christian Manuel
#include "lexer.h"
#include "lexer_output.h"
#include "utilities.h"
// #include "token.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define deb 0

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
	strcpy(curr->filename, fname);

	curr->text = malloc(sizeof(char) * 100); // I REVERSED THESE COMMENTS
    //curr->text = NULL;


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
    else if (feof(fp))
        return true;
    else
        return false;
        
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
		if (deb)
           	printf("BUFFER AT TOP is: --%s-- len = %d\n", buffer, len);
        //     printf("I is %d\n", i);

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
            // strcpy(curr->text, buffer);
            // curr->text[len] = '\0';
			if (deb)
	            printf("Curr text is: <%s>\n", curr->text);

            // DELETE LATER
            curr->column++;
			break;
		}
		
		else if (buffer[i] == '\n')
		{
            if (i==0)
                printf("i is 0\n");
            buffer[i] = '\0';
            // if (deb)
            // printf("(AT NEWLINE) buffer is: --%s-- \nlen = %d\n", buffer, len);

            curr->column = 1;
            i--;
            // We have stuff to return from token new line
            if (i>0)
			{
                ungetc('\n', fp);
                //printf("I is %d\n",i);
                int k;
                for (k = 0; k < len+1; k++)
                {
                    curr->text[k] = buffer[k];
                }
                break;
            }
            curr->line++;

            continue;
            
            // it's just a new line, we continue
            i--;
            continue;

            // curr->text[k] = '\0';
            //printf("BUFFER text is: <%s>\n", buffer);
            // printf("Curr text is: <%s>\n", curr->text);
			break;
		}

        //ADDED TO GO BACKWARDS FOR , AND ;
 		else if (buffer[i] == ',' || buffer[i] == ';' || buffer[i] == '.')
		{
            // means we need to get the token before that
			if (i > 0)
			{				
                // in example 0, we need to get x
				ungetc(buffer[i], fp);
                //curr->column;

                buffer[i] = '\0';
                len = strlen(buffer);  
                strcpy(curr->text, buffer);

                //printf("IN I>0 i is %d, len = %d, buffer = %s\n", i, len, buffer);

                break;
			}
            buffer[i+1] = '\0';
			len = strlen(buffer);
            //printf("i is %d, len = %d, buffer = %s\n", i, len, buffer);
            strcpy(curr->text, buffer);
			curr->column++;

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

        else if(feof(fp))
        {
            // if (curr->text != NULL)
            //     free(curr->text);
            break;
            //printf("reached end of file\n");
			if (i > 0)
			{				
                // in example 0, we need to get x
				ungetc(';', fp);
                curr->column;
                curr->line--;
                buffer[i] = '\0';
                len = strlen(buffer);  
                strcpy(curr->text, buffer);

                //printf("i is %d, len = %d, buffer = %s\n", i, len, buffer);

                break;
			}
            
			len = strlen(buffer);
            //printf("i is %d, len = %d, buffer = %s\n", i, len, buffer);
            strcpy(curr->text, buffer);
			curr->column++;
            curr->line++;
            break;
        }

        else if(buffer[i] == '#')
        {
            fgets(buffer, 999, fp);
            //printf("cur text is '%s'\n", curr->text);
            fflush(stdout);            
            //strcpy(curr->text, "skip");
            //printf("hi'%s'\n", curr->text);
            fflush(stdout);
            curr->line++;
            //curr->typ = skipsym;
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
    ret->column = curr->column; // SHUOLD BE 1
    ret->line = curr->line;
	int a = 0;
	
	// go until next token can be identified
    look_for_symbol();
    fflush(stdout);
	// at this point, we have our next string without whitespace in, so we can use it to 
	// identify what token we have
	
    if (1)
    {
        if (strcmp(curr->text, ".") == 0) 
        {
            curr->typ = periodsym;
        }

        else if(curr->text[0] == '#')
        {
            curr->typ = skipsym;
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

        else if(feof(fp))
        {
            curr->typ = eofsym;
            // if (curr->text != NULL)
            //     free(curr->text);
            //printf("eof\n");
            //strcpy(curr->text, "");
        }
        
        // here we assume valid string such as "ab" "12" and "3c"
        else
        {
            a = atoi(curr->text);
            if (a != 0) // CHANGED FROM NULL BUT CHANGE LATER
            {
                if (a < SHRT_MAX)
                    curr->value = a;
                curr->typ = numbersym;
				curr->value = a;
                
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
    //if (ret->typ == identsym)
        //printf("RET: %d\n", ret->column);
    //if (ret->line == )
    if (curr->line > ret->line)
    {
        //printf("curr line = %d ret line = %d\n", curr->line, ret->line);
        ret->column = 1;
    }
	ret->line = curr->line;
	//ret->column = curr->column;
    //if (curr->text != NULL)
	    strcpy(ret->text, curr->text);
    ret->value = curr->value;
    if (ret->typ == eofsym)
    {
        ret->text = '\0';
    }
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
