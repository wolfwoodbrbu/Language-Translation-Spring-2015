#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "globals.h"
#include "header.h"
Token Scanner(void)
{
    /*declare tokens
    int BEGIN = 1;
    int END = 2;
    int READ = 3;
    int WRITE = 4;
    int ID = 5;
    int INTLITERAL = 6;
    int LPAREN = 7;
    int RPAREN = 8;
    int SEMI = 9;
    int COMMA = 10;
    int ASSIGNOP = 11;
    int PLUSOP = 12;
    int MINUSOP = 13;
    int SCANEOF = 14;
    int ERROR = 15;
    */

    int token_operation = 0;
    Token tokenReturned = BLANKSPACE;
    char inpLine;
    char lineBuffer[BUFFER_LEGNTH];
    char tempBuffer[BUFFER_LEGNTH];
    char tokenBuffer[BUFFER_LEGNTH];
    char errorLine[BUFFER_LEGNTH];
    char buffer[2];
    int lineNum = 0;
    extern FILE *inFile;
    extern FILE *outFile;

    memset(lineBuffer,0,sizeof(lineBuffer));
    memset(tempBuffer,0,sizeof(tempBuffer));
    memset(tokenBuffer,0,sizeof(tokenBuffer));
    memset(errorLine,0,sizeof(errorLine));
    memset(buffer,0,sizeof(buffer));


    do
    {
        inpLine = fgetc(inFile);
        if (inpLine == '\n' || inpLine == EOF)
        {
            token_operation = 0;
        }
        else if (inpLine == ' ' || inpLine == '\t')
        {
            token_operation = BLANKSPACE; //?
        }
        else if (isalpha(inpLine))
        {
            token_operation = ID;
        }
        else if (isdigit(inpLine))
        {
            token_operation = INTLITERAL;
        }
        else if (inpLine == '(')
        {
            token_operation = LPAREN;
        }
        else if (inpLine == ')')
        {
            token_operation = RPAREN;
        }
        else if (inpLine == ';')
        {
            token_operation = SEMICOLON;
        }
        else if (inpLine == ',')
        {
            token_operation = COMMA;
        }
        else if (inpLine == ':')
        {
            token_operation = ASSIGNOP; //will still have to check for =
        }
        else if (inpLine == '+')
        {
            token_operation = PLUSOP;
        }
        else if (inpLine == '-')
        {
            token_operation = MINUSOP;
        }
        else
        {
            token_operation = ERROR;
            printf("Lexical Error - bad input.\n");
        }

        switch(token_operation)
        {
        case 0: //\n or EOF
            fprintf(outFile, "%s\n",lineBuffer);
            lineNum++;
            sprintf(lineBuffer, "%d", lineNum);
            if (inpLine == EOF)
            {
                fprintf(outFile, "Token ID: %d\t token type: %s\tActual token: %s\n", SCANEOF, "SCANEOF","EOF");
                tokenReturned = SCANEOF;
            }
            break;
        case ID: //isAlpha
            sprintf(buffer, "%c", inpLine);
            strcpy(tokenBuffer,buffer);
            do
            {
                inpLine = fgetc(inFile);
                if (isalnum(inpLine))
                {
                    sprintf(buffer, "%c", inpLine);
                    strcat(tokenBuffer,buffer);
                }
            }
            while(isalnum(inpLine));
            ungetc(inpLine, inFile);
            if (strcmpi(tokenBuffer,"BEGIN") == 0)
            {
                fprintf(outFile,"Token ID: %d \t Token type: %s \t\t Actual token: %s\n", BEGIN, "BEGIN", tokenBuffer);
                tokenReturned = BEGIN;
            }
            else if (strcmpi(tokenBuffer,"END") == 0)
            {
                fprintf(outFile,"Token ID: %d \t Token type: %s \t\t Actual token: %s\n", END, "END", tokenBuffer);
                tokenReturned = END;
            }
            else if (strcmpi(tokenBuffer, "READ") == 0)
            {
                fprintf(outFile,"Token ID: %d \t Token type: %s \t\t Actual token: %s\n", READ, "READ", tokenBuffer);
                tokenReturned = READ;
            }
            else if (strcmpi(tokenBuffer,"WRITE")==0)
            {
                fprintf(outFile,"Token ID: %d \t Token type: %s \t\t Actual token: %s\n", WRITE, "WRITE", tokenBuffer);
                tokenReturned = WRITE;
            }
            else
            {
                fprintf(outFile,"Token ID: %d \t Token type: %s \t\t Actual token: %s\n", ID, "ID", tokenBuffer);
                tokenReturned = ID;
            }
            strcat(lineBuffer,tokenBuffer);
            break;
        case INTLITERAL: //isDigit
            sprintf(buffer, "%c", inpLine);
            strcpy(tokenBuffer,buffer);
            do
            {
                inpLine = fgetc(inFile);
                if(isdigit(inpLine))
                {
                    sprintf(buffer, "%c",inpLine);
                    strcat(tokenBuffer,buffer);
                }
            }
            while(isdigit(inpLine));
            ungetc(inpLine,inFile);
            sprintf(tempBuffer, "Token number: %d \t Token type: %s \t Actual token: %s\n", INTLITERAL, "INTLITERAL", tokenBuffer);
            strcat(lineBuffer,buffer);
            tokenReturned = INTLITERAL;
            break;
        case LPAREN: //LPAREN
            sprintf(buffer,"%c",inpLine);
            strcpy(tokenBuffer, buffer);
            sprintf(tempBuffer, "Token ID: %d \t Token type: %s \t Actual token: %s\n", LPAREN, "LPAREN", tokenBuffer);
            fprintf(outFile, "%s", tempBuffer);
            strcat(lineBuffer,tokenBuffer);
            tokenReturned = LPAREN;
            break;
        case RPAREN: //RPAREN
            sprintf(buffer,"%c",inpLine);
            strcpy(tokenBuffer, buffer);
            sprintf(tempBuffer, "Token ID: %d \t Token type: %s \t Actual token: %s\n", RPAREN, "RPAREN", tokenBuffer);
            fprintf(outFile, "%s", tempBuffer);
            strcat(lineBuffer,tokenBuffer);
            tokenReturned = RPAREN;
            break;
        case SEMICOLON: //SEMICOLON
            sprintf(buffer,"%c",inpLine);
            strcpy(tokenBuffer, buffer);
            sprintf(tempBuffer, "Token ID: %d \t Token type: %s \t Actual token: %s\n", SEMICOLON, "SEMICOLON", tokenBuffer);
            fprintf(outFile, "%s", tempBuffer);
            strcat(lineBuffer,tokenBuffer);
            tokenReturned = SEMICOLON;
            break;
        case COMMA: //COMMA
            sprintf(buffer,"%c",inpLine);
            strcpy(tokenBuffer, buffer);
            sprintf(tempBuffer, "Token ID: %d \t Token type: %s \t\t Actual token: %s\n", COMMA, "COMMA", tokenBuffer);
            fprintf(outFile, "%s", tempBuffer);
            strcat(lineBuffer,tokenBuffer);
            tokenReturned = COMMA;
            break;
        case ASSIGNOP: //:, test for =
            sprintf(buffer,"%c", inpLine);
            strcpy(tokenBuffer,buffer);
            inpLine = fgetc(inFile);
            if (inpLine == '=')
            {
                sprintf(buffer,"%c",inpLine);
                strcat(tokenBuffer,buffer);
                sprintf(tempBuffer, "Token ID: %d \t Token type: %s \t Actual token: %s\n",ASSIGNOP, "ASSIGNOP", tokenBuffer);
            }
            else
            {
                ungetc(inpLine, inFile);
                sprintf(errorLine, "Error. %s is not recognized\n", buffer);
                sprintf(tempBuffer, "Token ID: %d \t Token type: %s \t\t Actual token: %s\n",ERROR, "ERROR", tokenBuffer);
                tokenReturned = ERROR;
                //print error token
            }
            fprintf(outFile, "%s", tempBuffer);
            //strcat (tokenBuffer,charBuffer);
            tokenReturned = ASSIGNOP;
            break;
        case PLUSOP: //PLUSOP
            sprintf(lineBuffer, "%d ", lineNum);
            sprintf(buffer,"%c",inpLine);
            sprintf(tempBuffer, "Token ID: %d \t token type: %s \t Actual token: %c\n", PLUSOP, "PLUSOP", inpLine);
            strcat(lineBuffer,tempBuffer);
            fprintf(outFile, "%s", lineBuffer);
            tokenReturned = PLUSOP;
            break;
        case MINUSOP: //minusop, check for comment (--)
            sprintf(buffer, "%c", inpLine);
            strcpy(tokenBuffer,buffer);
            inpLine = fgetc(inFile);
            if (inpLine == '-')
            {
                sprintf(buffer, "%c", inpLine);
                strcat(tokenBuffer,buffer);
                strcat(lineBuffer,tokenBuffer);

                do
                {
                    inpLine = fgetc(inFile);
                    sprintf(buffer, "%c", inpLine);
                    if(inpLine != '\n' && inpLine != EOF)
                        strcat(lineBuffer, buffer);
                    else
                        ungetc(inpLine, inFile);

                }
                while (inpLine != '\n' || inpLine != EOF);
            }
            else
            {
                ungetc(inpLine, inFile);
                sprintf(tempBuffer, "Token ID: %d \t Token type: %s \t\t Actual token: %s\n",MINUSOP, "MINUSOP", tokenBuffer);
                tokenReturned = MINUSOP;
            }
            break;
        case BLANKSPACE: //is space
            //sprintf(buffer, "%c", inpLine);
            //strcat(lineBuffer, buffer);
            break;
        default:
            //all are errors.
            break;

        }

    }
    while (tokenReturned == BLANKSPACE);
    return tokenReturned;
}
