/******************************************************************************************
 Title          : Pgm.cpp
 Author         : Anton Nikolaev
 Created on     : 1995
 Description    : Implementation file for Image.h header file.
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

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <vector>
#include <iostream>
#include <cmath>
#include <cassert>
#include "Image.h"
#include "DisjSets.h"
#include "HoughDatabase.h"
#include "Database.h"

using namespace std;

/******************************************************************************************
 * readImage
 ******************************************************************************************/
int readImage(Image *im, const char *fname) {
  FILE *input;
  char line[1024];
  int nCols,nRows;
  int levels;
  int i, j;

  /* open it */
  if (!fname || (input=fopen(fname,"r"))==0){
    printf("readImage: Cannot open file\n");
    return-1;
  }

  /* check for the right "magic number" */
  if (
        fread(line,1,3,input)!=3
      ||strncmp(line,"P5\n",3)
     )
  {
    fclose(input);
    printf("readImage: Expected .pgm file\n");
    return -1;
  }

  /* skip the comments */
  do
    fgets(line,sizeof line,input);
  while(*line=='#');

  /* read the width and height */
  sscanf(line,"%d %d\n",&nCols,&nRows);
  im->setSize(nRows, nCols);

  /* read # of gray levels */
  fgets(line,sizeof line,input);
  sscanf(line,"%d\n",&levels);
  im->setColors(levels);

  /* read pixel row by row */
  for(i=0;i<nRows;i++)
  {
    for(j=0;j<nCols;j++)
    {
      int byte=fgetc(input);

      if (byte==EOF) /* short file */
      {
        fclose(input);
        printf("readImage: short file\n");
        return -1;
      }
      else
        im->setPixel(i,j,byte);
    }
  }

  /* close the file */
  fclose(input);
  return 0; /* OK */
}

/******************************************************************************************
 * readAsBinaryImage
 ******************************************************************************************/
int readAsBinaryImage(Image *im, const char *fname, int threshold) {
    FILE *input;
    char line[1024];
    int nCols,nRows;
    int i, j;
    
    /* open it */
    if (!fname || (input=fopen(fname,"r"))==0){
        printf("readImage: Cannot open file\n");
        return-1;
    }
    
    /* check for the right "magic number" */
    if (fread(line,1,3,input)!=3 || strncmp(line,"P5\n",3)) {
        fclose(input);
        printf("readImage: Expected .pgm file\n");
        return -1;
    }
    
    /* skip the comments */
    do
        fgets(line,sizeof line,input);
    while(*line=='#');
    
    /* read the width and height */
    sscanf(line,"%d %d\n",&nCols,&nRows);
    im->setSize(nRows, nCols);
    
    /* read line with # of gray levels */
    fgets(line,sizeof line,input);
    im->setColors(1); /* a binary image can be saved as a PGM file with 1 as the number of colors in the header */
    
    /* read pixels row by row */
    for(i=0; i<nRows; i++) {
        for(j=0; j<nCols; j++) {
            int byte=fgetc(input);
            
            if (byte==EOF) /* short file */ {
                fclose(input);
                printf("readImage: short file\n");
                return -1;
            }
            else {
                /* 0 is black, 255 is white */
                if (byte<=threshold) {
                    im->setPixel(i,j,0);
                }
                else {
                    im->setPixel(i,j,1);
                }
            }
        }
    }
    
    /* close the file */
    fclose(input);
    return 0; /* OK */
}

/******************************************************************************************
 * readAndLabelBinaryImage
 ******************************************************************************************/
