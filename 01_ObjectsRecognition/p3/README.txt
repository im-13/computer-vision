
The program takes a labeled image as input, computes object attributes, generates the database of the objects, and displays positions and orientations of objects in the output image.

Image.* : Image class (2-D array of integers, along with size, number of colors)
                      (For our purposes the number of colors is 256)

DisjSets.* : Disjoint sets class.

Database.* : Database class for storing objects properties.

Pgm.cpp   : Functions to read/write a pgm image file.

Line.cpp :  Functions to draw a line on an image.

p3.cpp : Contains main().

----------------------
To compile in Linux:
-----------
 
   make all

To run:
-----------

./p3 two_objects_120_B_L.pgm database.txt two_objects_120_B_L_PO.pgm

This will open labeled image two_objects_120_B_L.pgm, save objects properties in database.txt, and save output image to file two_objects_120_B_L_PO.pgm.

-----------
