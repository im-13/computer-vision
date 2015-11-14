/******************************************************************************************
 Title          : DisjSets.cpp
 Author         : Ilona Michalowska
 Created on     : September 22, 2015
 Description    : Implementation file for DisjSets.h header file.
 ******************************************************************************************/

#include "DisjSets.h"
#include <iostream>

using namespace std;

/**
 * Construct the initial disjoint sets object with 1 element.
 * Element at index 0 will not be used.
 */
DisjSets::DisjSets( )
{
    s.push_back(-1);
}

/**
 * Union two disjoint sets.
 * If root1 and root2 are in the same set, do nothing.
 */
void DisjSets::unionSets( int root1, int root2 )
{
    if (root1!=0 && root2!=0) {
        int r1,r2;
        r1 = find(root1);
        r2 = find(root2);
        if (r1!=r2) {
            if (r1<r2) {
                s[root2] = r1;
                s[r2] = r1;
            }
            else {
                s[root1] = r2;
                s[r1] = r2;
            }
        }
    }
}


/**
 * Perform a find.
 * Return index of the set containing x if element is found, otherwise return -1.
 */

int DisjSets::find( int x ) const
{
    if (x > 0) {
        if( s[ x ] < 0 )
            return x;
        else
            return find( s[ x ] );
    }
    else {
        return -1;
    }
}

/**
 * Add a new element to the set s, set up as root (-1)
 */
void DisjSets::addElement( )
{
    s.push_back(-1);
}

/**
 * Return the number of labels
 */
int DisjSets::getNumberOfLabels( ) const
{
    return int(s.size( )) - 1;
}

/**
 * Print the set (all labels)
 */
void DisjSets::printSet( ) const
{
    
    for (int i=0; i<s.size( ); i++) {
        cout << "s[" << i << "] = " << s[i] << endl;
    }
    cout << "END OF s\n" << endl;
}

/**
 * Return the number of disjoint sets
 */
int DisjSets::getNumberOfLevels( ) const
{
    int numOfLevels = 0, setSize = int(s.size( ));
    for (int i=1; i<setSize; i++) {
        if (s[i]<0) {
            numOfLevels++;
        }
    }
    return numOfLevels;
}

/**
 * Return a list of distinct labels (levels)
 */
vector<int> DisjSets::getLevels( ) const
{
    vector<int> listOfLevels;
    int setSize = int(s.size( ));
    for (int i=1; i<setSize; i++) {
        if (s[i]<0) {
            listOfLevels.push_back(i);
        }
    }
    return listOfLevels;
}
