
The program finds the normal from the brightest surface spot on the sphere in each of the 3 input images, directed toward the light source.

Database.* : Database class for storing objects properties.

DisjSets.* : Disjoint sets class.

HoughDatabase.* : HoughDatabase class for Hough images.

Image.* : Image class (2-D array of integers, along with size, number of colors)
                      (For our purposes the number of colors is 256)

Line.cpp :  Functions to draw a line on an image.

Pgm.cpp   : Functions to read/write a pgm image file.

s2.cpp : Contains main().

----------------------
To compile in Linux:
-----------
 
   make all

To run:
-----------

./s2 sphereProperties.txt sphere1.pgm sphere2.pgm sphere3.pgm directions.txt

This will open and read file sphereProperties.txt, images sphere1.pgm, sphere2.pgm, sphere3.pgm, find surface normals directed toward the light sources and will save results in file directions.txt.

-----------

Formula to compute the normal vector to the sphere’s surface at a given point:
x,y - image plane coordinates
X,Y,Z - sphere coordinates

This formula gives the resulting normal vector in a 3-D coordinate system, originating at the sphere’s center, having its x-axis and y-axis parallel respectively to the x-axis and the y-axis of the image, and z-axis chosen such as to form an orthonormal right-hand coordinate system.

    if (x < xCenter) {
        X = - fabs(xCenter - x);
    }
    else if (x > xCenter) {
        X = fabs(xCenter - x);
    }
    else {
        X =0;
    }
    /* find Y */
    if (y < yCenter) {
        Y = - fabs(yCenter - y);
    }
    else if (y > yCenter) {
        Y = fabs(yCenter - y);
    }
    else {
        Y =0;
    }
    /* find Z */
    Z = sqrt(pow(radius,2) - pow(X,2) - pow(Y,2));

    /* scale normal vector */
    double length  = sqrt(pow(X,2) + pow(Y,2) + pow(Z,2));
    n[0] = X * intensity / length;
    n[1] = Y * intensity / length;
    n[2] = Z * intensity / length;
