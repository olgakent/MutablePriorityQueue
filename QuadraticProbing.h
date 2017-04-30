/*******************************************************************************
 Author:        Olga Kent
 Date Created:  April 27, 2017
 Course:        CSCI 335-02 Tuesday/Thursday 5:35-6:50
 Professor:     Ioannis Stamos

 Assignment #4  QuadraticProbing.h
                Templated Hash Table with Quadratic Probing. The code taken from
                Data Structures and Algorithm Analysis in C++ (Fourth Edition),
                by Mark Allen Weiss. http://users.cis.fiu.edu/~weiss/dsaa_c++4/code/
                The code was modified and some functions were added for use with
                MutableQueue.
 *******************************************************************************/
#ifndef QUADRATIC_PROBING_H
#define QUADRATIC_PROBING_H

#include <vector>
#include <algorithm>
#include <functional>
#include <string>

using namespace std;

int nextPrime( int n );

// QuadraticProbing Hash table class
//
// CONSTRUCTION: an approximate initial size or default of 101
//
// ******************PUBLIC OPERATIONS*********************
// bool insert( x )       --> Insert x
// bool remove( x )       --> Remove x
// bool contains( x )     --> Return true if x is present
// void makeEmpty( )      --> Remove all items
// int hashCode( string str ) --> Global method to hash strings

template <typename HashedObj>
class HashTable
{
  public:
    explicit HashTable( int size = 101 ) : array( nextPrime( size ) )
      { makeEmpty( ); }

    // Description: Returns true if item @x is in the hash table
    bool contains( const HashedObj & x ) const
    {
        return isActive( findPos( x ) );
    }

    // Description: Makes the hash table empty
    void makeEmpty( )
    {
        currentSize = 0;
        for( auto & entry : array )
            entry.info = EMPTY;
    }
    // Description: Inserts item @x to the hash table
    bool insert( const HashedObj & x )
    {
        // Insert x as active
        int currentPos = findPos( x );
        if( isActive( currentPos ) )
            return false;

        if( array[ currentPos ].info != DELETED )
            ++currentSize;

        array[ currentPos ].element = x;
        array[ currentPos ].info = ACTIVE;

        // Rehash; see Section 5.5
        if( currentSize > array.size( ) / 2 )
            rehash( );

        return true;
    }
    // Description: Inserts item @x as an rvalue reference
    bool insert( HashedObj && x )
    {
            // Insert x as active
        int currentPos = findPos( x );
        if( isActive( currentPos ) )
            return false;

        if( array[ currentPos ].info != DELETED )
            ++currentSize;

        array[ currentPos ] = std::move( x );
        array[ currentPos ].info = ACTIVE;

            // Rehash; see Section 5.5
        if( currentSize > array.size( ) / 2 )
            rehash( );

        return true;
    }
    // Description: Removes item @x from the hash table
    bool remove( const HashedObj & x )
    {
        int currentPos = findPos( x );
        if( !isActive( currentPos ) )
            return false;

        array[ currentPos ].info = DELETED;
        return true;
	};

    enum EntryType { ACTIVE, EMPTY, DELETED };

    // Added by Olga K.
    // Description: Returns the requested Hashed Object
	HashedObj getItem(const HashedObj & x)
	{
		HashEntry h = array[findPos(x)];
		if (h.info== ACTIVE)
			return h.element;
		else
			return HashedObj();
	}
  private:
    struct HashEntry
    {
        HashedObj element;
        EntryType info;

        HashEntry( const HashedObj & e = HashedObj{ }, EntryType i = EMPTY )
          : element{ e }, info{ i } { }

        HashEntry( HashedObj && e, EntryType i = EMPTY )
          : element{ std::move( e ) }, info{ i } { }
    };
    // hash table with entries
    vector<HashEntry> array;
    // number of items in the table
    int currentSize;

    // Description: Returns true if position @current_pos in the hash table is ACTIVE
    bool isActive( int currentPos ) const
      { return array[ currentPos ].info == ACTIVE; }

    // Description: Returns the position of item @x in the hash table
    int findPos( const HashedObj & x ) const
    {
        int offset = 1;
        int currentPos = myhash( x );

        while( array[ currentPos ].info != EMPTY &&
               array[ currentPos ].element != x )
        {
            currentPos += offset;  // Compute ith probe
            offset += 2;
            if( currentPos >= array.size( ) )
                currentPos -= array.size( );
        }

        return currentPos;
    }
    // Description: Rehashes the table by creating a new double-sized table
    void rehash( )
    {
        vector<HashEntry> oldArray = array;

        // Create new double-sized, empty table
        array.resize( nextPrime( 2 * oldArray.size( ) ) );
        for( auto & entry : array )
            entry.info = EMPTY;

        // Copy table over
        currentSize = 0;
        for( auto & entry : oldArray )
            if( entry.info == ACTIVE )
                insert( std::move( entry.element ) );
    }
    size_t myhash( const HashedObj & x ) const
    {
        static hash<HashedObj> hf;
        return hf( x ) % array.size( );
    }

};

#endif // QUADRATIC_PROBING_H
