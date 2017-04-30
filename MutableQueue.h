/********************************************************************************
Author:        Olga Kent
Date Created:  April 27, 2017
Course:        CSCI 335-02 Tuesday/Thursday 5:35-6:50
Professor:     Ioannis Stamos

Assignment #4  MutableQueue.h
               The Mutable Priority Queue class based on the implementation of
               BinomialQueue.h taken from the book Data Structures and
               Algorithm Analysis in C++ (Fourth Edition), by Mark Allen Weiss
               http://users.cis.fiu.edu/~weiss/dsaa_c++4/code/
               The code was modified to include the ability to search for any
               specific key in the priority queue (i.e. not only the minimum key),
               and to be able to remove any specific key.
*******************************************************************************/

#ifndef MUTABLEQUEUE_H
#define MUTABLEQUEUE_H
#include <iostream>
#include "dsexceptions.h"
#include "QuadraticProbing.h"


using namespace std;
/************************ Added by Olga K.***************************************
- Struct MyPair which will be inserted into a hash table to include
the ability to search and remove any item in MutableQueue.
- Struct hash allows MyPair class insertion into HashTable.
*******************************************************************************/
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

// Mutable Priority Queue class
// based on the implementation of BinomialQueue.h from the book
// CONSTRUCTION: with no parameters
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x
// deleteMin( )           --> Return and remove smallest item
// Comparable findMin( )  --> Return smallest item
// bool isEmpty( )        --> Return true if empty; else false
// void makeEmpty( )      --> Remove all items
// void merge( rhs )      --> Absorb rhs into this heap
// ******************ERRORS********************************
// Throws UnderflowException as warranted
template <typename Comparable>
class MutableQueue
{
public:
	MutableQueue() : theTrees(DEFAULT_TREES)
	{
		for (auto & root : theTrees)
			root = nullptr;
		currentSize = 0;
	}

	MutableQueue(const Comparable & item) : theTrees(1), currentSize{ 1 }
	{
		theTrees[0] = new BinomialNode{ item, nullptr, nullptr };
	}

	MutableQueue(const MutableQueue & rhs)
		: theTrees(rhs.theTrees.size()), currentSize{ rhs.currentSize }
	{
		for (int i = 0; i < rhs.theTrees.size(); ++i)
			theTrees[i] = clone(rhs.theTrees[i]);
	}

	MutableQueue(MutableQueue && rhs)
		: theTrees{ std::move(rhs.theTrees) }, currentSize{ rhs.currentSize }
	{
	}

	~MutableQueue()
	{
		makeEmpty();
	}

	/**
	* Deep copy.
	*/
	MutableQueue & operator=(const MutableQueue & rhs)
	{
		MutableQueue copy = rhs;
		std::swap(*this, copy);
		return *this;
	}

	/**
	* Move.
	*/
	MutableQueue & operator=(MutableQueue && rhs)
	{
		std::swap(currentSize, rhs.currentSize);
		std::swap(theTrees, rhs.theTrees);
		return *this;
	}

	/**
	* Return true if empty; false otherwise.
	*/
	bool isEmpty() const
	{
		return currentSize == 0;
	}

	/**
	* Returns minimum item.
	* Throws UnderflowException if empty.
	*/
	const Comparable & findMin() const
	{
		if (isEmpty())
			throw UnderflowException{};

		return theTrees[findMinIndex()]->element;
	}

    /**************************************************************************
    Modified by Olga K.
    ***************************************************************************/
	/**
	* Insert item x into the priority queue; allows duplicates.
	*/
	void insert(const Comparable & x)
	{
		MutableQueue oneItem{ x };
		this->hashTable.insert(MyPair<Comparable, BinomialNode*>(x, oneItem.theTrees[0]));
		merge(oneItem);
	}

	/**
	* Insert item x into the priority queue; allows duplicates.
	*/
	void insert(Comparable & x)
	{
		MutableQueue oneItem{ std::move(x) };
		this->hashTable.insert(MyPair<Comparable, BinomialNode*>(x, oneItem.theTrees[0]));
		merge(oneItem);
	}

	/**
	* Insert item x into the priority queue; allows duplicates.
	*/
	void newInsert(const Comparable & x)
	{
		BinomialNode* item = new BinomialNode(x,nullptr, nullptr);
		this->hashTable.insert(MyPair<Comparable, BinomialNode *>(x,item));
		item->parent = nullptr;

		BinomialNode* carry = item;
		//this->hashTable.insertTree(item);
		for (unsigned int i = 0; i<this->theTrees.size(); i++)
		{
			if (this->theTrees[i] == nullptr)
			{
				this->theTrees[i] = carry;
				this->currentSize++;
				this->theTrees.resize(this->currentSize);
				break;
			}
			else
			{
				carry = this->combineTrees(this->theTrees[i], carry);
				this->theTrees[i] = nullptr;
			}
		}
		item = nullptr;
		carry = nullptr;
	}

	/**
	* Remove the smallest item from the priority queue.
	* Throws UnderflowException if empty.
	*/
	void deleteMin()
	{
		Comparable x;
		deleteMin(x);
	}

