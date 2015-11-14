
The program converts a gray–level image to a binary one using a command-line provided threshold value.

Database.* : Database class for storing objects properties.

DisjSets.* : Disjoint sets class.

HoughDatabase.* : HoughDatabase class for Hough images.

Image.* : Image class (2-D array of integers, along with size, number of colors)
                      (For our purposes the number of colors is 256)

Line.cpp :  Functions to draw a line on an image.

Pgm.cpp   : Functions to read/write a pgm image file.

h2.cpp : Contains main().

----------------------
To compile in Linux:
-----------
 
   make all

To run:
-----------

./h2 hough_simple_1_G_S.pgm 42 hough_simple_1_G_S_T42_B.pgm
./h2 hough_simple_2_G_S.pgm 20 hough_simple_2_G_S_T20_B.pgm

This will open image hough_simple_1_G_S.pgm (or hough_simple_2_G_S.pgm), read it, threshold using value 42( or 20) and will save resulting image to file hough_simple_1_G_S_T42_B.pgm (or hough_simple_2_G_S_T20_B.pgm).

-----------
