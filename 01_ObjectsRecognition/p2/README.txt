
The program segments a binary image into several connected regions.

Image.* : Image class (2-D array of integers, along with size, number of colors)
                      (For our purposes the number of colors is 256)

DisjSets.* : Disjoint sets class.

Database.* : Database class for storing objects properties.

Pgm.cpp   : Functions to read/write a pgm image file.

Line.cpp :  Functions to draw a line on an image.

p2.cpp : Contains main().

----------------------
To compile in Linux:
-----------
 
   make all

To run:
-----------

./p2 two_objects_120_B.pgm two_objects_120_B_L.pgm

This will open binary image two_objects_120_B.pgm, label it, and save resulting image to file two_objects_120_B_L.pgm.

-----------
