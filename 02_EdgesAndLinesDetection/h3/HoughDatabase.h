/******************************************************************************************
 Title          : HoughDatabase.h
 Author         : Ilona Michalowska
 Created on     : October 22, 2015
 Description    : Header file for image objects database class called HoughDatabase.
 ******************************************************************************************/

#ifndef HOUGH_DATABASE_H
#define HOUGH_DATABASE_H

#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

class HoughDatabase {
  public:
    
    struct Record {
        long long int   areaSum;
        long long int   areaWeightedSum;
        long long int   iWeightedSum;
        long long int   jWeightedSum;
        int             rowCenter;
        int             colCenter;
        double             rho;
        double             theta;
    };
    
    void initializeRecords(int numberOfRecords);
    void updateSums(int recordLabel, int i, int j, int v);
    void calculateRhoTheta(int rhoShift);
    void printRecords() const;
    void optimize();
    vector<Record> getRecords() const;

  private:
    
    vector<Record> records;
};

#endif
