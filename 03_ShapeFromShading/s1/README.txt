
The program locates the sphere in an image and computes its center and radius.

Database.* : Database class for storing objects properties.

DisjSets.* : Disjoint sets class.

HoughDatabase.* : HoughDatabase class for Hough images.

Image.* : Image class (2-D array of integers, along with size, number of colors)
                      (For our purposes the number of colors is 256)

Line.cpp :  Functions to draw a line on an image.

Pgm.cpp   : Functions to read/write a pgm image file.

s1.cpp : Contains main().

----------------------
To compile in Linux:
-----------
 
   make all

To run:
-----------

./s1 sphere0.pgm 85 sphereProperties.txt

This will open image sphere0.pgm, read it with threshold value 85, find sphere’s center and radius and will save results in file sphereProperties.txt.

-----------
