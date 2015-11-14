/******************************************************************************************
 Title          : s1.cpp
 Author         : Ilona Michalowska
 Created on     : November 12, 2015
 Description    : The program locates the sphere in an image and computes its center and 
                  radius.
 Usage          : ./s1 <arg1> <arg2> <arg3>
                  where:
                  <arg1> is an input original gray-level image
                  <arg2> is an input threshold value
                  <arg3> is an output parameters file
 Comments       : Assuming an orthographic projection, the sphere projects into a circle on 
                  the image plane. The greyscale image is thresholded in orded to obtain a 
                  binary one. The resulting parameters file is a text file consisting of a 
                  single line containing the x-coordinate of the center, the y-coordinate 
                  of the center, and the radius of the circle, separated by a space.
 ******************************************************************************************/

#include <iomanip>
#include <cassert>
#include <string>
#include <iostream>
#include <vector>
#include "Image.h"
#include "DisjSets.h"
#include "HoughDatabase.h"
#include "Database.h"

using namespace std;

/**
 * Print information on how to run the program.
 */
void showUsage(string fileName);

/******************************************************************************************
 * MAIN
 ******************************************************************************************/
int main(int argc, char * argv[]) {
    
    if (argc!=4) {
        showUsage(argv[0]);
        return 0;
    }
    
    Image *input;
    input = new Image;
    assert(input != 0);
    
    if (readAsBinaryImage(input, argv[1], atoi(argv[2]))) {
        printf("Can't open file %s\n", argv[1]);
        return 0;
    }
    
    if (calculateSpherePropertiesAndSaveAsTxt(input, argv[3])) {
        printf("Can't calculate sphere properties\n");
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
    << "\t<arg1> is an input original gray-level image\n"
    << "\t<arg2> is an input input threshold value\n"
    << "\t<arg3> is an output parameters file\n"
    << "example:\n\t" << fileName <<  " inputImage.pgm 85 sphereProperties.txt\n";
}