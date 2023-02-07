#include "PNG.h"
#include <QtCore\QDebug>
#include <iostream>
#include <string>
#include <bitset>
#include <sstream>
Steganography::Steganography()
{
}


string decToBin(int num, int pad)
{
    int r =  0;    
    int q = -1;    
    string b = ""; 

    while(q != 0)
    {
        r = num%2;
        q = num/2;
        num = q;
        if (r) b.insert(0,"1");
        else   b.insert(0, "0");
    }

    
    while(b.length() < (unsigned long) pad)
        b.insert(0, "0");

    return b;
}

string messageToBinaryString(string str, int lbits){
    string binStr = "";
    int temp;

    
    for(unsigned int i = 0; i<str.length(); i++)
    {
        
        temp = static_cast<int>(str[i]);
        binStr.append(decToBin(temp, bitsPerLetter));
    }


    while(binStr.length() % lbits != 0)
        binStr.append("0") ;


    return binStr ;

}

int binStringToInt(string str){

    int value = 0 ;
    for(unsigned int i = 0; i < str.length(); i++){
        value=value << 1 ;
        if(str[i] == '1'){
            value = value | 1 ;
        }
    }
    return value ;
}

int createMask(int lbits){
    return (1 << lbits) - 1;
}


bool doesLSBOneOrZero(int value) {
    if(value & 1) {
        return true;
    } else {
        return false;
    }
}

void convertToBinary(unsigned int n)
{
    if (n / 2 != 0) {
        convertToBinary(n / 2);
    }
    printf("%d", n % 2);
    cout << endl;
}

void EmbbedMessage(const QImage & origImage, QImage & newImage, string msg, int leastBits){
    string binMsg ;
    unsigned long msg_index = 0 ;     
    int red, green, blue ;
    int mask = createMask(leastBits);
    QRgb p;

    binMsg = messageToBinaryString(msg, leastBits);

    binMsg.append(2*bitsPerLetter,'0');
    cout << binMsg << endl;


    for(int i = 0; i < origImage.width(); i++)
    {
        for(int j = 0; j < origImage.height(); j++)
        {
            p = origImage.pixel(i,j);

           
            red  = qRed(p);

            if(msg_index < binMsg.length()){
                
                red &= ~mask;
                
                red = red |  binStringToInt(binMsg.substr(msg_index, leastBits)) ;
                msg_index += leastBits ;
            }
            
            
           
            green = qGreen(p);

            if(msg_index < binMsg.length()){
                green &= ~mask;
                green = green | binStringToInt(binMsg.substr(msg_index, leastBits)) ;
                msg_index += leastBits ;
            }

            

            
            blue  = qBlue(p);

            if(msg_index < binMsg.length()){
                blue &= ~mask;
                blue = blue |  binStringToInt(binMsg.substr(msg_index, leastBits)) ;
                msg_index += leastBits ;
            }

            

            newImage.setPixel(i,j,qRgb(red, green, blue));
        }
    }
}


string binaryStringToMessage(string binStr){
    
    string text = "";
    stringstream sstream(binStr);
    while (sstream.good())
    {
        bitset<8> bits;
        sstream >> bits;
        text += char(bits.to_ulong());
    }
    return text;
    
}

/*Nie udało mi się sprawić, aby metoda pobierała ukrytą wiadomość z formatu PNG*/
string ExtractMessage(const QImage &stegoImage/*, int leastBits*/){
    
    string binMsg = "";
    string tempMessage = "";

    int red, green, blue;
    QRgb p;
    unsigned int end = 0;
    
        for(int i = 0; i < 14; i++)
        {
            for(int j = 0; j < stegoImage.height(); j++)
            {
                p = stegoImage.pixel(i,j);

                red  = qRed(p);

               
                if (doesLSBOneOrZero(red))
                {
                    tempMessage += "1";
                    
                } else {
                    tempMessage += "0";
                }
                

                green = qGreen(p);
                
                if (doesLSBOneOrZero(green))
                {
                    tempMessage += "1";
                    
                } else {
                    tempMessage += "0";
                    
                }
                

                blue  = qBlue(p);
                
                if (doesLSBOneOrZero(blue))
                {
                    tempMessage += "1";
                    
                } else {
                    tempMessage += "0";
                }
                
                
                if ((j+1)%3 == 0 && tempMessage.compare("00000000") != 0)
                {
                    binMsg +=tempMessage;
                    tempMessage = "";
                } else if ((j+1)%3 == 0 && tempMessage.compare("00000000") == 0) {
                    end = 1;
                    break;
                }
            }
            tempMessage = "";
            if (end == 1)
            {
                break;
            }
        }
    

    return binaryStringToMessage(binMsg);
   
}
