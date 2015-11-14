/******************************************************************************************
 Title          : h2.cpp
 Author         : Ilona Michalowska
 Created on     : October 22, 2015
 Description    : The program converts a gray–level image to a binary one using a command-
                  line provided threshold value.
 Usage          : ./h2 <arg1> <arg2> <arg3>
                  where:
                  <arg1> is an input gray–level image
                  <arg2> is an input gray–level threshold
                  <arg3> is an output binary image
 Comments       : The background in the input image is darker than the objects.
                  The background in the output image is black (0) and the objects are white
                  (255).
 ******************************************************************************************/

#include <cassert>
#include <string>
#include <iostream>
#include "Image.h"

using namespace std;

/**
 * Print information on how to run the program.
 */
void showUsage(string fileName);

/******************************************************************************************
 * MAIN
 ******************************************************************************************/
int main(int argc, char **argv) {
    
    if (argc!=4) {
        showUsage(argv[0]);
        return 0;
    }
    
    Image *im;
    im=new Image;
    assert(im!=0);
    
    if (readAsBinaryImage(im, argv[1], atoi(argv[2]))!=0) {
        printf("Can't open file %s\n", argv[1]);
        return 0;
    }
    
    if (writeImage(im, argv[3])!=0) {
        printf("Can't write to file %s\n", argv[3]);
        return 0;
    }
    
    return 0;
}

/******************************************************************************************
 * showUsage
 ******************************************************************************************/
void showUsage(string fileName) {
    cout << "********************************************************************************\n"
    << "Usage:\t" << fileName << " <arg1> <arg2> <arg3>\n"
    << "********************************************************************************\n"
    << "where:\n"
    << "\t<arg1> is an input gray–level image\n"
    << "\t<arg2> is an input gray–level threshold\n"
    << "\t<arg3> is an output binary image\n"
    << "example:\n\t" << fileName <<  " input.pgm 100 output.pgm\n";
}