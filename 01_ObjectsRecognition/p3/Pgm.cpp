/******************************************************************************************
 Title          : Pgm.cpp
 Author         : Anton Nikolaev
 Created on     : 1995
 Description    : Implementation file for Image.h header file.
 Modified by    : Ilona Michalowska
 Modified on    : September 22, 2015
 Comments       : Added functions
                  readAsBinaryImage
                  readAndLabelBinaryImage
                  readLabeledImage
                  addPositionAndOrientation
 ******************************************************************************************/

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "Image.h"
#include "DisjSets.h"
#include "Database.h"
#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

int readImage(Image *im, const char *fname)
/*
 reads image from fname;
 
 returns 0 if OK or -1 if something goes wrong.
 */
{
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

int readAsBinaryImage(Image *im, const char *fname, int threshold)
/*
 reads image from fname, saves as binary image in Image object im;
 
 returns 0 if OK or -1 if something goes wrong.
 */
{
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

int readAndLabelBinaryImage(Image *im, const char *fname)
/*
 reads binary image from fname, saves labeled binary image in Image object im;
 
 returns 0 if OK or -1 if something goes wrong.
 */
{
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

int readLabeledImage(Image *im, const char *fname, Database &db)
/*
 reads labeled image from fname, saves objects' info in db Database;
 
 returns 0 if OK or -1 if something goes wrong.
 */
{
    FILE *input;
    char line[1024];
    int nCols,nRows;
    int levels;
    int i, j;
    
    /* open it */
    if (!fname || (input=fopen(fname,"r"))==0){
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

int addPositionAndOrientation(Image *im, Database &db, bool recognizedOnly)
/*
 draws objects' positions and orientations in the image;
 
 returns 0.
 */
{
    vector<Database::Record> objects = db.getRecords( );
    int numOfObjects = objects.size( );
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
        
        // draw line (orientation)
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


int writeImage(const Image *im, const char *fname)
/*
 writes the image into fname;
 
 returns 0 if OK or -1 if something goes wrong.
 */
{
    FILE *output;
    int nRows;
    int nCols;
    int colors;
    int i, j;
    
    /* open the file */
    if (!fname || (output=fopen(fname,"w"))==0){
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
    for(i=0; i<nRows; i++)
    {
        for(j=0; j<nCols; j++)
        {
            int byte=im->getPixel(i,j);
            
            if (fputc(byte,output)==EOF) /* couldn't write */
            {
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

