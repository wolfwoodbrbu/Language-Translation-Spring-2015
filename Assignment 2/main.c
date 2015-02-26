#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "header.h"
#include "globals.h"
#include "scanner.h"

/*CSC 460 - Language Translation
Group 2 - Faye Bickerton, Zach Couch, Jared Sheppard
*/

char *input_file_directory;
char *input_file_name;
char *input_file_extension;
char *output_file_directory;
char *output_file_name;
char *output_file_extension;

FILE *inFile;
FILE *outFile;



/** \brief Returns the char array of the next line in the input accounts for new line and empty line.
 *
 * \param s - a char array to put the user input into.
 * \param n - size of buffer to read.
 * \param f - input type.
 * \return
 *
 */
char *GetLine (char *s, int n, FILE *f)
{
    // copy next line into p
    char *p = fgets (s, n, f);

    // check if it was empty.
    if (p != NULL)
    {
        s[strlen(s)-1] = '\0';
    }
    return p;
}

void GetPathParts(char **file_name, char **file_extension, char **file_path, char *expected_extension)
{

    char extracted_file_name[100];
    char extracted_extension[100];
    char extracted_path[100];
    char *extension_ptr = NULL;
    char *path_ptr = NULL;
    char *file_name_ptr = NULL;
    int j;
    size_t namesize;

    memset(extracted_file_name, 0,  sizeof(extracted_file_name));
    memset(extracted_extension, 0,  sizeof(extracted_extension));
    memset(extracted_path,      0,  sizeof(extracted_path));

    file_name_ptr = *file_name;

    path_ptr = file_name_ptr;

    if(file_name == NULL)
    {
        return;
    }
    namesize = strlen(file_name_ptr);
    for(j = 0; j <= namesize; j++)
    {
        if( file_name_ptr[j] == '\\' )
        {
            path_ptr = &file_name_ptr[j] + 1;
        }
        if( file_name_ptr[j] == '.' )
        {
            extension_ptr = &file_name_ptr[j];
        }
    }

    if ( path_ptr != file_name_ptr )
    {
        size_t path_legnth = 0;
        path_legnth = (path_ptr) - file_name_ptr;
        strncpy(extracted_path, file_name_ptr, path_legnth);
    }

    if ( extension_ptr != NULL )
    {
        strcpy(extracted_extension,extension_ptr);
    }
    else
    {
        strcpy(extracted_extension, expected_extension);
    }

    size_t name_legnth = 0;

    if(extension_ptr != NULL)
    {
        name_legnth = extension_ptr - path_ptr;
    }
    else
    {
        name_legnth = namesize;
    }

    strncpy(extracted_file_name, path_ptr, name_legnth);

    *file_name = malloc(sizeof(extracted_file_name) + 1);
    *file_extension = malloc(sizeof(extracted_extension) + 1);
    *file_path = malloc(sizeof(extracted_path) + 1);

    strcpy(*file_name, extracted_file_name);
    strcpy(*file_extension,  extracted_extension);
    strcpy(*file_path, extracted_path);

    return;
}

FILE *GetInputFile(int arg)
{
    bool goodToGo = false;
    bool first = true;
    char p[BUFFER_LEGNTH];
    FILE *inFile;
    char filename[BUFFER_LEGNTH];
    memset(p,0,sizeof(p));
    memset(filename,0,sizeof(filename));

    do
    {
        if(arg == 1)
        {
            if (first)
            {
                printf("Please enter the Input Filename:\n");
                first = false;
            }
            else
            {
                printf("Invalid Source File!\n\n");
                printf("Please enter the Input Filename:\n");
            }
            input_file_name = GetLine(p,BUFFER_LEGNTH,stdin);
            if ( input_file_name[0] == '\0' )
            {
                // Exit Program
                return NULL;
            }
            // We Got Something
        }

        GetPathParts(&input_file_name, &input_file_extension, &input_file_directory, ".IN");

        printf("\nInput File Directory:  %s\n", input_file_directory);
        printf("Input File Name:       %s\n",   input_file_name);
        printf("Input File Extension:  %s\n\n", input_file_extension);

        strcat(filename, input_file_directory);
        strcat(filename, input_file_name);
        strcat(filename, input_file_extension);

        if( access(filename, F_OK) != -1 )
        {
            inFile = fopen(filename, "r");
            if (inFile != NULL )
            {
                goodToGo = true;
            }
        }
        else
        {
            printf("%s does not exist\n\n", filename);
            arg = 1;
        }

    }
    while (!goodToGo);

    return inFile;
}


FILE *GetOutputFile(int arg)
{
    char p[BUFFER_LEGNTH];
    FILE *outFile;
    char backup_file_name[BUFFER_LEGNTH];
    char filename[BUFFER_LEGNTH];
    memset(p,0,sizeof(p));
    memset(filename,0,sizeof(filename));
    memset(backup_file_name,0,sizeof(backup_file_name));

    if(arg <= 2)
    {
        printf("Please enter the Output Filename:\n");
        output_file_name = GetLine(p,BUFFER_LEGNTH,stdin);
        if ( output_file_name[0] == '\0' )
        {
            printf("\nNo output file name given. Defaulting to input file name.");
            output_file_name = input_file_name;
            output_file_directory = input_file_directory;
        }
    }

    GetPathParts(&output_file_name, &output_file_extension, &output_file_directory, ".OUT");

    printf("\nOutput File Directory:  %s\n",    output_file_directory);
    printf("Output File Name:       %s\n",      output_file_name);
    printf("Output File Extension:  %s\n\n",    output_file_extension);

    strcat(filename, output_file_directory);
    strcat(filename, output_file_name);
    strcat(filename, output_file_extension);

    if( access(filename, F_OK) != -1 )
    {
        // back up file here
        printf("Backing up old output file.\n");
        strcpy(backup_file_name,filename);
        strcat(backup_file_name, ".old");
        if( access(backup_file_name, F_OK) != -1 )
        {
            printf("Removing old backup.\n");
            remove(backup_file_name);
        }
        rename(filename, backup_file_name);
    }
    outFile = fopen(filename, "w");

    return outFile;

}

int Init(int argc, char *argv[])
{

    //printf("cmdline args count=%d \n", argc);
    //Initialize Variables
    input_file_name = NULL;
    output_file_name = NULL;

    //char copyChar;


    printf(argv[0]);
    printf("\n");

    if(argc > 1)
    {
        input_file_name = argv[1];
        printf(input_file_name);
        printf("\n");
        //begin checking arguments
        if(argc > 2)
        {
            output_file_name = argv[2];
            printf(output_file_name);
            printf("\n");
            if ((outFile = GetOutputFile(argc)) == NULL)
            {
                return 0;
            }
        }
    }

    else
    {
        // Get some user Input
        printf("\n\n");

        if ((inFile = GetInputFile(argc)) == NULL)
        {
            return 0;
        }

        if ((outFile = GetOutputFile(argc)) == NULL)
        {
            fclose(inFile);
            return 0;
        }



    }
    return 1;
}

void SystemGoal()
{
    printf("Scanning for tokens.");
    Token response;
    do{
        response = Scanner();
    } while (response != SCANEOF);

}

void WrapUp()
{
    if ( inFile != NULL ) fclose(inFile);
    if ( outFile != NULL ) fclose(outFile);
}



int main(int argc, char *argv[])
{
    if(Init(argc, argv)) {
        SystemGoal();
        WrapUp();
    }
    system("PAUSE");
    return 0;
}
