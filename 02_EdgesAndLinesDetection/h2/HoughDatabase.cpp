/******************************************************************************************
 Title          : HoughDatabase.cpp
 Author         : Ilona Michalowska
 Created on     : October 22, 2015
 Description    : Implementation file for HoughDatabase.h header file.
 ******************************************************************************************/

#include <iostream>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstring>
#include "HoughDatabase.h"

using namespace std;

/**
 * Initialize database object with NumberOfRecords records.
 * (Label of the element is i+1).
 */
void HoughDatabase::initializeRecords(int numberOfRecords) {
    for (int i=0; i<numberOfRecords; i++) {
        Record r;
        r.areaSum = 0;
        r.areaWeightedSum = 0;
        r.iWeightedSum = 0;
        r.jWeightedSum = 0;
        r.rowCenter = -1;
        r.colCenter = -1;
        r.rho = -1;
        r.theta = -1;
        records.push_back(r);
    }
}

/**
 * Update record for a given object (for raster scan).
 */
void HoughDatabase::updateSums(int recordLabel, int i, int j, int v) {
    if (recordLabel>0 && recordLabel<=records.size( )) {
        records[recordLabel-1].areaSum++;
        records[recordLabel-1].areaWeightedSum += v;
        records[recordLabel-1].iWeightedSum += i * v;
        records[recordLabel-1].jWeightedSum += j * v;
    }
}

/**
 * Calculate properties of objects in the database.
 */
void HoughDatabase::calculateRhoTheta(int rhoShift) {
    //int numOtThetaUnits = 180 * 5; /* from HoughTransform */
    
    double r, t, rho, theta;
    long long int A;
    
    for (int i=0; i<records.size(); i++) {
        A = records[i].areaWeightedSum;
        r = 1.0 * records[i].iWeightedSum / A;
        t = 1.0 * records[i].jWeightedSum / A;
        
        rho = (r-rhoShift);
        theta = t/5;

        records[i].rowCenter = int (r + 0.5);
        records[i].colCenter = int (t + 0.5);
        records[i].rho = rho;
        records[i].theta = theta;
   }
}

/**
 * Print records for all objects in the database.
 */
void HoughDatabase::printRecords() const {
    cout << "areaSum\tareaWeightedSum\tiSum\tjSum\trowCenter\tcolCenter\trho\ttheta\n";
    for (int i=0; i<records.size( ); i++) {
        cout << records[i].areaSum << "\t"
             << records[i].areaWeightedSum << "\t"
             << records[i].iWeightedSum << "\t"
             << records[i].jWeightedSum << "\t"
             << records[i].rowCenter << "\t"
             << records[i].colCenter << "\t"
             << records[i].rho << "\t"
             << records[i].theta << endl;
    }
}

/**
 * Return database records.
 */
vector<HoughDatabase::Record> HoughDatabase::getRecords() const {
    return records;
}

/**
 * Optimize database records.
 */
void HoughDatabase::optimize() {
    int sizeOfRecords, weight1, weight2;
    double rho1, rho2, theta1, theta2;
    
    sizeOfRecords = int(records.size());
    
    for (int i=0; i<sizeOfRecords; i++) {
        for (int j=0; j<sizeOfRecords; j++) {
            rho1 = records[i].rho;
            rho2 = records[j].rho;
            theta1 = records[i].theta;
            theta2 = records[j].theta;
            if (abs(rho1-rho2)<10 && abs(theta1-theta2)<7) {
                weight1 = int(records[i].areaWeightedSum);
                weight2 = int(records[j].areaWeightedSum);
                records[i].theta = (theta1*weight1+theta2*weight2) / (weight1+weight2);
                records[j].theta = (theta1*weight1+theta2*weight2) / (weight1+weight2);
                records[i].rho = (rho1*weight1+rho2*weight2) / (weight1+weight2);
                records[j].rho = (rho1*weight1+rho2*weight2) / (weight1+weight2);
                records[i].areaWeightedSum += weight2;
                records[j].areaWeightedSum += weight1;
            }
        }
    }
}


