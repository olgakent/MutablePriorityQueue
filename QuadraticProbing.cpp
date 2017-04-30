/*******************************************************************************
 Author:        Olga Kent
 Date Created:  April 27, 2017
 Course:        CSCI 335-02 Tuesday/Thursday 5:35-6:50
 Professor:     Ioannis Stamos

 Assignment #4  QuadraticProbing.cpp
                Templated Hash Table with Quadratic Probing. The code taken from
                Data Structures and Algorithm Analysis in C++ (Fourth Edition),
                by Mark Allen Weiss
                http://users.cis.fiu.edu/~weiss/dsaa_c++4/code/
 *******************************************************************************/
#include "QuadraticProbing.h"
#include <iostream>
using namespace std;

/**
 * Internal method to test if a positive number is prime.
 * Not an efficient algorithm.
 */
bool isPrime( int n )
{
    if( n == 2 || n == 3 )
        return true;

    if( n == 1 || n % 2 == 0 )
        return false;

    for( int i = 3; i * i <= n; i += 2 )
        if( n % i == 0 )
            return false;

    return true;
}

/**
 * Internal method to return a prime number at least as large as n.
 * Assumes n > 0.
 */
int nextPrime( int n )
{
    if( n % 2 == 0 )
        ++n;

    for( ; !isPrime( n ); n += 2 )
        ;

    return n;
}
