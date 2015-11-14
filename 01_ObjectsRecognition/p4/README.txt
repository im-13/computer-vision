
The program recognizes objects from the database in the image. Objects are recognized based on their areas and roundness ratios (E min / E max). Two objects are considered the same if their areas differ by 15% and their roundness ratios differ by 10%.

Image.* : Image class (2-D array of integers, along with size, number of colors)
                      (For our purposes the number of colors is 256)

DisjSets.* : Disjoint sets class.

Database.* : Database class for storing objects properties.

Pgm.cpp   : Functions to read/write a pgm image file.

Line.cpp :  Functions to draw a line on an image.

p4.cpp : Contains main().

----------------------
To compile in Linux:
-----------
 
   make all

To run:
-----------

./p4 many_objects_1_120_B_L.pgm database.txt many_objects_1_120_B_L_R.pgm

or

./p4 many_objects_2_120_B_L.pgm database.txt many_objects_2_120_B_L_R.pgm

This will open labeled image many_objects_1_120_B_L.pgm (or many_objects_2_120_B_L.pgm), read in objects from database.txt, and save output image with recognized objects to file many_objects_1_120_B_L_R.pgm (or many_objects_2_120_B_L_R.pgm).
-----------
