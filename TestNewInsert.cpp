/********************************************************************************
Author:        Olga Kent
Date Created:  April 27, 2017
Course:        CSCI 335-02 Tuesday/Thursday 5:35-6:50
Professor:     Ioannis Stamos

Assignment #4  TestNewInsert.cpp
               This program implements a Mutable Priority Queue and tests the
               search functionality. First, it reads all the entries from the
               input file and inserts them into the mutable priority queue.
               Then it deletes every key from a delete file and print whether the
               deletion was successful.

*******************************************************************************/

#include <iostream>
#include <fstream>
#include "QuadraticProbing.h"
#include "MutableQueue.h"
using namespace std;

int main(int argc, char** argv){
	if(argc != 3){
		cout << "Usage: " << argv[0] << " <input_file_to_create_queue> <input_file_to_check_search>\n";
		return 1;
	}

	cout << "\nInput file to create queue is " << argv[1] << endl;
	cout << "Input file to check search is " << argv[2] << endl;

	// Open input file. Exit if it fails.
	ifstream inputfile(argv[1]);
	if(inputfile.fail()){
		cout << "Error opening: " << argv[1] << endl;
		exit(1);
	}

	// Open search file. Exit if it fails.
	ifstream searchfile(argv[2]);
	if(searchfile.fail()) {
		cout << "Error opening: " << argv[2] << endl;
		exit(1);
	}

	MutableQueue<int> my_queue;
	int value, count = 0;
	while (inputfile >> value) {
		my_queue.newInsert(value);
		count++;
	}
	cout << "Success inserting " << count << " elements into the queue. ";
	cout << "The minimum element is " << my_queue.findMin() << endl;
	while (searchfile >> value) {
		if (my_queue.find(value)) {
			cout << value << " Found" << endl;
		}
		else {
			cout << value << " Not found" << endl;
		}
	}


	return 0;
}