int readAndLabelBinaryImage(Image *im, const char *fname) {
    FILE *input;
    char line[1024];
    int nCols,nRows;
    int levels;
    int i, j;
    
    /* open it */
    if (!fname || (input=fopen(fname,"r"))==0) {
        printf("readImage: Cannot open file\n");
        return -1;
    }
    
    /* check for the right "magic number" */
    if (fread(line,1,3,input)!=3 || strncmp(line,"P5\n",3)) {
        fclose(input);
        printf("readImage: Expected .pgm file\n");
        return -1;
    }
    
    /* skip the comments */
    do {
        fgets(line,sizeof line,input);
    }
    while (*line=='#');
    
    /* read the width and height */
    sscanf(line,"%d %d\n",&nCols,&nRows);
    im->setSize(nRows, nCols);
    
    /* read line with # of gray levels, check if binary image  */
    fgets(line,sizeof line,input);
    sscanf(line,"%d\n",&levels);
    if (levels!=1) {
        fclose(input);
        printf("readImage: Expected binary .pgm file\n");
        return -1;
    }
    
    /* FIRST RUN */
    
    /* read pixel row by row, label starting from 1 */
    int nextLabel = 0;
    DisjSets labels;
    
    for(i=0; i<nRows; i++) {
        
        for(j=0; j<nCols; j++) {
            
            int byte = fgetc(input);
            
            if (byte==EOF) /* short file */ {
                fclose(input);
                printf("readImage: short file\n");
                return -1;
            }
            else {
                /* 0 is black, 255 is white */
                if (byte!=0) {
                    int NW, N, W;
                    
                    /* most pixels--except for top row and left column */
                    if (i!=0 && j!=0) { /* check pixels in the following order: NW, N, W */
                        
                        NW = im->getPixel(i-1,j-1);
                        N = im->getPixel(i-1,j);
                        W = im->getPixel(i,j-1);
                        
                        if (NW!=0) {
                            im->setPixel(i,j,NW);
                            if (N!=0 && W==0 && N!=NW) {
                                labels.unionSets(NW,N);
                            }
                            if (W!=0 && N==0 && W!=NW) {
                                labels.unionSets(NW,W);
                            }
                            if (W!=0 && N!=0 && W!=N) {
                                labels.unionSets(N,W);
                            }
                        }
                        else {
                            if (N!=0 && W==0) {
                                im->setPixel(i,j,N);
                            }
                            else if (N==0 && W!=0) {
                                im->setPixel(i,j,W);
                            }
                            else if (N==0 && W==0) {
                                im->setPixel(i,j,++nextLabel);
                                labels.addElement( );
                            }
                            else if (N!=0 && W!=0) {
                                if (N==W) {
                                    im->setPixel(i,j,N);
                                }
                                else {
                                    labels.unionSets(N,W);
                                    im->setPixel(i,j,N);
                                }
                            }
                        }
                    }
                    /* top left corner */
                    if (i==0 && j==0) {
                        im->setPixel(i,j,++nextLabel);
                        labels.addElement( );
                    }
                    /* top row */
                    if (i==0 && j!=0) {
                        W = im->getPixel(i,j-1);
                        if (W!=0) {
                            im->setPixel(i,j,W);
                        }
                        else {
                            im->setPixel(i,j,++nextLabel);
                            labels.addElement( );
                        }
                    }
                    /* left column */
                    if (i!=0 && j==0)  {
                        N = im->getPixel(i-1,j);
                        if (N!=0) {
                            im->setPixel(i,j,N);
                        }
                        else {
                            im->setPixel(i,j,++nextLabel);
                            labels.addElement( );
                        }
                    }
                }
            }
        }
    }
    
    /* save # levels (num of objects) */
    levels = labels.getNumberOfLevels( );
    im->setColors(levels);
    printf("Number of objects: %d\n", levels);
    
    /* SECOND RUN */
    
    /* create a finalLabels vector with labels of all sets */
    vector<int> finalLabels;
    finalLabels.push_back(-1);
    vector<int> listOfLevels = labels.getLevels( );
    int label, numOfLabels = labels.getNumberOfLabels( );
    
    for (i=1; i<=numOfLabels; i++) {
        label = labels.find(i);
        for (j=0; j < listOfLevels.size(); j++) {
            if (label==listOfLevels[j]) {
                label = j+1;
            }
        }
        finalLabels.push_back(label);
    }
    
    /* relabel the image */
    int l;
    for(i=0; i<nRows; i++) {
        for(j=0; j<nCols; j++) {
            l = im->getPixel(i,j);
            if (l>0 && l<=finalLabels.size( )) {
                im->setPixel(i,j,finalLabels[l]);
            }
        }
    }
    
    /* close the file */
    fclose(input);
    return 0; /* OK */
}

