#ifndef PNG_H
#define PNG_H
#include <string>
#include <QtGui\QImage>
#include <QtGui\QImageWriter>
#include <cmath>
#include <iostream>

using namespace std ;

class Steganography
{
public:
    Steganography();

};

const int bitsPerLetter = 8;

string decToBin(int num, int pad) ;
string messageToBinaryString(string str, int lbits) ;
int binStringToInt(string str) ;
string binaryStringToMessage(string binStr) ;
int createMask(int lbits) ;
bool doesLSBOneOrZero(int value) ;
void convertToBinary(unsigned int n) ;
void EmbbedMessage(const QImage &origImage, QImage & newImage, string msg, int leastBits) ;
string ExtractMessage(const QImage &stegoImge) ;

#endif // PNG_H