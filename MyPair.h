/********************************************************************************
Author:        Olga Kent
Date Created:  April 27, 2017
Course:        CSCI 335-02 Tuesday/Thursday 5:35-6:50
Professor:     Ioannis Stamos

Assignment #4  MyPair.h
			Struct MyPair which is inserted into a hash table to include
			the ability to search and remove any item in MutableQueue. Struct
			hash allows MyPair class insertion into HashTable.
*******************************************************************************/

#ifndef MYPAIR_H
#define MYPAIR_H

template<typename a, typename b>
struct MyPair {
	a key;
	b data;
	MyPair() {};
	MyPair(a key, b data):key(key),data(data) {};
	bool operator>(const MyPair<a, b>&other) const {
		return key > other.key;
	}
	bool operator<(const MyPair<a, b>&other) const {
		return key < other.key;
	}
	bool operator!=(const MyPair<a, b>&other) const {
		return key != other.key&&data != other.data;
	}
};

namespace std {

    template<typename a, typename b>
    struct hash< MyPair<a, b> > {
    private:
    	const std::hash<a> ah;
    	const std::hash<b> bh;
    public:
    	hash() : ah(), bh() {}
    	size_t operator()(const MyPair<a, b> &p) const {
    		return ah(p.key);
    	}
    };
}
#endif