/******************************************************************************************
 * labelBinaryImage
 ******************************************************************************************/
int labelBinaryImage(Image *im) {
    int nRows, nCols, levels;
    int i, j;
    
    nRows = im->getNRows( );
    nCols = im->getNCols( );
    
    /* FIRST RUN */
    
    /* read pixel row by row, label starting from 1 */
    int nextLabel = 0;
    DisjSets labels;
    
    for(i=0; i<nRows; i++) {
        
        for(j=0; j<nCols; j++) {

            /* 0 is black, 255 is white */
            if (im->getPixel(i, j) != 0) {
                int NW, N, W;
                    
                /* most pixels--except for top row and left column */
                if (i!=0 && j!=0) { /* check pixels in the following order: NW, N, W */
                        
                    NW = im->getPixel(i-1,j-1);
                    N = im->getPixel(i-1,j);
                    W = im->getPixel(i,j-1);
                        
                    if (NW!=0) {
                        im->setPixel(i,j,NW);
                        if (N!=0 && W==0 && N!=NW) {
                            labels.unionSets(NW,N);
                        }
                        if (W!=0 && N==0 && W!=NW) {
                            labels.unionSets(NW,W);
                        }
                        if (W!=0 && N!=0 && W!=N) {
                            labels.unionSets(N,W);
                        }
                    }
                    else {
                        if (N!=0 && W==0) {
                            im->setPixel(i,j,N);
                        }
                        else if (N==0 && W!=0) {
                            im->setPixel(i,j,W);
                        }
                        else if (N==0 && W==0) {
                            im->setPixel(i,j,++nextLabel);
                            labels.addElement( );
                        }
                        else if (N!=0 && W!=0) {
                            if (N==W) {
                                im->setPixel(i,j,N);
                            }
                            else {
                                labels.unionSets(N,W);
                                im->setPixel(i,j,N);
                            }
                        }
                    }
                }
                /* top left corner */
                if (i==0 && j==0) {
                    im->setPixel(i,j,++nextLabel);
                    labels.addElement( );
                }
                /* top row */
                if (i==0 && j!=0) {
                    W = im->getPixel(i,j-1);
                    if (W!=0) {
                        im->setPixel(i,j,W);
                    }
                    else {
                        im->setPixel(i,j,++nextLabel);
                        labels.addElement( );
                    }
                }
                /* left column */
                if (i!=0 && j==0)  {
                    N = im->getPixel(i-1,j);
                    if (N!=0) {
                        im->setPixel(i,j,N);
                    }
                    else {
                        im->setPixel(i,j,++nextLabel);
                        labels.addElement( );
                    }
                }
            }
        }
    }
    
    /* save # levels (num of objects) */
    levels = labels.getNumberOfLevels( );
    im->setColors(levels);
    //printf("Number of objects: %d\n", levels);
    
    /* SECOND RUN */
    
    /* create a finalLabels vector with labels of all sets */
    vector<int> finalLabels;
    finalLabels.push_back(-1);
    vector<int> listOfLevels = labels.getLevels( );
    int label, numOfLabels = labels.getNumberOfLabels( );
 
    for (i=1; i<=numOfLabels; i++) {
        label = labels.find(i);
        for (j=0; j < listOfLevels.size(); j++) {
            if (label==listOfLevels[j]) {
                label = j+1;
            }
        }
        finalLabels.push_back(label);
    }

    /* relabel the image */
    int l;
    for(i=0; i<nRows; i++) {
        for(j=0; j<nCols; j++) {
            l = im->getPixel(i,j);
            if (l>0 && l<=finalLabels.size( )) {
                im->setPixel(i,j,finalLabels[l]);
            }
        }
    }

    return 0; /* OK */
}

/******************************************************************************************
 * readLabeledImage
 ******************************************************************************************/
