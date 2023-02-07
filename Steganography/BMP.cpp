#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fstream>
#include <limits>
#include <conio.h>
#include "BMP.h"                                                   
using namespace std;

                                         
extern FILE* image;                                                        
extern FILE* hidden;                                                       
extern FILE* output;                                                       

unsigned char ident[2];                                             
unsigned int fsize;                                                 
unsigned int offset;                                                
int width;                                                          
int height;                                                         

void open_image(const char * imageName)                             
{
    image = fopen(imageName, "rb");                                 

    if (image == NULL)                                              
    {
        printf("ERROR FILE MISSING!");               

    }
}
void open_hidden(const char * fileName)                                                  
{
    hidden = fopen(fileName, "rb");                                   

    if (hidden == NULL)                                             
    {
        printf("\nCAN'T OPEN THE FILE.\n");                   
            

    }
}
void open_output(const char * imageName)                            
{
    output = fopen(imageName, "rb");                                   

    if (output == NULL)                                             
    {
        printf("\nNO STEGANOMETRY HAS TAKEN PLACE.\n");             
    }
}

bool check_if_right_path(const char * fileName) {
    std::ifstream test(fileName); 
  return (test) ? true : false;
}

void close_files()                                                  
{
    fclose(hidden);
    fclose(output);
    fclose(image);
}

int giveMeSize(const char * filename){
    
    ifstream mySource;
    mySource.open(filename, ios_base::binary);
    mySource.seekg(0,ios_base::end);
    int size = mySource.tellg();
    mySource.close();
    return size;
}

int countCharacters(const char * fileName) {
    string line;
    int sum=0;
    ifstream inData ;
    inData.open(fileName);

        while(!inData.eof())
        {
            getline(inData,line);

            int numofChars= line.length();
    
            sum=numofChars+sum;
        }
return sum;
}

void get_header(FILE* fp)                                           
{
    rewind(fp);                                                     
    fread(ident, sizeof(char), 2, fp);                              
    fread(&fsize, sizeof(int), 1, fp);                                 
    fseek(fp, 0xA, SEEK_SET);                                         
    fread(&offset, sizeof(int), 1, fp);                                
    fseek(fp, 0x12, SEEK_SET);                                        
    fread(&width, sizeof(int), 1, fp);                                 
    fseek(fp, 0x16, SEEK_SET);                                        
    fread(&height, sizeof(int), 1, fp);                                

}

void print_header(FILE* fp)                                        
{
    get_header(fp);                                                 
    printf("   FILE INFORMATION:\n");
    printf("   File Type:   %2c%c\n", ident[0], ident[1]);           
    printf("   File Length:  %d Bytes\n", fsize);                    
    printf("   BMP Information Offset: 0x%x\n", offset);            
    printf("   Image Width:  %d Pixels\n", width);                  
    printf("   Image Height: %d Pixels", height);                   
    ident[0] = 0;                                                    
    ident[1] = 0;
    fsize = 0;
    offset = 0;
    width = 0;
    height = 0;

}

void print_last_modified(const char * imageName) {
        
	char timeStr[100] = "";
	struct stat buf;
	time_t ltime;
	char datebuf [9];
	char timebuf [9];

	if (!stat(imageName, &buf))
	{
		strftime(timeStr, 100, "%d-%m-%Y %H:%M:%S", localtime( &buf.st_mtime));
		printf("\n   Last modified date and time = %s\n\n", timeStr);
	}
	else
	{
		printf("ERROR WITH GETTIN A TIME!\n");
	}
}

void hide_message(const char * imageName)                            
{
    get_header(image);                                              
    rewind(hidden);                                                 
    rewind(image);                                                  


    
    output = fopen(imageName, "wb");                                
    if (output == NULL)                                             
    {
        printf("ERROR, FILE MISSING!");               
        exit(0);                                                    
    }

    long sizeA, i, sizeB, h;                                         
    char* a, * b, * header;                                             
    unsigned char outMur;                                           
    int j, k;                                                       

    fseek(image, 0, SEEK_END);                                    
    sizeA = ftell(image);                                           
    fseek(hidden, 0, SEEK_END);                                   
    sizeB = ftell(hidden);                                          
    rewind(hidden);                                                 
    rewind(image);                                                  
    a = (char*)malloc(sizeA);                                       
    b = (char*)malloc(sizeB);                                       
    header = (char*)malloc(offset - 1);                               

    fread(a, sizeof(char), sizeA, image);                              
    fread(b, sizeof(char), sizeB, hidden);                             
    rewind(hidden);                                                 
    rewind(image);                                                  

    fread(header, sizeof(char), (offset), image);                      
    fwrite(header, sizeof(char), (offset), output);                    
    fseek(image, offset, SEEK_SET);                                   

    i = offset;                                                       
    for (h = 0; h < sizeB; h++)                                            
    {
        j = 7;                                                     
        for (k = 0; k <= 7; k++)                                           
        {
            outMur = (a[i] & (~1)) | ((1) & (b[h] >> j));           
            j--;                                                    
            i++;                                                    
            fputc(outMur, output);                                   
        }
    }
    while (i != (sizeA))                                            
    {
        fputc(a[i], output);
        i++;
    }
    free(a);                                                        
    free(b);                                                        
    free(header);                                                   
    fclose(hidden);                                                 
                                     

}

void unhide_message(const char * imageName, int amountOfCharacters)                                               
{
    long sizeA, sizeB, i, h;                                           
    char* a, * b;                                                     
    int k, j;                                                        

    open_output(imageName);                                                                                    
    if (output == NULL || hidden == NULL)                           
    {
        printf("ERROR, FILE MISSING!");
        exit(0);
    }
    get_header(output);                                             
    rewind(output);                                                 

    sizeB = amountOfCharacters;

    fseek(output, 0, SEEK_END);                                    
    sizeA = ftell(output);                                          
    a = (char*)malloc(sizeA);                                       
    b = (char*)malloc(sizeA);                                       
    rewind(output);                                                 

    fread(a, sizeof(char), sizeA, output);                             
    fseek(output, offset, SEEK_SET);                                  
    i = offset;                                                     
    h = 0;                                                          
    while (i <= sizeA)                                               
    {
        j = 7;                                                        
        for (k = 0; k <= 7; k++)                                           
        {
            b[h] = b[h] | ((a[i] & (1)) << j);                       
            i++;                                                    
            j--;                                                    
        }                                       
        cout << b[h];
        h++;                                                        
        if (h == sizeB)                                             
            break;
    }
    cout << endl;
    free(a);                                                        
    free(b);                                                        
    close_files();                                                  
                                           
}

void goForward() {
    cout << "Press Enter to Continue";
    getch();
}

