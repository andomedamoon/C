
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include "BMP.h"                                                                    
#include "PNG.h"
using namespace std;

                                                             
FILE* image;                                                                            
FILE* hidden;                                                                           
FILE* output;                                                                           

int main()
{
    string sel;                                                                             
    char yesno;                                                                             
    string check = "-i";
    string hideMessage = "-e";
    string decodeMessage = "-d";
    string isEnough = "-c";
    string programInfo = "-h";
    string quit = "-q";
    do                                                                      
    {
        
        printf("What would you like to do?\n"                                               
        "\t-i)Print image header.\n"
        "\t-e)Hide message.\n"
        "\t-d)Decode message.\n"
        "Selection: ");                                                              
        cin >> sel;
        string path;
        if (sel.compare(check) == 0)
        {
                                                                                    
            printf("\nPlease provide the path of the image: ");                     
            cin >> path;
            printf("\n");
            const char* ext = strrchr(path.c_str(), '.');

            if(ext != nullptr) {
            if (strcmp(ext, ".png") == 0 || strcmp(ext, ".bmp") == 0)
            {            
            open_image(path.c_str());                                                 
            print_header(image);                                                     
            print_last_modified(path.c_str());
            fclose(image);                                                            
            goForward();
            } else {
                cout << "The given path leads to a file with an unsupported extension!" << endl;
                goForward();
            }   
            } else {
                cout << "The given path is invalid!" << endl;
                goForward();
            }
        } else if (sel.compare(hideMessage) == 0)
        {
            string hiddenMessage;
            printf("\nPlease provide the path of the image: ");                     
            cin >> path;
            if (check_if_right_path(path.c_str()))
            {
                const char* ext = strrchr(path.c_str(), '.');
                if (strcmp(ext, ".bmp") == 0) {
                    open_image(path.c_str());
                    printf("Please provide your message to hide: ");
                    cin.ignore();
                    getline(cin, hiddenMessage);
                    std::ofstream outfile ("TempToHide.txt");
                    outfile << hiddenMessage << std::endl;
                    open_hidden("TempToHide.txt");
                    
                    hide_message("output.bmp");
                    printf("Your message has been hidden in the output.bmp file!\n");
                    goForward();

                } else if (strcmp(ext, ".png") == 0) {
                    printf("Please provide your message to hide: ");
                    cin.ignore();
                    getline(cin, hiddenMessage);
                    QImage image2;
                    bool isSet = image2.load(path.c_str());
                    QImage newImage("output.png");
                    newImage = image2.copy();
                    QImageWriter writer("C:\\users\\breda\\Desktop\\TestQt\\output.png");
                    writer.write(newImage);
                    EmbbedMessage(image2, newImage, hiddenMessage, 3);
                    writer.write(newImage);
                    printf("Your message has been hidden in the output.png file!\n");
                    goForward();
                    
                }
            } else {
                cout << "The path is invalid!\n" << endl;
                goForward();
            }
        } else if (sel.compare(decodeMessage) == 0) {
            printf("\nPlease provide the path of the image: ");                     
            cin >> path;
            if (check_if_right_path(path.c_str()))
            {
                const char* ext = strrchr(path.c_str(), '.');
                if (strcmp(ext, ".bmp") == 0)
                {
                
                int amount;
                cout << "Provide length of your hidden message: ";
                cin >> amount;
                unhide_message(path.c_str(), amount);
                goForward();
                } else if (strcmp(ext, ".png") == 0)
                {
                    QImage temp; 
                    bool isSet = temp.load(path.c_str());
                    cout << ExtractMessage(temp) << endl;
                    goForward();
                }
                
            } else {
                cout << "The path is invalid!\n" << endl;
                goForward();
            }
        } else if (sel.compare(isEnough) == 0) {
            string hiddenMessage;
            printf("\nPlease provide the path of the image: ");                     
            cin >> path;
            if (check_if_right_path(path.c_str()))
            {
                printf("Please provide your message to hide: ");
                cin.ignore();
                getline(cin, hiddenMessage);
                std::ofstream outfile ("Temp.txt");
                outfile << hiddenMessage << std::endl;
                int imageSize = giveMeSize(path.c_str());
                int messageSize = giveMeSize("Temp.txt");
                cout << "Size of image: " << imageSize << endl;
                cout << "Size of the message: " << messageSize << endl;
                unlink("Temp.txt");
                if (imageSize >= (messageSize * 8))
                {
                    cout << "The message can be hidden." << endl;
                    goForward();
                } else {
                    cout << "The message can't be hidden!" << endl;
                    goForward();
                }
                
            } else {
                cout << "The path is invalid!\n" << endl;
                goForward();
            }
        }
        
    } while (sel.compare(quit) != 0); 
    close_files();                                                                     
    return 0;
}