int readLabeledImage(Image *im, const char *fname, Database &db) {
    FILE *input;
    char line[1024];
    int nCols,nRows;
    int levels;
    int i, j;
    
    /* open it */
    if (!fname || (input=fopen(fname,"r"))==0) {
        printf("readImage: Cannot open file\n");
        return -1;
    }
    
    /* check for the right "magic number" */
    if ( fread(line,1,3,input)!=3 ||strncmp(line,"P5\n",3)) {
        fclose(input);
        printf("readImage: Expected .pgm file\n");
        return -1;
    }
    
    /* skip the comments */
    do {
        fgets(line,sizeof line,input);
    }
    while (*line=='#');
    
    /* read the width and height */
    sscanf(line,"%d %d\n",&nCols,&nRows);
    im->setSize(nRows, nCols);
    
    /* read # of gray levels, i.e. the number of objects in the image */
    fgets(line,sizeof line,input);
    sscanf(line,"%d\n",&levels);
    im->setColors(levels);
    db.initializeRecords(levels);
    
    /* read pixel row by row */
    for(i=0; i<nRows; i++)
    {
        for(j=0; j<nCols; j++)
        {
            int byte=fgetc(input);
            
            if (byte==EOF) /* short file */
            {
                fclose(input);
                printf("readImage: short file\n");
                return -1;
            }
            else {
                im->setPixel(i,j,byte);
                if (byte!=0) {
                    db.updateSums(byte, i, j);
                    
                }
            }
        }
    }
    
    /* close the file */
    fclose(input);
    return 0; /* OK */
}

/******************************************************************************************
 * readAndThresholdImage
 ******************************************************************************************/
int readAndThresholdImage(Image *im, const char *fname, int threshold) {
    FILE *input;
    char line[1024];
    int nCols,nRows;
    int i, j;
    
    /* open it */
    if (!fname || (input=fopen(fname,"r"))==0) {
        printf("readImage: Cannot open file\n");
        return-1;
    }
    
    /* check for the right "magic number" */
    if (
        fread(line,1,3,input)!=3
        ||strncmp(line,"P5\n",3)
        )
    {
        fclose(input);
        printf("readImage: Expected .pgm file\n");
        return -1;
    }
    
    /* skip the comments */
    do
        fgets(line,sizeof line,input);
    while(*line=='#');
    
    /* read the width and height */
    sscanf(line,"%d %d\n",&nCols,&nRows);
    im->setSize(nRows, nCols);
    
    /* read line with # of gray levels */
    fgets(line,sizeof line,input);
    im->setColors(255);
    
    /* read pixels row by row */
    for(i=0; i<nRows; i++) {
        for(j=0; j<nCols; j++) {
            int byte=fgetc(input);
            
            if (byte==EOF) /* short file */ {
                fclose(input);
                printf("readImage: short file\n");
                return -1;
            }
            else {
                /* 0 is black, 255 is white */
                if (byte<=threshold) {
                    im->setPixel(i,j,0);
                }
                else {
                    im->setPixel(i,j,byte);
                }
            }
        }
    }
    
    /* close the file */
    fclose(input);
    return 0; /* OK */
}

/******************************************************************************************
 * thresholdImage
 ******************************************************************************************/
int thresholdImage(Image *im, int threshold) {
    int nCols = im->getNCols(), nRows = im->getNRows();
    int i, j;
    
    /* read pixels row by row */
    for(i=0; i<nRows; i++) {
        for(j=0; j<nCols; j++) {
            int pixVal = im->getPixel(i, j);

            /* 0 is black, 255 is white */
            if (pixVal<=threshold) {
                im->setPixel(i,j,0);
            }
            else {
                im->setPixel(i,j,pixVal);
            }
            
        }
    }

    return 0; /* OK */
}

/******************************************************************************************
 * thresholdAndMakeBinaryImage
 ******************************************************************************************/
int thresholdAndMakeBinaryImage(Image *im, int threshold) {
    int nCols = im->getNCols(), nRows = im->getNRows();
    int i, j;
    
    /* read pixels row by row */
    for(i=0; i<nRows; i++) {
        for(j=0; j<nCols; j++) {
            int pixVal = im->getPixel(i, j);
            
            /* 0 is black, 255 is white */
            if (pixVal<=threshold) {
                im->setPixel(i,j,0);
            }
            else {
                im->setPixel(i,j,1);
            }
            
        }
    }
    
    im->setColors(1);
    
    return 0; /* OK */
}

/******************************************************************************************
 * apply5x5GaussianFilter
 ******************************************************************************************/
