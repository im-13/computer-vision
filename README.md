# computer-vision

Repository contains projects in the field of computer vision:

01_ObjectsRecognition
A vision system that recognizes 2-D objects in gray–level PGM images:
- converts a gray–level image to a binary one using a given threshold value
- segments a binary image into several connected regions and labels objects
- computes objects’ attributes in a labeled image, and generates the database of the objects
- recognizes objects in the image by comparing with objects stored in a database 
- outputs image displaying the positions and orientations (using dots and line segments) of the objects that have been recognized.

02_EdgesAndLinesDetection
A vision system that recognizes lines in an image using the Hough Transform:
- finds the locations of edge points in the Gaussian-filtered image using Sobel operator
- threshold the edge image in order to get strong edges
- generates an image of the Hough Transform space of a given binary edge image 
- finds lines in the image from its Hough Transform space, using a given threshold, and draws the detected lines on a copy of the original scene image
- trims detected lines so that they do not extend beyond the objects they belong to

03_ShapeFromShading
A vision system that recovers the orientation and reflectance of an object’s surface using photometric stereo. Given three images of an object taken using three different light sources, it computes the surface normals and albedo for this object.
- finds the location of the calibration object—a sphere—and its radius
- computes the directions and intensities of the light sources
- computes the surface normals and albedo of the object, and outputs its "needles" map and albedo map 
