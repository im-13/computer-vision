
The program computes the surface albedo and outputs an albedo map.

Database.* : Database class for storing objects properties.

DisjSets.* : Disjoint sets class.

HoughDatabase.* : HoughDatabase class for Hough images.

Image.* : Image class (2-D array of integers, along with size, number of colors)
                      (For our purposes the number of colors is 256)

Line.cpp :  Functions to draw a line on an image.

Pgm.cpp   : Functions to read/write a pgm image file.

s4.cpp : Contains main().

----------------------
To compile in Linux:
-----------
 
   make all

To run:
-----------

./s4 directions.txt sphere1.pgm sphere2.pgm sphere3.pgm 85 sphereAlbedoMap.pgm
./s4 directions.txt object1.pgm object2.pgm object3.pgm 85 objectAlbedoMap.pgm

This will open and read file directions.txt, images sphere1.pgm, sphere2.pgm, sphere3.pgm (object1.pgm, object2.pgm, object3.pgm), find surface albedos and will save albedo map in file sphereAlbedoMap.pgm (objectAlbedoMap.pgm).

-----------