int apply5x5GaussianFilter(Image *im) {
    // Gaussian mask: 1/16 [1, 4, 6, 4, 1]
    
    int nRows = im->getNRows();
    int nCols = im->getNCols();
    int Minus2 = 0;
    int Minus1 = 0;
    int Current = 0;
    int Plus1 = 0;
    int Plus2 = 0;
    int newCurrent = 0;
    
    Image *temp;
    temp = new Image;
    assert(temp != 0);
    temp->setSize(im->getNRows(), im->getNCols());
    temp->setColors(im->getColors());
    
    // convolve Gaussian mask with rows of im
    for(int i=0; i<nRows; i++) {
        for(int j=0; j<nCols; j++) {
            Current = im->getPixel(i, j);
            if (j == 0) {
                Minus2 = 0;
                Minus1 = 0;
                Plus1 = im->getPixel(i, 1);
                Plus2 = im->getPixel(i, 2);
            }
            newCurrent = int((Minus2 + Minus1*4 + Current*6 + Plus1*4 + Plus2)/16.0 + 0.5);
            temp->setPixel(i, j, newCurrent);
            
            // reassign pixel values
            Minus2 = Minus1;
            Minus1 = Current;
            Plus1 = Plus2;
            if (j >= nCols-3) {
                Plus2 = 0;
            }
            else {
                Plus2 = im->getPixel(i, j+3);
            }
        }
    }
    
    // convolve Gaussian mask with columns of temp
    for(int j=0; j<nCols; j++) {
        for(int i=0; i<nRows; i++) {
            Current = temp->getPixel(i, j);
            if (j == 0) {
                Minus2 = 0;
                Minus1 = 0;
                Plus1 = temp->getPixel(1, j);
                Plus2 = temp->getPixel(2, j);
            }
            newCurrent = int((Minus2 + Minus1*4 + Current*6 + Plus1*4 + Plus2)/16.0 + 0.5);
            im->setPixel(i, j, newCurrent);
            
            // reassign pixel values
            Minus2 = Minus1;
            Minus1 = Current;
            Plus1 = Plus2;
            if (i >= nRows-3) {
                Plus2 = 0;
            }
            else {
                Plus2 = temp->getPixel(i+3, j);
            }
        }
    }
    return 0;
}

/******************************************************************************************
 * scalePixelValues
 ******************************************************************************************/
int scalePixelValues(Image *im, int maxPixelValue) {
    int nRows = im->getNRows();
    int nCols = im->getNCols();
    int curPixVal = 0;
    int newPixVal = 0;
    
    for(int i=0; i<nRows; i++) {
        for(int j=0; j<nCols; j++) {
            curPixVal = im->getPixel(i, j);
            newPixVal = int(curPixVal*255.0/maxPixelValue + 0.5);
            im->setPixel(i, j, newPixVal);
        }
    }
    return 0;
}

/******************************************************************************************
 * applyLaplacian
 ******************************************************************************************/
int applyLaplacian(Image *im, Image *output) {
    // Laplacial stencil:
    // |  0  1  0  |
    // |  1 -4  1  |
    // |  0  1  0  |
    
    int nRows = im->getNRows();
    int nCols = im->getNCols();
    output->setSize(im->getNRows(), im->getNCols());
    output->setColors(im->getColors());
    
    // for scaling the output
    int maxPixelValue = 0;
    
    for(int i=0; i<nRows; i++) {
        for(int j=0; j<nCols; j++) {
            // pad outermost ring of pixels with 0's
            if (i==0 || i==nRows-1 || j==0 || j==nCols-1) {
                output->setPixel(i, j, 0);
            }
            else {
                int current = im->getPixel(i, j);
                int N = im->getPixel(i-1, j);
                int E = im->getPixel(i, j+1);
                int S = im->getPixel(i+1, j);
                int W = im->getPixel(i, j-1);
                int newCurrent = int(4*((N+E+S+W)/4.0 - current)+0.5);
                output->setPixel(i, j, newCurrent);
                
                // for scaling the output
                if (newCurrent > maxPixelValue) {
                    maxPixelValue = newCurrent;
                }
            }
        }
    }
    
    scalePixelValues(output, maxPixelValue);
    
    return 0;
}
/******************************************************************************************
 * applySobelOperator
 ******************************************************************************************/
