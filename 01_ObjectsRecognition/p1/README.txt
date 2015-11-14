
The program converts a gray–level image to a binary one using a command-line provided threshold value.

Image.* : Image class (2-D array of integers, along with size, number of colors)
                      (For our purposes the number of colors is 256)

DisjSets.* : Disjoint sets class.

Database.* : Database class for storing objects properties.

Pgm.cpp   : Functions to read/write a pgm image file.

Line.cpp :  Functions to draw a line on an image.

p1.cpp : Contains main().

----------------------
To compile in Linux:
-----------
 
   make all

To run:
-----------

./p1 two_objects.pgm 120 two_objects_120_B.pgm

This will open image two_objects.pgm, read it as binary image with threshold value 120, and will save resulting image to file two_objects_120_B.pgm.

-----------