	/**
	* Remove the minimum item and place it in minItem.
	* Throws UnderflowException if empty.
	*/
	void deleteMin(Comparable & minItem)
	{
		if (isEmpty())
			throw UnderflowException{};

		int minIndex = findMinIndex();
		minItem = theTrees[minIndex]->element;

		BinomialNode *oldRoot = theTrees[minIndex];
		BinomialNode *deletedTree = oldRoot->leftChild;
		this->hashTable.remove(minItem);
		delete oldRoot;

		// Construct H''
		MutableQueue deletedQueue;
		deletedQueue.theTrees.resize(minIndex);
		deletedQueue.currentSize = (1 << minIndex) - 1;
		for (int j = minIndex - 1; j >= 0; --j)
		{
			deletedQueue.theTrees[j] = deletedTree;
			deletedTree = deletedTree->nextSibling;
			deletedQueue.theTrees[j]->nextSibling = nullptr;
			deletedQueue.theTrees[j]->parent = nullptr;
		}

		// Construct H'
		theTrees[minIndex] = nullptr;
		currentSize -= deletedQueue.currentSize + 1;

		merge(deletedQueue);
	}

	/**
	* Make the priority queue logically empty.
	*/
	void makeEmpty()
	{
		currentSize = 0;
		for (auto & root : theTrees)
			makeEmpty(root);
	}

	/**
	* Merge rhs into the priority queue.
	* rhs becomes empty. rhs must be different from this.
	* Exercise 6.35 needed to make this operation more efficient.
	*/
	void merge(MutableQueue & rhs)
	{
		if (this == &rhs)    // Avoid aliasing problems
			return;

		currentSize += rhs.currentSize;

		if (currentSize > capacity())
		{
			int oldNumTrees = theTrees.size();
			int newNumTrees = max(theTrees.size(), rhs.theTrees.size()) + 1;
			theTrees.resize(newNumTrees);
			for (int i = oldNumTrees; i < newNumTrees; ++i)
				theTrees[i] = nullptr;
		}

		BinomialNode *carry = nullptr;
		for (int i = 0, j = 1; j <= currentSize; ++i, j *= 2)
		{
			BinomialNode *t1 = theTrees[i];
			BinomialNode *t2 = i < rhs.theTrees.size() ? rhs.theTrees[i] : nullptr;

			int whichCase = t1 == nullptr ? 0 : 1;
			whichCase += t2 == nullptr ? 0 : 2;
			whichCase += carry == nullptr ? 0 : 4;

			switch (whichCase)
			{
			case 0: /* No trees */
			case 1: /* Only this */
				break;
			case 2: /* Only rhs */
				theTrees[i] = t2;
				rhs.theTrees[i] = nullptr;
				break;
			case 4: /* Only carry */
				theTrees[i] = carry;
				carry = nullptr;
				break;
			case 3: /* this and rhs */
				carry = combineTrees(t1, t2);
				theTrees[i] = rhs.theTrees[i] = nullptr;
				break;
			case 5: /* this and carry */
				carry = combineTrees(t1, carry);
				theTrees[i] = nullptr;
				break;
			case 6: /* rhs and carry */
				carry = combineTrees(t2, carry);
				rhs.theTrees[i] = nullptr;
				break;
			case 7: /* All three */
				theTrees[i] = carry;
				carry = combineTrees(t1, t2);
				rhs.theTrees[i] = nullptr;
				break;
			}
		}

		for (auto & root : rhs.theTrees)
			root = nullptr;
		rhs.currentSize = 0;
	}

	/**
	* Returns minimum item.
	* Throws UnderflowException if empty.
	*/
	bool find(Comparable &item) const
	{
		return this->hashTable.contains(MyPair< Comparable, BinomialNode* >(item, nullptr));
	}
	bool remove(const Comparable& key)
	{
		if (hashTable.contains(MyPair< Comparable, BinomialNode* >(key, nullptr)))
		{
			MyPair<Comparable, BinomialNode*> deletePair = this->hashTable.getItem(MyPair< Comparable, BinomialNode* >(key, nullptr));
			if (this->remove(deletePair.data))
			{
				this->hashTable.remove(deletePair);
				return true;
			}
		}
		return false;
	}



private:
	struct BinomialNode
	{
		Comparable    element;
		BinomialNode *leftChild;
		BinomialNode *nextSibling;
		BinomialNode *parent;

		BinomialNode(const Comparable & e, BinomialNode *lt, BinomialNode *rt)
			: element{ e }, leftChild{ lt }, nextSibling{ rt } { }

		BinomialNode(Comparable && e, BinomialNode *lt, BinomialNode *rt)
			: element{ std::move(e) }, leftChild{ lt }, nextSibling{ rt } { }
	};



	const static int DEFAULT_TREES = 1;


	vector<BinomialNode *> theTrees;  // An array of tree roots
	int currentSize;                  // Number of items in the priority queue
	HashTable <MyPair<Comparable,BinomialNode*>> hashTable;