int applySobelOperator(Image *im, Image *output) {
    int nRows = im->getNRows();
    int nCols = im->getNCols();
    output->setSize(im->getNRows(), im->getNCols());
    output->setColors(im->getColors());
    int NW = 0;
    int N = 0;
    int NE = 0;
    int E = 0;
    int SE = 0;
    int S = 0;
    int SW = 0;
    int W = 0;
    int newCurrent = 0;
    int delta1 = 0;
    int delta2 = 0;
    
    // for scaling the output
    int maxPixelValue = 0;
    
    for(int i=0; i<nRows; i++) {
        for(int j=0; j<nCols; j++) {
            // pad outermost ring of pixels with 0's
            if (i==0 || i==nRows-1 || j==0 || j==nCols-1) {
                output->setPixel(i, j, 0);
            }
            else {
                if (i==1 & j==1) {
                    NW = im->getPixel(i-1, j-1);
                    N = im->getPixel(i-1, j);
                    NE = im->getPixel(i-1, j+1);
                    E = im->getPixel(i, j+1);
                    SE = im->getPixel(i+1, j+1);
                    S = im->getPixel(i+1, j);
                    SW = im->getPixel(i+1, j-1);
                    W = im->getPixel(i, j-1);
                }
                delta1 = -NW + NE + 2*E + SE - SW -2*W;
                delta2 = NE + 2*N + NE - SE -2*S - SW;
                newCurrent = int(sqrt(pow(delta1,2) + pow(delta2,2))+0.5);
                output->setPixel(i, j, newCurrent);
                
                // for scaling the output
                if (newCurrent > maxPixelValue) {
                    maxPixelValue = newCurrent;
                }
                
                // reassign pixel values
                NW = N;
                W = im->getPixel(i, j);
                SW = S;
                N = NE;
                S = SE;
                NE = im->getPixel(i-1, j+2);
                E = im->getPixel(i, j+2);
                SE = im->getPixel(i+1, j+2);
            }
        }
    }
    
    scalePixelValues(output, maxPixelValue);
    
    return 0;
}

/******************************************************************************************
 * HoughTransform
 ******************************************************************************************/
int HoughTransform(Image *im, Image *output) {
    int nRows = im->getNRows();
    int nCols = im->getNCols();
    int i,j,t;
    
    int numOfRhoUnits = int(sqrt(pow(nRows,2)+pow(nCols,2)) + 0.5);
    int numOtThetaUnits = 180 * 5;
    
    // set size of Hough image output and initialize all pizels to 0
    output->setSizeAndInitialize(3*numOfRhoUnits, numOtThetaUnits);
    output->setColors(255);
    output->setRhoShift(numOfRhoUnits);

    int maxPixelValue = 0;
    int possibleMaxPixelValue = 0;
    
    // iterate through image im
    for (i=0; i<nRows; i++) {
        for (j=0; j<nCols; j++) {
            if (im->getPixel(i, j) != 0) {

                for (t=0; t<numOtThetaUnits; t++) {
                    int rho = int(i * cos(t*M_PI/numOtThetaUnits) + j * sin(t*M_PI/numOtThetaUnits) + 0.5) + numOfRhoUnits;
                    if (rho >= 3 * numOfRhoUnits || rho < 0) {
                        continue;
                    }
                    
                    //possibleMaxPixelValue = output->incrementPatchAroundPixel(rho, t);
                    possibleMaxPixelValue = output->incrementPixel(rho, t);
                    if (possibleMaxPixelValue > maxPixelValue) {
                        maxPixelValue = possibleMaxPixelValue;
                    }
                }
            }
        }
    }
    
    scalePixelValues(output, maxPixelValue);

    return 0;
}

/******************************************************************************************
 * setRhoShiftForHoughImage
 ******************************************************************************************/
int setRhoShiftForHoughImage(Image *im, Image *Hough) {
    int numOfRhoUnits = int(sqrt(pow(im->getNRows(),2)+pow(im->getNCols(),2)) + 0.5);
    Hough->setRhoShift(numOfRhoUnits);
    return 0;
}

