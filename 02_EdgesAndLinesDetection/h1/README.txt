
The program finds the locations of edge points in the image.

Database.* : Database class for storing objects properties.

DisjSets.* : Disjoint sets class.

HoughDatabase.* : HoughDatabase class for Hough images.

Image.* : Image class (2-D array of integers, along with size, number of colors)
                      (For our purposes the number of colors is 256)

Line.cpp :  Functions to draw a line on an image.

Pgm.cpp   : Functions to read/write a pgm image file.

h1.cpp : Contains main().

----------------------
To compile in Linux:
-----------
 
   make all

To run:
-----------

./h1 hough_simple_1.pgm hough_simple_1_G_S.pgm
./h1 hough_simple_2.pgm hough_simple_2_G_S.pgm

This will open image hough_simple_1.pgm (or hough_simple_2.pgm), read it, smooth with Gaussian filter, apply Sobel operator and will save resulting image to file hough_simple_1_G_S.pgm (or hough_simple_2_G_S.pgm).

-----------
