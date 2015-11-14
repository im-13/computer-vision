/******************************************************************************************
 Title          : p2.cpp
 Author         : Ilona Michalowska
 Created on     : September 22, 2015
 Description    : The program segments a binary image into several connected regions.
 Usage          : ./p2 <arg1> <arg2>
                  where:
                  <arg1> is an input binary image
                  <arg2> is an output labeled image
 Comments       : The background in the input image is black (0) and the objects are white
                  (255).
 ******************************************************************************************/

#include "Image.h"
#include "DisjSets.h"
#include <cstdio>
#include <iomanip>
#include <cassert>
#include <string>
#include <iostream>
#include <vector>

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
        //exit(1);
		return 0;
	}
	Image *im;
	im=new Image;
	assert(im!=0);
    if (readAndLabelBinaryImage(im, argv[1])!=0) {
		printf("Can't open file %s\n", argv[1]);
		return 0;
	}

	if (writeImage(im, argv[2])!=0) {
		printf("Can't write to file %s\n", argv[2]);
		return 0;
	}
}

/******************************************************************************************
 * showUsage
 ******************************************************************************************/
void showUsage(string fileName) {
    cerr << "********************************************************************************\n"
         << "Usage:\t" << fileName << " <arg1> <arg2>\n"
         << "********************************************************************************\n"
         << "where:\n"
         << "\t<arg1> is an input binary image\n"
         << "\t<arg2> is an output labeled image\n"
         << "example:\n\t" << fileName <<  " input.pgm output.pgm\n";
}
