
The program finds lines in the image from its Hough Transform space using a given threshold and draws the detected lines / edges on a copy of the original scene image.

Database.* : Database class for storing objects properties.

DisjSets.* : Disjoint sets class.

HoughDatabase.* : HoughDatabase class for Hough images.

Image.* : Image class (2-D array of integers, along with size, number of colors)
                      (For our purposes the number of colors is 256)

Line.cpp :  Functions to draw a line on an image.

Pgm.cpp   : Functions to read/write a pgm image file.

h4.cpp : Contains main().

----------------------
To compile in Linux:
-----------
 
   make all

To run:
-----------

./h4 hough_simple_1.pgm hough_simple_1_G_S_T42_B_H.pgm 134 hough_simple_1_G_S_T42_B_H_T134.pgm
./h4 hough_simple_2.pgm hough_simple_2_G_S_T20_B_H.pgm 145 hough_simple_2_G_S_T20_B_H_T145.pgm

This will open and read original image hough_simple_1.pgm (or hough_simple_2.pgm) and its Hough image hough_simple_1_G_S_T42_B_H.pgm (or hough_simple_2_G_S_T20_B_H.pgm), threshold the Hough image using value 134 (or 145) and will save result with detected lines to file hough_simple_1_G_S_T42_B_H_T134.pgm (or hough_simple_2_G_S_T20_B_H_T145.pgm).
The result of BONUS modification will be saved to file edges.mg

-----------
