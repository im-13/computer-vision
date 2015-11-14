
The program generates an image of the Hough Transform space of a given binary edge image.

Database.* : Database class for storing objects properties.

DisjSets.* : Disjoint sets class.

HoughDatabase.* : HoughDatabase class for Hough images.

Image.* : Image class (2-D array of integers, along with size, number of colors)
                      (For our purposes the number of colors is 256)

Line.cpp :  Functions to draw a line on an image.

Pgm.cpp   : Functions to read/write a pgm image file.

h3.cpp : Contains main().

----------------------
To compile in Linux:
-----------
 
   make all

To run:
-----------

./h3 hough_simple_1_G_S_T42_B.pgm hough_simple_1_G_S_T42_B_H.pgm
./h3 hough_simple_2_G_S_T20_B.pgm hough_simple_2_G_S_T20_B_H.pgm

This will open image hough_simple_1_G_S_T42_B.pgm (or hough_simple_2_G_S_T20_B.pgm), read it, perform Hough transform on it and will save resulting Hough image to file hough_simple_1_G_S_T42_B_H.pgm (or hough_simple_2_G_S_T20_B_H.pgm).

-----------
