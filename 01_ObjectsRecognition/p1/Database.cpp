/******************************************************************************************
 Title          : Database.cpp
 Author         : Ilona Michalowska
 Created on     : September 22, 2015
 Description    : Implementation file for Database.h header file.
 ******************************************************************************************/

#include "Database.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstring>

using namespace std;

/**
 * Initialize database object with NumberOfRecords records.
 * (Label of the element is i+1).
 */
void Database::initializeRecords(int numberOfRecords)
{
    for (int i=0; i<numberOfRecords; i++) {
        Record r;
        r.areaSum = 0;
        r.iSum = 0;
        r.jSum = 0;
        r.ijSum = 0;
        r.iSquaredSum = 0;
        r.jSquaredSum = 0;
        r.rowCenter = -1;
        r.colCenter = -1;
        r.theta = -1;
        r.minE = -1;
        r.maxE = -1;
        r.recognized = false;
        records.push_back(r);
    }
}

/**
 * Add record with basic object information.
 */
void Database::addBasicRecord(int rowC, int colC, double theta, double minE, double maxE, long long int area)
{
    Record r;
    r.areaSum = area;
    r.iSum = 0;
    r.jSum = 0;
    r.ijSum = 0;
    r.iSquaredSum = 0;
    r.jSquaredSum = 0;
    r.rowCenter = rowC;
    r.colCenter = colC;
    r.theta = theta;
    r.minE = minE;
    r.maxE = maxE;
    r.recognized = false;
    records.push_back(r);
}

/**
 * Update record for a given object (for raster scan).
 */
void Database::updateSums(int recordLabel, int i, int j)
{
    if (recordLabel>0 && recordLabel<=records.size( )) {
        records[recordLabel-1].areaSum++;
        records[recordLabel-1].iSum += i;
        records[recordLabel-1].jSum += j;
        records[recordLabel-1].ijSum += i*j;
        records[recordLabel-1].iSquaredSum += i*i;
        records[recordLabel-1].jSquaredSum += j*j;
    }
}

/**
 * Calculate properties of objects in the database.
 */
void Database::calculateProperties( )
{
    double x, y, minTheta, maxTheta, minE, maxE;
    long long int A, aPrim, bPrim, cPrim, a, b, c;

    for (int i=0; i<records.size(); i++) {
        A = records[i].areaSum;
        x = 1.0 * records[i].iSum / A;
        y = 1.0 * records[i].jSum / A;
        aPrim = records[i].iSquaredSum;
        bPrim = 2 * records[i].ijSum;
        cPrim = records[i].jSquaredSum;
        
        a = aPrim - x * x * A;
        b = bPrim - 2 * x * y * A;
        c = cPrim - y * y * A;
        
        minTheta = 0.5 * atan2(double(b), double(a-c));
        minE = a * pow(sin(minTheta),2) - b * cos(minTheta) * sin(minTheta) + c * pow(cos(minTheta),2);
        
        maxTheta = M_PI / 2 + minTheta;
        maxE = a * pow(sin(maxTheta),2) - b * cos(maxTheta) * sin(maxTheta) + c * pow(cos(maxTheta),2);
        
        records[i].rowCenter = int (x + 0.5);
        records[i].colCenter = int (y + 0.5);
        records[i].theta = minTheta;
        records[i].minE = minE;
        records[i].maxE = maxE;
   }
}

/**
 * Print records for all objects in the database.
 */
void Database::printRecords( ) const
{
    cout << "areaSum\tiSum\tjSum\tijSum\tiSquaredSum\tjSquaredSum\trowCenter\tcolCenter\ttheta\tminE\tmaxE\troundness\trecognized\n";
    for (int i=0; i<records.size( ); i++) {
        cout << records[i].areaSum << "\t"
             << records[i].iSum << "\t"
             << records[i].jSum << "\t"
             << records[i].ijSum << "\t"
             << records[i].iSquaredSum << "\t"
             << records[i].jSquaredSum << "\t"
             << records[i].rowCenter << "\t"
             << records[i].colCenter << "\t"
             << records[i].theta << "\t"
             << records[i].minE << "\t"
             << records[i].maxE << "\t"
             << records[i].minE/records[i].maxE << "\t"
             << records[i].recognized << endl;
    }
}

