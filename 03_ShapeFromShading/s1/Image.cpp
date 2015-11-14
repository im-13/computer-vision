/******************************************************************************************
 Title          : Image.cpp
 Description    : Implementation file for Image.h header file.
 Modified by    : Ilona Michalowska
 Modified on    : November 12, 2015
 Comments       : Added functions
 
 Image(const Image &im, bool binaryCopy);
 int setSizeAndInitialize(int rows, int columns);
 int setRhoShift(int rs);
 int getRhoShift() const {return rhoShift;};
 int incrementPixel(int i, int j);
 int incrementPatchAroundPixel(int i, int j);
 int readAsBinaryImage(Image *im, const char *filename, int threshold);
 int readAndLabelBinaryImage(Image *im, const char *fname);
 int labelBinaryImage(Image *im);
 int readLabeledImage(Image *im, const char *filename, Database &db);
 int readAndThresholdImage(Image *im, const char *filename, int threshold);
 int thresholdImage(Image *im, int threshold);
 int thresholdAndMakeBinaryImage(Image *im, int threshold);
 int apply5x5GaussianFilter(Image *im);
 int scalePixelValues(Image *im, int maxPixelValue);
 int applyLaplacian(Image *im, Image *output);
 int applySobelOperator(Image *im, Image *output);
 int HoughTransform(Image *im, Image *output);
 int setRhoShiftForHoughImage(Image *original, Image *Hough);
 int findLocalMaxima(Image *Hough, HoughDatabase &db);
 int addPositionAndOrientation(Image *im, Database &db, bool recognizedOnly);
 int calculateSpherePropertiesAndSaveAsTxt(Image *im, const char *fname);
 int calculateLightSourcesDirectionsAndIntensities(const char *ifname, Image *input1, Image *input2, Image *input3, const char *ofname);
 void findBrightestPixel(Image *input, int iStart, int iEnd, int jStart, int jEnd, int (&bp)[3]);
 void calculateSphereSurfaceNormal(double xCenter, double yCenter, double radius, int bp[3], double (&n)[3]);
 int computeAndDrawNormals(const char *fname, Image *input1, Image *input2, Image *input3, int step, int threshold, Image *output);
 int invert3x3matrix(double (&s)[3][3]);
 void computeSurfaceNormal(double (&n)[3], int I[3], double S[3][3]);
 void drawNeedle(Image *im, double n[3], int i, int j);
 int computeAndDrawAlbedos(const char *fname, Image *input1, Image *input2, Image *input3, int threshold, Image *output);
 int computeAlbedo(int I[3], double S[3][3]);
 int drawLines(Image *im, HoughDatabase &db, Image *sobel);
 ******************************************************************************************/

/*
 -- Sample file --
 Source file "Image.C"
*/

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "Image.h"
#include "HoughDatabase.h"
#include "Database.h"
#include "DisjSets.h"

using namespace std;

/******************************************************************************************
 * default constructor
 ******************************************************************************************/
Image::Image() {
    /* initialize image class */
    /* everything is zero...  */
    Ncols=0;
    Nrows=0;
    Ncolors=0;
    rhoShift=0;
    image=NULL;
}

/******************************************************************************************
 * copy constructor
 ******************************************************************************************/
Image::Image(const Image &im) {
    /* initialize image class */
    /* Copy from im  */
  setSize(im.getNRows(), im.getNCols());
  setColors(im.getColors());
  setRhoShift(im.getRhoShift());
  int i,j;
  for (i=0; i<getNRows(); ++i)
    for (j=0; j<getNCols(); ++j){
      setPixel(i,j, im.getPixel(i,j));
    }
}

/******************************************************************************************
 * overloaded copy constructor
 ******************************************************************************************/
Image::Image(const Image &im, bool binaryCopy) {
    int i,j, numRows, numCols;
    numRows = im.getNRows( );
    numCols = im.getNCols( );
    
    /* initialize image class */
    setSize(numRows, numCols);
    setRhoShift(im.getRhoShift());
    
    /* Copy from im */
    if (binaryCopy) {
        setColors(1);
        for (i=0; i<numRows; ++i) {
            for (j=0; j<numCols; ++j){
                if (im.getPixel(i,j)==0) {
                    setPixel(i, j, 0);
                }
                else {
                    setPixel(i, j, 1);
                }
            }
        }
    }
    else {
        setColors(im.getColors());
        for (i=0; i<numRows; ++i) {
            for (j=0; j<numCols; ++j){
                setPixel(i,j, im.getPixel(i,j));
            }
        }
    }
}

