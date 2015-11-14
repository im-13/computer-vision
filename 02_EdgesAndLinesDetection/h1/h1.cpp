/******************************************************************************************
 Title          : h1.cpp
 Author         : Ilona Michalowska
 Created on     : October 20, 2015
 Description    : The program finds the locations of edge points in the image.
 Usage          : ./h1 <arg1> <arg2>
                  where:
                  <arg1> is an input gray-level image
                  <arg2> is an output gray-level edge image
 Comments       : Input image is filtered with 5x5 Gaussian filter and then Sobel operator
                  is used to produce an "edge" image where the intensity at each point is 
                  proportional to edge magnitude.
 ******************************************************************************************/

#include <iomanip>
#include <cassert>
#include <string>
#include <iostream>
#include <vector>
#include "Image.h"

using namespace std;

/**
 * Print information on how to run the program.
 */
void showUsage(string fileName);

/******************************************************************************************
 * MAIN
 ******************************************************************************************/
int main(int argc, char * argv[]) {
    
    if (argc!=3) {
        showUsage(argv[0]);
        return 0;
    }
    
    Image *input, *output;
    input = new Image;
    assert(input != 0);
    output = new Image;
    assert(output != 0);
    
    if (readImage(input, argv[1])) {
        printf("Can't open file %s\n", argv[1]);
        return 0;
    }
    
    apply5x5GaussianFilter(input);
    applySobelOperator(input, output);
    
    if (writeImage(output, argv[2])) {
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
    << "Usage:\t" << fileName << " <arg1> <arg2>\n"
    << "********************************************************************************\n"
    << "where:\n"
    << "\t<arg1> is an input gray-level image\n"
    << "\t<arg2> is an output gray-level edge image\n"
    << "example:\n\t" << fileName <<  " input.pgm output.pgm\n";
}