/******************************************************************************************
 Title          : s3.cpp
 Author         : Ilona Michalowska
 Created on     : November 12, 2015
 Description    : The program, given 3 images of an object, computes the normals to that 
                  object’s surface.
 Usage          : ./s3 <arg1> <arg2> <arg3> <arg4> <arg5> <arg6> <arg7>
                  where:
                  <arg1> is an input directions file
                  <arg2> is an input image 1
                  <arg3> is an input image 2
                  <arg4> is an input image 3
                  <arg5> is a step
                  <arg6> is a threshold value
                  <arg7> is an output image
 Comments       : The output is image 1 with the normals shown as a "needle map". The 
                  gridpoints are shown in black (value 0). "Needles" originate from each
                  gridpoint as lines (orthographic projections of the scaled by factor 10
                  normal vectors at these points onto the image plane) in white color (255).
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
    
    if (argc!=8) {
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
    
    /* Copy input1 image to draw needle map */
    
    Image *output;
    output = new Image(*input1);
    assert(output != 0);

    
    /* Read light source directions, compute and draw normals */
    
    if (computeAndDrawNormals(argv[1], input1, input2, input3, atoi(argv[5]), atoi(argv[6]), output)) {
        printf("Can't compute and draw normals\n");
        return 0;
    }
    
    if (writeImage(output, argv[7])) {
        printf("Can't write to file %s\n", argv[7]);
        return 0;
    }
    
    return 0;
}

/******************************************************************************************
 * showUsage
 ******************************************************************************************/
void showUsage(string fileName) {
    cout << "********************************************************************************\n"
         << "Usage:\t" << fileName << " <arg1> <arg2> <arg3> <arg4> <arg5> <arg6> <arg7>\n"
         << "********************************************************************************\n"
         << "where:\n"
         << "\t<arg1> is an input directions file\n"
         << "\t<arg2> is an input image 1\n"
         << "\t<arg3> is an input image 2\n"
         << "\t<arg4> is an input image 3\n"
         << "\t<arg5> is an input step\n"
         << "\t<arg6> is an input threshold value\n"
         << "\t<arg7> is an output image\n"
         << "example:\n\t" << fileName << " directions.txt input1.pgm input2.pgm input3.pgm 30 90 output.pgm\n";
}