/******************************************************************************************
 Title          : Image.h
 Description    : Header file for Image class.
 Modified by    : Ilona Michalowska
 Modified on    : October 22, 2015
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
                  int drawLines(Image *im, HoughDatabase &db, Image *sobel);
 ******************************************************************************************/

/*
 -- Sample file --
 Header file "Image.h"
*/

/******************************************************************************************
 * Definition of class Image
 ******************************************************************************************/

#ifndef _IMAGE
#define _IMAGE

#include "HoughDatabase.h"
#include "Database.h"
#include "DisjSets.h"

class Image {

private:
    
    int Nrows; /* number of rows */
    int Ncols; /* number of columns */
    int Ncolors; /* number of gray level colors */
    int rhoShift; /* rho shift for Hough image */
    int **image;

public:
    
    /**
     * Default constructor.
     */
    Image();
    
    /**
     * Copy constructor.
     */
    Image(const Image &im);

    /**
     * Overloaded copy constructor; makes a binary copy of im.
     */
    Image(const Image &im, bool binaryCopy);
    
    /**
     * Destructor.
     */
    ~Image();

/**
 * MEMBER FUNCTIONS
 */

    /**
     * Sets the size of the image to the given height (# of rows) and width (# of columns);
     * allocates memory for rows x columns image;
     * returns: -2 if rows or columns <= 0, -1 if cannot allocate space, rows*columns if success.
     */
    int setSize(int rows, int columns);

    /**
     * Sets the size of the image to the given height (# of rows) and width (# of columns);
     * allocates memory for rows x columns image;
     * initializes all elements to 0;
     * returns: -2 if rows or columns <= 0, -1 if cannot allocate space, rows*columns if success.
     */
    int setSizeAndInitialize(int rows, int columns);

    /**
     * Returns the number of columns in the image.
     */
    int getNCols() const {return Ncols;};

    /**
     * Returns the number of rows in the image.
     */
    int getNRows() const {return Nrows;};

    /**
     * Sets and returns rho shift value.
     */
    int setRhoShift(int rs);
    
    /**
     * Returns rho shift value.
     */
    int getRhoShift() const {return rhoShift;};

    /**
     * Sets and returns number of gray-level colors in the image (not counting 0).
     */
    int setColors(int colors);

    /**
     * Returns the number of gray-level colors in the image.
     */
    int getColors() const { return Ncolors; };

    /**
     * Sets the pixel in the image at row i and column j to a particular color;
     * returns pixel i,j or -1 if error.
     */
    int setPixel(int i, int j, int color);
    
    /**
     * Return color of pixel at row i and column j or -1 if error.
     */
    int getPixel(int i, int j) const;

    /**
     * Increments value of pixel at row i and column j by 1;
     * returns value of incremented pixel or -1 if error.
     */
    int incrementPixel(int i, int j);

    /**
     * Increments values of pixels around pixel at row i and column j by 1;
     * returns maximum value of a pixel in the patch or -1 if error.
     */
    int incrementPatchAroundPixel(int i, int j);
    
};

/**
 * FUNCTIONS FOR READ-WRITE OF PGM IMAGES
 */

/**
 * Reads image from fname;
 * returns 0 if OK or -1 if something goes wrong.
 */
int readImage(Image *im, const char *fname);

/**
 * Reads image from fname, thresholds and saves as binary image in Image object im;
 * returns 0 if OK or -1 if something goes wrong. 
 */
int readAsBinaryImage(Image *im, const char *fname, int threshold);

/**
 * Reads binary image from fname, saves labeled binary image in Image object im;
 * returns 0 if OK or -1 if something goes wrong.
 */
int readAndLabelBinaryImage(Image *im, const char *fname);

/**
 * Labels binary Image object im.
 */
int labelBinaryImage(Image *im);

/**
 * Reads labeled image from fname, saves objects' info in db Database;
 * returns 0 if OK or -1 if something goes wrong. 
 */
int readLabeledImage(Image *im, const char *filename, Database &db);

/**
 * Reads image from fname, tresholds, and saves as grey-level image in Image object im;
 * returns 0 if OK or -1 if something goes wrong.
 */
int readAndThresholdImage(Image *im, const char *filename, int threshold);

/**
 * Thresholds object im.
 */
int thresholdImage(Image *im, int threshold);

/**
 * Thresholds Image object im and makes it binary.
 */
int thresholdAndMakeBinaryImage(Image *im, int threshold);

/**
 * Filters im using Gaussian mask 1/16 [1, 4, 6, 4, 1].
 */
int apply5x5GaussianFilter(Image *im);

/**
 * Scales pixel values if there are values greater than 255;
 * maxPixelValue is the max value in image im.
 */
int scalePixelValues(Image *im, int maxPixelValue);

/**
 * Applies Laplacian operator to image im, saves result in output.
 */
int applyLaplacian(Image *im, Image *output);

/**
 * Applies Sobel operator to image im, saves result in output.
 */
int applySobelOperator(Image *im, Image *output);

/**
 * Applies Hough transform to image im, saves result in output;
 * numOfRhoUnits = int(sqrt(pow(nRows,2)+pow(nCols,2)) + 0.5) pixels;
 * rho space equals 3*numOfRhoUnits
 * rho values are shifted by numOfRhoUnits to acomodate negative values;
 * numOtThetaUnits = 180 * 5 = PI = 180 degrees.
 */
int HoughTransform(Image *im, Image *output);

/**
 * Sets rho shift value for Hough image of im.
 */
int setRhoShiftForHoughImage(Image *im, Image *Hough);

/**
 * Finds "areas of brightness" in grey-level thresholded Hough image, saves results in db.
 */
int findLocalMaxima(Image *Hough, HoughDatabase &db);

/**
 * Draws objects' positions and orientations in the image in black (0).
 */
int addPositionAndOrientation(Image *im, Database &db, bool recognizedOnly);

/**
 * Draws detected lines in image im.
 */
int drawLines(Image *im, HoughDatabase &db);

/**
 * Draws detected line segments in image im.
 */
int drawLines(Image *im, HoughDatabase &db, Image *sobel);

/**
 * Writes image im into file filename;
 * returns 0 if OK or -1 if something goes wrong.
 */
int writeImage(const Image *im, const char *filename);


/*
function for drawing a line
*/

/**
 * Returns rho shift value.
 */
int
line(Image *&im, int x0, int y0, int x1, int y1, int color,Image *&sobel);
/**
 * Returns rho shift value.
 */
int
line(Image *&im, int x0, int y0, int x1, int y1, int color);

#endif
