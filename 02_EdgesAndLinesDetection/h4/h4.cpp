/******************************************************************************************
 Title          : h4.cpp
 Author         : Ilona Michalowska
 Created on     : October 20, 2015
 Description    : The program finds lines in the image from its Hough Transform space using 
                  a given threshold and draws the detected lines on a copy of the original 
                  scene image.
 Usage          : ./h4 <arg1> <arg2> <arg3> <arg4>
                  where:
                  <arg1> is an input original gray-level image
                  <arg2> is an input gray-level Hough image
                  <arg3> is an input gray-level Hough threshold value
                  <arg4> is an output gray-level line image
 Comments       : Hough image is thrsholded  and saved as a grey-level image and its binary 
                  copy. The objects in the binary copy are labeled, and a HoughDatabase
                  object is used to record all "areas of brightness" weights and weighted
                  positions. HoughDatabase class contains the optimize member function that
                  further averages values of rho and theta if the values for two records
                  differ by less than 10 pixels and 7 degress, respectively.
                  Drawing the lines in the original image that do not extend beyond the 
                  actual edges is implemented with the use of its smoothed with the Gaussian
                  filter and thresholded binary Sobel image. Lines are drawn only on these
                  pixels in the original image that have corresponding black (0) pixels in
                  the Sobel image.
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
    
    if (argc!=5) {
        showUsage(argv[0]);
        return 0;
    }
    
    Image *input, *inputCopy, *Hough, *Sobel, *output;
    input = new Image;
    assert(input != 0);
    Hough = new Image;
    assert(Hough != 0);
    Sobel = new Image;
    assert(Sobel != 0);
    output = new Image;
    assert(output != 0);
    
    if (readImage(input, argv[1])) {
        printf("Can't open file %s\n", argv[1]);
        return 0;
    }
    
    inputCopy = new Image(*input);
    assert(inputCopy != 0);

    //writeImage(input, "input.pgm");

    if (readAndThresholdImage(Hough, argv[2], atoi(argv[3]))) {
        printf("Can't open file %s\n", argv[2]);
        return 0;
    }
    //writeImage(Hough, "Hough_T.pgm");

    setRhoShiftForHoughImage(input, Hough);
    HoughDatabase db;
    findLocalMaxima(Hough, db);
    
    /* For drawing detected lines that do not extend beyond actual edges */
    apply5x5GaussianFilter(input);
    //writeImage(input, "input_G.pgm");
    applySobelOperator(input, Sobel);
    //writeImage(Sobel, "input_G_S.pgm");
    apply5x5GaussianFilter(Sobel);
    //writeImage(Sobel, "input_G_S_G.pgm");
    thresholdAndMakeBinaryImage(Sobel, 15);
    //writeImage(Sobel, "input_G_S_G_TB.pgm");
    
    drawLines(input, db);
    drawLines(inputCopy, db, Sobel);

    if (writeImage(input, argv[4])) {
        printf("Can't write to file %s\n", argv[3]);
        return 0;
    }
    if (writeImage(inputCopy, "edges.pgm")) {
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
    << "Usage:\t" << fileName << " <arg1> <arg2> <arg3> <arg4>\n"
    << "********************************************************************************\n"
    << "where:\n"
    << "\t<arg1> is an input original gray-level image\n"
    << "\t<arg2> is an input gray-level Hough image\n"
    << "\t<arg3> is an input gray-level Hough threshold value\n"
    << "\t<arg4> is an output gray-level line image\n"
    << "example:\n\t" << fileName <<  " inputImage.pgm inputHoughImage.pgm 100 output.pgm\n";
}