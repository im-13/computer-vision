/******************************************************************************************
 Title          : h3.cpp
 Author         : Ilona Michalowska
 Created on     : October 20, 2015
 Description    : The program generates an image of the Hough Transform space of a given 
                  binary edge image.
 Usage          : ./h3 <arg1> <arg2>
                  where:
                  <arg1> is an input binary edge image
                  <arg2> is an output gray-level Hough image
 Comments       : The brightness of each pixel (voting bin) in the output image is 
                  proportional to the number of votes it received.
                  Hough image is an array of size:
                  - 360 columns corresponding to 180 degrees
                    (sampling size for theta is 0.5 degree)
                  - sqrt(N^2 + N^2)/2 * 2 rows
                    (sampling size for rho is 0.5 pixel)
                    rho dimension is doubled to accomodate negative values, i.e. all
                    indices are shifted by sqrt(N^2 + N^2)/2.
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
    
    HoughTransform(input, output);

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
    << "\t<arg1> is an input binary edge image\n"
    << "\t<arg2> is an output gray-level Hough image\n"
    << "example:\n\t" << fileName <<  " input.pgm output.pgm\n";
}