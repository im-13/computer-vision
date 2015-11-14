/******************************************************************************************
 Title          : Database.h
 Author         : Ilona Michalowska
 Created on     : September 22, 2015
 Description    : Header file for image objects database class called Database.
 ******************************************************************************************/

#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

class Database {
  public:
    
    struct Record {
        long long int   areaSum;
        long long int   iSum;
        long long int   jSum;
        long long int   ijSum;
        long long int   iSquaredSum;
        long long int   jSquaredSum;
        int             rowCenter;
        int             colCenter;
        double          theta;
        double          minE;
        double          maxE;
        bool            recognized;
    };
    
    void initializeRecords(int numberOfRecords);
    void addBasicRecord(int rowC, int colC, double theta, double minE, double maxE, long long int area);
    void updateSums(int recordLabel, int i, int j);
    void calculateProperties( );
    void printRecords( ) const;
    int readBasicFromTxtFile(const char *fname);
    int saveInTxtFile(const char *fname) const;
    int recognizeObjects(const Database &other);
    vector<Record> getRecords( ) const;

  private:
    
    vector<Record> records;
};

#endif