	/**
	* Find index of tree containing the smallest item in the priority queue.
	* The priority queue must not be empty.
	* Return the index of tree containing the smallest item.
	*/
	int findMinIndex() const
	{
		int i;
		int minIndex;

		for (i = 0; theTrees[i] == nullptr; ++i)
			;

		for (minIndex = i; i < theTrees.size(); ++i)
			if (theTrees[i] != nullptr &&
				theTrees[i]->element < theTrees[minIndex]->element)
				minIndex = i;

		return minIndex;
	}

	/**
	* Return the capacity.
	*/
	int capacity() const
	{
		return (1 << theTrees.size()) - 1;
	}

	/**
	* Return the result of merging equal-sized t1 and t2.
	*/
	BinomialNode * combineTrees(BinomialNode *t1, BinomialNode *t2)
	{
		if (t2->element < t1->element)
			return combineTrees(t2, t1);
		t2->nextSibling = t1->leftChild;
		t1->leftChild = t2;
		t1->parent = nullptr;
		t2->parent = t1;
		return t1;
	}

	/**
	* Make a binomial tree logically empty, and free memory.
	*/
	void makeEmpty(BinomialNode * & t)
	{
		if (t != nullptr)
		{
			makeEmpty(t->leftChild);
			makeEmpty(t->nextSibling);
			delete t;
			t = nullptr;
		}
	}

	/**
	* Internal method to clone subtree.
	*/
	BinomialNode * clone(BinomialNode * t) const
	{
		if (t == nullptr)
			return nullptr;
		else
			return new BinomialNode{ t->element, clone(t->leftChild), clone(t->nextSibling) };
	}

	bool remove(BinomialNode* t)
	{
		Comparable removedKey = t->element;
		// Move t up to the root
		while (t->parent != nullptr)
		{
			//Move node to the left
			BinomialNode* tempLeftChild = t->parent->leftChild;
			BinomialNode* tprt = tempLeftChild;
			if (tempLeftChild != t)
			{
				while (tprt->nextSibling != t)
				{
					tprt = tprt->nextSibling;
				}
				if (tprt->nextSibling == t)
				{
					tprt->nextSibling = tempLeftChild;
					BinomialNode* firstNextSibling = tempLeftChild->nextSibling;
					BinomialNode* firstLeftChild = tempLeftChild->leftChild;
					tempLeftChild->nextSibling = t->nextSibling;
					tempLeftChild->leftChild = t->leftChild;
					if (firstNextSibling == t)
						t->nextSibling = tempLeftChild;
					else
						t->nextSibling = firstNextSibling;
					t->leftChild = firstLeftChild;
					this->updateParents(t);
					this->updateParents(tempLeftChild);
					t->parent->leftChild = t;
				}
			}
			//swap parent node with its leftchild
			BinomialNode* tempSibling = t->nextSibling;
			tempLeftChild = t->leftChild;
			t->nextSibling = t->parent->nextSibling;
			t->leftChild = t->parent;
			t->parent->nextSibling = tempSibling;
			t->parent->leftChild = tempLeftChild;
			BinomialNode* tempParent = t->parent;
			t->parent = tempParent->parent;
			tempParent->parent = t;
			this->updateParents(t);

			this->updateParents(t->leftChild);
			//Re-link row of trees
			if (t->parent != nullptr)
			{
				if (t->parent->leftChild == t->leftChild)
				{
					t->parent->leftChild = t;
				}
				else
				{
					BinomialNode* temp = t->parent->leftChild;
					if (temp != t)
					{
						while (temp->nextSibling != t->leftChild)
							temp = temp->nextSibling;
						temp->nextSibling = t;
					}
				}
			}
			tempLeftChild = nullptr;
			tempSibling = nullptr;
		}
		//Find the index of the tree
		int minIndex = -1;
		for (unsigned int i = 0; i<this->theTrees.size(); i++)
		{
			if (this->theTrees[i] == nullptr)
				continue;
			if (this->theTrees[i] == t)
			{
				minIndex = i;
				break;
			}
			if (this->theTrees[i]->element== t->leftChild->element)
			{
				theTrees[i] = t;
				minIndex = i;
				break;
			}
		}
		if (minIndex != -1)
		{
			//this->hashTable.removeTree(t);
			BinomialNode* deletedChild = t->leftChild;
			delete t;
			MutableQueue deletedQueue;
			deletedQueue.currentSize = (1 << minIndex) - 1;
			deletedQueue.theTrees.resize(minIndex+1);
			for (int j = minIndex - 1; j >= 0; j--)
			{
				deletedQueue.theTrees[j] = deletedChild;
				deletedChild = deletedChild->nextSibling;
				deletedQueue.theTrees[j]->nextSibling = nullptr;
				deletedQueue.theTrees[j]->parent = nullptr;
			}

			this->theTrees[minIndex] = nullptr;
			this->currentSize -= deletedQueue.currentSize + 1;
			merge(deletedQueue);
			return true;
		}
		else
		{
			return false;
		}
	}

	void updateParents(BinomialNode* parent)
	{
		BinomialNode* temp = parent->leftChild;
		while (temp != nullptr)
		{
			temp->parent = parent;
			temp = temp->nextSibling;
		}
	}

};

#endif