/**
 * Read basic object info from a txt file. Return 0 if OK or -1 if something goes wrong.
 */
int Database::readBasicFromTxtFile(const char *fname)
{
    FILE *input;
    char line[1024];
    
    /* open it */
    if (!fname || (input=fopen(fname, "r"))==0){
        printf("readDatabase: Cannot open file\n");
        return -1;
    }
    
    /* check the header */
    if (strncmp(fgets(line, sizeof line, input), "label\trowC\tcolC\ttheta\tminE\tmaxE\tarea\n", 37))
    {
        fclose(input);
        printf("readDatabase: Invalid format\n");
        return -1;
    }

    int label, rowC, colC;
    double theta, minE, maxE;
    long long int area;
    
    while (fgets(line, sizeof line, input) != 0) {
        sscanf(line, "%d\t%d\t%d\t%lf\t%lf\t%lf\t%lld\n", &label, &rowC, &colC, &theta, &minE, &maxE, &area);
        this->addBasicRecord(rowC, colC, theta*M_PI/180, minE, maxE, area);
    }
    
    /* close the file */
    fclose(input);
    return 0; /* OK */
}


/**
 * Save database in txt file. Return 0 if OK or -1 if something goes wrong.
 */
int Database::saveInTxtFile(const char *fname) const
{
    FILE *output;
    
    /* open the file */
    if (!fname || (output=fopen(fname,"w"))==0){
        printf("writeDatabase: cannot open file\n");
        return(-1);
    }
    
    int numOfRecords = records.size( );
    printf("Saving database with %d records\n", numOfRecords);
    
    /* write the header */
    fprintf(output,"label\trowC\tcolC\ttheta\tminE\tmaxE\tarea\n");
    
    /* write records */
    for(int i=0; i<numOfRecords; i++)
    {
        fprintf(output,"%d\t%d\t%d\t%f\t%f\t%f\t%lld\n",i+1,records[i].rowCenter,records[i].colCenter,records[i].theta*180/M_PI,records[i].minE,records[i].maxE,records[i].areaSum);
    }
    
    /* close the file */
    fclose(output);
    return 0; /* OK */
}

/**
 * Compare objects in the database with objects in other database.
 * Return 0 if any object is recognized, -1 otherwise.
 */
int Database::recognizeObjects(const Database &other)
{
    int numOfRecognized = 0;
    vector<Database::Record> recordsOther = other.getRecords( );
    int numOfRecords = records.size( );
    int numOfRecordsInOther = recordsOther.size( );
    long long int area, areaOther;
    double minE, maxE, minEOther, maxEOther, roundness, roundnessOther;

    for (int i=0; i<numOfRecords; i++) {
        area = records[i].areaSum;
        minE = records[i].minE;
        maxE = records[i].maxE;
        roundness = minE / maxE;
        for (int j=0; j<numOfRecordsInOther; j++) {
            areaOther = recordsOther[j].areaSum;
            minEOther = recordsOther[j].minE;
            maxEOther = recordsOther[j].maxE;
            roundnessOther = minEOther / maxEOther;
            // similar area nad similar roundness
            if (
                ((area <= areaOther) ? (1.0*area/areaOther >= 0.85) : (1.0*areaOther/area >= 0.85))
                &&
                ((roundness <= roundnessOther) ? (roundness/roundnessOther >= 0.90) : (roundnessOther/roundness >=0.90))
                ) {
                records[i].recognized = true;
                numOfRecognized++;
            }
        }
    }
    
    if (numOfRecognized) {
        printf("Recognized %d objects\n", numOfRecognized);
        return 0;
    }
    else {
        return 1;
    }
}

/**
 * Return database records.
 */
vector<Database::Record> Database::getRecords( ) const
{
    return records;
}


