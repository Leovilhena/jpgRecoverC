/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */
 
#include <stdio.h>
#include <cs50.h>
#include <string.h>


// compare function declaration
bool comp(char* b);

// main function
int main(void)
{
    // open main file
    FILE* card = fopen("card.raw", "r");
    if (card == NULL)
    {
        printf("File does not exist.\n");
        return 1;
    }
    
    // buffer array
    char buffer[512]; 
    
    // jpg found counter
    int jcounter = 0;
    
    // array for file title
    char title[8];
    
    // variable to check if out stream is open = 1 or closed = 0
    int stream = 0;
    
    // interate til end of the file
    while(stream == 0)
    {

        // if found a jpg signature
        if(comp(buffer) == true)
        {
            // creates a file name based on how many jpg's file were found (jcounter)
            sprintf(title, "%03d.jpg", jcounter);
            
            // open the file for writting
            FILE* out = fopen(title, "w");
            
            // outfile stream is open
            stream = 1; 
            
            // writes into the outstream file
            fwrite(&buffer, sizeof(buffer), 1, out);
            
            // while file is open
            while(stream == 1) 
            {
                fread(&buffer, sizeof(buffer), 1, card);
                fwrite(&buffer, sizeof(buffer), 1, out);
                
                // check for another jpg signature
                if(comp(buffer) == true)
                {
                    jcounter++;
                    
                    // outstream and file are closed
                    fclose(out);
                    stream = 0;
                }
                else if(feof(card))
                {
                    fclose(out);
                    break;
                }
                    
            }
        }
        else
        {
            // read chunks of 512 bytes from buffer
            fread(&buffer, sizeof(buffer), 1, card);
        }
    }
    
    // see ya later alligator!
    fclose(card);
    
}

/** function that checks if 4 bytes from buffer are the same as jpg's signature
 *  Returns true if bytes are equal
 */
bool comp(char* b)
{
    // jpg's signature (MagicNumber)
    char n1[4] = {0xff, 0xd8, 0xff, 0xe0};
    char n2[4] = {0xff, 0xd8, 0xff, 0xe1};
    
    // condition to check if we have a jpg signature!
    if(memcmp(b, n1, 4) == 0 || memcmp(b, n2, 4) == 0)
    {
        return true;
    }
    else
        return false;
}
