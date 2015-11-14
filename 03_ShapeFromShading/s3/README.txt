
The program, given 3 images of an object, computes the normals to that object’s surface, and outputs object’s "needle map”.

Database.* : Database class for storing objects properties.

DisjSets.* : Disjoint sets class.

HoughDatabase.* : HoughDatabase class for Hough images.

Image.* : Image class (2-D array of integers, along with size, number of colors)
                      (For our purposes the number of colors is 256)

Line.cpp :  Functions to draw a line on an image.

Pgm.cpp   : Functions to read/write a pgm image file.

s3.cpp : Contains main().

----------------------
To compile in Linux:
-----------
 
   make all

To run:
-----------

./s3 directions.txt sphere1.pgm sphere2.pgm sphere3.pgm 15 85 sphereNeedleMap.pgm
./s3 directions.txt object1.pgm object2.pgm object3.pgm 15 85 objectNeedleMap.pgm

This will open and read file directions.txt, images sphere1.pgm, sphere2.pgm, sphere3.pgm (object1.pgm, object2.pgm, object3.pgm), find surface normals and will save “needle map” in file sphereNeedleMap.pgm (objectNeedleMap.pgm).

-----------