/******************************************************************************************
 * findLocalMaxima
 ******************************************************************************************/
int findLocalMaxima(Image *Hough, HoughDatabase &db) {
    // 1. make a copy of hough, make it binary, label
    Image *temp;
    temp = new Image(*Hough, true);
    // writeImage(temp, "Hough_T_B.pgm");
    assert(temp != 0);
    
    labelBinaryImage(temp);
    //writeImage(temp, "Hough_T_B_L.pgm");

    int numOfColors = temp->getColors();
    int nRows = temp->getNRows();
    int nCols = temp->getNCols();
    int objLabel, pixVal;
    
    db.initializeRecords(numOfColors);

    for(int i=0; i<nRows; i++) {
        for(int j=0; j<nCols; j++) {
            objLabel = temp->getPixel(i, j);
            if (objLabel != 0) {
                pixVal = Hough->getPixel(i, j);
                db.updateSums(objLabel, i, j, pixVal);
            }
        }
    }
    
    db.calculateRhoTheta(Hough->getRhoShift());
    //db.printRecords();
    db.optimize();
    //cout << "AFTER OPTIMIZATION" << endl;
    // db.printRecords( );
 
    return 0;
}

/******************************************************************************************
 * addPositionAndOrientation
 ******************************************************************************************/
int addPositionAndOrientation(Image *im, Database &db, bool recognizedOnly) {
    vector<Database::Record> objects = db.getRecords( );
    int numOfObjects = int(objects.size( ));
    int x0, y0, x1, y1;
    double theta;
    
    for (int i=0; i<numOfObjects; i++) {
        
        if (recognizedOnly && !objects[i].recognized) {
            continue;
        }
        
        x0 = objects[i].rowCenter;
        y0 = objects[i].colCenter;
        theta = objects[i].theta;
        x1 = int (x0 + 20 * cos(theta) + 0.5);
        y1 = int (y0 + 20 * sin(theta) + 0.5);
        
        //draw line (orientation)
        line(im, x0, y0, x1, y1, 0);
        
        // draw point (position)
        for (int k=x0-1; k<=x0+1; k++) {
            for (int j=y0-1; j<=y0+1; j++) {
                im->setPixel(k, j, 0);
            }
        }
    }
    return 0;
}

/******************************************************************************************
 * drawLines
 ******************************************************************************************/
int drawLines(Image *im, HoughDatabase &db) {
    vector<HoughDatabase::Record> objects = db.getRecords( );
    int numOfObjects = int(objects.size( ));
    int x ,y, xMax, yMax;
    double rho, theta, sinTheta, cosTheta;
    
    xMax = im->getNRows() - 1;
    yMax = im->getNCols() - 1;
    
    for (int i=0; i<numOfObjects; i++) {
        rho = objects[i].rho;
        theta = objects[i].theta * M_PI / 180;
        sinTheta = sin(theta);
        cosTheta = cos(theta);
        vector<int> coords;
        
        // case 1
        x = 0;
        y = int(rho / sinTheta + 0.5);
        if ( x>=0 && x<=xMax && y>=0 && y<=yMax) {
            coords.push_back(x);
            coords.push_back(y);
        }
        
        // case 2
        x = xMax;
        y = int((rho - x * cosTheta) / sinTheta + 0.5);
        if ( x>=0 && x<=xMax && y>=0 && y<=yMax) {
            if (coords.size() == 2) {
                if (x!=coords[0] && y!=coords[1]) {
                    coords.push_back(x);
                    coords.push_back(y);
                }
            }
            else {
                coords.push_back(x);
                coords.push_back(y);
            }
        }
        
        // case 3
        y = 0;
        x = int(rho / cosTheta + 0.5);
        if ( x>=0 && x<=xMax && y>=0 && y<=yMax) {
            if (coords.size() == 2) {
                if (x!=coords[0] && y!=coords[1]) {
                    coords.push_back(x);
                    coords.push_back(y);
                }
            }
            else {
                coords.push_back(x);
                coords.push_back(y);
            }
        }
        
        // case 4
        y = yMax;
        x = int((rho - y * sinTheta) / cosTheta + 0.5);
        if ( x>=0 && x<=xMax && y>=0 && y<=yMax) {
            if (coords.size() == 2) {
                if (x!=coords[0] && y!=coords[1]) {
                    coords.push_back(x);
                    coords.push_back(y);
                }
            }
            else {
                coords.push_back(x);
                coords.push_back(y);
            }
        }
        
        // draw line
        if (coords.size() >= 4) {
            line(im, coords[0], coords[1], coords[2], coords[3], 255);
        }
        
    }
    return 0;
}

