#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void printBytes(char *buff)
{
    int i;
    for(i = 0; i < sizeof(buff); i++)
    {
        printf("%u ", buff[i]);

        if(buff[i+1] == '\0')
        { 
            printf("\n");
            return;
        }
    }
}

int main(int argc, char *argv[])
{
    char infile[128];
    char outfile[128];
    char buffer[512];

    memset(infile, '\0', sizeof(infile));
    memset(infile, '\0', sizeof(outfile));
    memset(buffer, '\0', sizeof(buffer));

    strcpy(infile, argv[1]);
    strcpy(outfile, argv[2]);

    FILE *fptr1 = fopen(infile, "rb");
    FILE *fptr2 = fopen(outfile, "wb+");

    fread(buffer, sizeof(buffer), 1, fptr1);
    printBytes(buffer); 
    fwrite(buffer, sizeof(buffer), 1, fptr2);
    return 0;
}
