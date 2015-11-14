/******************************************************************************************
 Title          : DisjSets.h
 Author         : Ilona Michalowska
 Created on     : September 22, 2015
 Description    : Header file for a basic disjoint sets class called DisjSets.
 ******************************************************************************************/

#ifndef DISJ_SETS_H
#define DISJ_SETS_H

#include <vector>

using namespace std;

class DisjSets
{
  public:
    DisjSets( );

    int find( int x ) const;
    void unionSets( int root1, int root2 );
    void addElement( );
    void printSet( ) const;
    int getNumberOfLabels( ) const;
    int getNumberOfLevels( ) const;
    vector<int> getLevels( ) const;

  private:
    vector<int> s;
};

#endif