/******************************************************************************************
 * destructor
 ******************************************************************************************/
Image::~Image() {
    int i;
    if (image) {
	for (i=0; i<Nrows; i++)
	    free( image[i] );
	free(image);
    }
}

/******************************************************************************************
 * setSize
 ******************************************************************************************/
int Image::setSize(int rows, int columns) {
    int i;
    if (rows<=0 || columns <=0){
	printf("setSize: rows, columns must be positive\n");
	return -2;
    }

    if ( (image=(int **)malloc(sizeof (int *) * rows))==NULL ){
	printf("setSize: can't allocate space\n");
	return -1;
     }

    for (i=0; i<rows; i++) {
	if ( (image[i]=(int *)malloc(sizeof(int) * columns))==NULL){
	    printf("setSize: can't allocate space\n");
	    return -1;
	}
    }

    Nrows=rows;
    Ncols=columns;

    return rows*columns;
}

/******************************************************************************************
 * setSizeAndInitialize
 ******************************************************************************************/
int Image::setSizeAndInitialize(int rows, int columns) {
    int i;
    if (rows<=0 || columns <=0){
        printf("setSize: rows, columns must be positive\n");
        return -2;
    }
    
    if ( (image=(int **)malloc(sizeof (int *) * rows))==NULL ){
        printf("setSize: can't allocate space\n");
        return -1;
    }
    
    for (i=0; i<rows; i++) {
        if ( (image[i]=(int *)malloc(sizeof(int) * columns))==NULL){
            printf("setSize: can't allocate space\n");
            return -1;
        }
    }
    
    Nrows=rows;
    Ncols=columns;
    
    // initialize all elements to 0:
    for (int r=0; r<rows; r++) {
        for (int c=0; c<columns; c++) {
            image[r][c] = 0;
        }
    }
    
    return rows*columns;
}

/******************************************************************************************
 * setRhoShift
 ******************************************************************************************/
int Image::setRhoShift(int rs) {
    rhoShift = rs;
    return rs;
}

/******************************************************************************************
 * setColors
 ******************************************************************************************/
int Image::setColors(int colors) {
  Ncolors=colors;
  return Ncolors;
}

/******************************************************************************************
 * setPixel
 ******************************************************************************************/
int Image::setPixel(int i, int j, int color) {
    if ( !image ) {
        printf("setPixel: write pixel to an empty image");
        return 0;
    }
    
    if ( i<0 || i>Nrows || j<0 || j>Ncols ){
        //  error_msg("Image::setPixel -> Out of boundaries\n");
        return -1;
    }
    image[i][j]=color;
    return color;
}

/******************************************************************************************
 * getPixel
 ******************************************************************************************/
int Image::getPixel(int i, int j) const {
   if ( !image ) {
       printf("getPixel: read pixel from an empty image\n");
       return -1;
     }
  if (i<0 || i>Nrows || j<0 || j>Ncols) {
        //  error_msg("getPixel: out of image");
        return -1;
       }
       else
          return image[i][j];
}

/******************************************************************************************
 * incrementPixel
 ******************************************************************************************/
int Image::incrementPixel(int i, int j) {
    if ( !image ) {
        printf("setPixel: write pixel to an empty image");
        return 0;
    }
    
    if ( i<0 || i>Nrows || j<0 || j>Ncols ) {
        //  error_msg("Image::setPixel -> Out of boundaries\n");
        return -1;
    }
    
    return ++image[i][j];
}
/******************************************************************************************
 * incrementPatchAroundPixel
 ******************************************************************************************/
int Image::incrementPatchAroundPixel(int i, int j) {
    if ( !image ) {
        printf("setPixel: write pixel to an empty image");
        return 0;
    }
    
    if ( i<0 || i>Nrows || j<0 || j>Ncols ){
        //  error_msg("Image::setPixel -> Out of boundaries\n");
        return -1;
    }
    
    int maxPixVal = 0;
    int curPixVal = 0;
    
    for (int k=i-1; k <=i+1; k++) {
        for (int l=j-1; l <=j+1; l++) {
            if (k>=0 && k<Nrows && l>=0 && l<Ncols) {
                curPixVal = image[k][l]++;
                if (curPixVal > maxPixVal) {
                    maxPixVal = curPixVal;
                }
            }
        }
    }
    return maxPixVal;
}