/******************************************************************************************
 * drawLines - overloaded to draw only edges
 ******************************************************************************************/
int drawLines(Image *im, HoughDatabase &db, Image *sobel) {
    vector<HoughDatabase::Record> objects = db.getRecords( );
    int numOfObjects = int(objects.size( ));
    int x ,y, xMax, yMax;
    double rho, theta, sinTheta, cosTheta;
    
    xMax = im->getNRows() - 1;
    yMax = im->getNCols() - 1;
    
    for (int i=0; i<numOfObjects; i++) {
        rho = objects[i].rho;
        theta = objects[i].theta * M_PI / 180;
        sinTheta = sin(theta);
        cosTheta = cos(theta);
        vector<int> coords;
        
        // case 1
        x = 0;
        y = int(rho / sinTheta + 0.5);
        if ( x>=0 && x<=xMax && y>=0 && y<=yMax) {
            coords.push_back(x);
            coords.push_back(y);
        }
        
        // case 2
        x = xMax;
        y = int((rho - x * cosTheta) / sinTheta + 0.5);
        if ( x>=0 && x<=xMax && y>=0 && y<=yMax) {
            if (coords.size() == 2) {
                if (x!=coords[0] && y!=coords[1]) {
                    coords.push_back(x);
                    coords.push_back(y);
                }
            }
            else {
                coords.push_back(x);
                coords.push_back(y);
            }
        }
        
        // case 3
        y = 0;
        x = int(rho / cosTheta + 0.5);
        if ( x>=0 && x<=xMax && y>=0 && y<=yMax) {
            if (coords.size() == 2) {
                if (x!=coords[0] && y!=coords[1]) {
                    coords.push_back(x);
                    coords.push_back(y);
                }
            }
            else {
                coords.push_back(x);
                coords.push_back(y);
            }
        }
        
        // case 4
        y = yMax;
        x = int((rho - y * sinTheta) / cosTheta + 0.5);
        if ( x>=0 && x<=xMax && y>=0 && y<=yMax) {
            if (coords.size() == 2) {
                if (x!=coords[0] && y!=coords[1]) {
                    coords.push_back(x);
                    coords.push_back(y);
                }
            }
            else {
                coords.push_back(x);
                coords.push_back(y);
            }
        }
        
        // draw line (orientation)
        if (coords.size() >= 4) {
            line(im, coords[0], coords[1], coords[2], coords[3], 255, sobel);
        }
        
    }
    return 0;
}

/******************************************************************************************
 * writeImage
 ******************************************************************************************/
int writeImage(const Image *im, const char *fname) {
    FILE *output;
    int nRows;
    int nCols;
    int colors;
    int i, j;
    
    /* open the file */
    if (!fname || (output=fopen(fname,"w"))==0) {
        printf("writeImage: cannot open file\n");
        return(-1);
    }
    nRows=im->getNRows();
    nCols=im->getNCols();
    colors=im->getColors();
    
    printf("Saving image of size %d %d\n", nRows, nCols);
    /* write the header */
    fprintf(output,"P5\n"); /* magic number */
    fprintf(output,"#\n");  /* empty comment */
    fprintf(output,"%d %d\n%03d\n",nCols,nRows,colors); /* image info */
    
    /* write pixels row by row */
    for(i=0; i<nRows; i++)  {
        for(j=0; j<nCols; j++) {
            int byte=im->getPixel(i,j);
            if (fputc(byte,output)==EOF) /* couldn't write */ {
                fclose(output);
                printf("writeImage: could not write\n");
                return -1;
            }
        }
    }
    
    /* close the file */
    fclose(output);
    return 0; /* OK */
}

