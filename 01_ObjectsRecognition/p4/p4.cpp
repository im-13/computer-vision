/******************************************************************************************
 Title          : p4.cpp
 Author         : Ilona Michalowska
 Created on     : September 22, 2015
 Description    : The program recognizes objects from the database in the image.
 Usage          : ./p4 <arg1> <arg2> <arg3>
                  where:
                  <arg1> is an input labeled image
                  <arg2> is an input database
                  <arg3> is an output image
 Build with     :
 Comments       : The background in the input image is black (0) and the objects are labeled
                  with consecutive natural numbers as labels (1, 2, ...). 
                  The input objects database includes a line for each of the objects with
                  the following values: object label, row position of the center, column 
                  position of the center, orientation (angle in degrees between the axis of 
                  minimum inertia and the vertical axis), the minimum moment of inertia, the
                  maximum moment of inertia, and area, separated with blanks.
                  The output image displays positions and orientations of recognized objects
                  using a dot for the position and a short line segment originating from the
                  dot for the orientation.
 ******************************************************************************************/

#include "Image.h"
#include "Database.h"
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

    if (argc!=4) {
		showUsage(argv[0]);
		return 0;
	}
	Image *im;
	im=new Image;
	assert(im!=0);
    Database db, inputDb;
    
    if (readLabeledImage(im, argv[1], db)) {
		printf("Can't open file %s\n", argv[1]);
		return 0;
	}
    
    db.calculateProperties( );
    
    if (inputDb.readBasicFromTxtFile(argv[2])) {
        printf("Can't open file %s\n", argv[2]);
        return 0;
    }

    if (db.recognizeObjects(inputDb)) {
        printf("No object recognized\n");
        return 0;
        
    }
    
    addPositionAndOrientation(im, db, true);

	if (writeImage(im, argv[3])) {
		printf("Can't write to file %s\n", argv[3]);
		return 0;
	}
}

/******************************************************************************************
 * showUsage
 ******************************************************************************************/
void showUsage(string fileName) {
    cout << "********************************************************************************\n"
         << "Usage:\t" << fileName << " <arg1> <arg2> <arg3>\n"
         << "********************************************************************************\n"
         << "where:\n"
         << "\t<arg1> is an input labeled image\n"
         << "\t<arg2> is an input database\n"
         << "\t<arg3> is an output image\n"
         << "example:\n\t" << fileName <<  " input.pgm database.txt output.pgm\n";
}

