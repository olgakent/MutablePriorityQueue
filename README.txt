/*******************************************************************************
Author:			Olga Kent
Date Created:	April 27, 2017
Course:			CSCI 335-02 Tuesday/Thursday 5:35-6:50
Professor:		Ioannis Stamos

Assignment #4:  Mutable Priority Queues

Description containing which parts of the assignment were completed,
bugs encountered, instructions on how to run the program, and input/output files.

The .zip file contains files:   MutableQueue.h
                                MyPair.h
                                QuadraticProbing.h
                                QuadraticProbing.cpp
                                TestInsertAndSearch.cpp
                                TestInsertAndDelete.cpp
                                TestNewInsert.cpp
                                dsexceptions.h
                                Makefile
                                Input files:
                                        100_numbers.txt
                                        1000_numbers.txt
                                        10000_numbers.txt
                                        100_test_search.txt
                                        1000_test_search.txt
                                        100_test_deletion.txt,
                                        1000_test_deletion.txt
                                README.txt
*******************************************************************************/

I. Parts Completed
	all parts are completed
    Hash table uses std:hash function. So I provide struct hash to
    allow MyPair class insertion into HashTable.

II. Bugs Encountered
    I ran into a "Specialization of template in different namespace" warnings.
    For better interaction, I removed MyPair from  within the MutableQueue.

III. Instructions to run the program
    To compile and run the program on terminal type:
    - make all

	- make runsearch100
	- make runsearch1000
	- make rundelete100
	- make rundelete1000
    - make newinsert100
    - make newinsert1000

	To delete executables and object file type:
    -  make clean

IV. Input/Output Files
    - Input:
            100_numbers.txt
            1000_numbers.txt
            100_test_search.txt
            1000_test_search.txt
            100_test_deletion.txt,
            1000_test_deletion.txt
    - Output: n/a
