/******************************************************************************************
 Title          : s2.cpp
 Author         : Ilona Michalowska
 Created on     : November 12, 2015
 Description    : The program finds the normal from the brightest surface spot on the sphere
                  in each of the 3 input images, directed toward the light source.
 Usage          : ./s2 <arg1> <arg2> <arg3> <arg4> <arg5>
                  where:
                  <arg1> is an input sphere parameters file
                  <arg2> is an input image 1
                  <arg3> is an input image 2
                  <arg4> is an input image 3
                  <arg5> is an output directions file
 Comments       : Formula to compute the normal vector gives the result in a 3-D coordinate 
                  system, originating at the sphere’s center, having its x-axis and y-axis 
                  parallel respectively to the x-axis and the y-axis of the image, and z-axis 
                  chosen such as to form an orthonormal right-hand coordinate system.
                  Intensity of the light source is the magnitude of the brightest pixel
                  found in the corresponding image; normal vector is scaled so that its
                  length equals this value. The resulting output file is a plain text file 
                  that consists of 3 lines. Line i contains the x-, y-, and z-components 
                  (separated by a space) of the vector computed for light source i.
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
    
    if (argc!=6) {
        showUsage(argv[0]);
        return 0;
    }
    
    /* Read images */
    
    Image *input1;
    input1 = new Image;
    assert(input1 != 0);
    Image *input2;
    input2 = new Image;
    assert(input2 != 0);
    Image *input3;
    input3 = new Image;
    assert(input3 != 0);
    
    if (readImage(input1, argv[2])) {
        printf("Can't open file %s\n", argv[2]);
        return 0;
    }
    if (readImage(input2, argv[3])) {
        printf("Can't open file %s\n", argv[3]);
        return 0;
    }
    if (readImage(input3, argv[4])) {
        printf("Can't open file %s\n", argv[4]);
        return 0;
    }
    
    /* Read sphere properties, calculate light sources directions and intensities, save results in a text file */
    
    if (calculateLightSourcesDirectionsAndIntensities(argv[1], input1, input2, input3, argv[5])) {
        printf("Can't calculate light sources directions and intensities\n");
        return 0;
    }
    
    return 0;
}

/******************************************************************************************
 * showUsage
 ******************************************************************************************/
void showUsage(string fileName) {
    cout << "********************************************************************************\n"
         << "Usage:\t" << fileName << " <arg1> <arg2> <arg3> <arg4> <arg5>\n"
         << "********************************************************************************\n"
         << "where:\n"
         << "\t<arg1> is an input sphere parameters file\n"
         << "\t<arg2> is an input image 1\n"
         << "\t<arg3> is an input image 2\n"
         << "\t<arg4> is an input image 3\n"
         << "\t<arg5> is an output directions file\n"
         << "example:\n\t" << fileName << " sphereParameters.txt input1.pgm input2.pgm input3.pgm directions.txt\n";
}