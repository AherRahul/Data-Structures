/********************************************************/
/*	Project:		Superstructure		*/
/*	File:			SuperStructure.h	*/
/*	Description:	Superstructure implementation	*/
/*	Author:			Viren Lakum (VL)	*/
/********************************************************/

#include <iostream>
using namespace std;

struct Entry
{
	//Fields for Hashtable Entry
	int key;
	int value;
	Entry* nextEntry;

	//Fields for Double-Linked List
	Entry* next;
	Entry* prev;

	//Fields for node of a tree
	Entry* left;
	Entry* right;

	//parametrized constructor
	Entry(int val, int k)
	{
		value = val;
		key = k;

		nextEntry = nullptr;
		next = nullptr;
		prev = nullptr;
		left = nullptr;
		right = nullptr;
	}
};

class Superstructure
{
	//Fields for HashTable
	int size;
	Entry** table;

	//Fields for Double-Linked List
	Entry* head;
	Entry* tail;

	//Fields for AVL Tree
	Entry* root;

	//Private print tree function for debugging
	void printTree(Entry* node)
	{
		static int level = 0;
		if (node)
		{
			level++;
			printTree(node->right);

			cout << "\t";
			for (int i = 0; i < level; i++)
				cout << "   ";
			cout << node->value << endl;

			printTree(node->left);
			level--;
		}
	}

	//print ascending function
	void printAsc(Entry* node)
	{
		if (node)
		{
			printAsc(node->left);
			cout << node->value << " ";
			printAsc(node->right);
		}
	}

	//Function to add a node in Tree
	Entry* addNode(Entry* node, Entry* newEntry)
	{
		if (!node)
		{
			return newEntry;
		}

		if (newEntry->value > node->value)
			node->right = addNode(node->right, newEntry);

		else if (newEntry->value < node->value)
			node->left = addNode(node->left, newEntry);

		node = rebalance(node);
		return node;
	}

	//function that rebalances the tree
	Entry* rebalance(Entry* node)
	{
		int balance = getBalance(node);

		if (2 == balance)
		{
			int childBalance = getBalance(node->right);

			if (1 == childBalance)
			{
				//cout << endl << "Rotated Left: " << node->value << endl;
				node = rotateLeft(node);
			}
			else
			{
				//cout << endl << "Rotate Right-Left: " << node->value << endl;
				node->right = rotateRight(node->right);
				node = rotateLeft(node);
			}
		}
		else if (-2 == balance)
		{
			int childBalance = getBalance(node->left);

			if (-1 == childBalance)
			{
				//cout << endl << "Rotate Right: " << node->value << endl;
				node = rotateRight(node);
			}
			else
			{
				//cout << endl << "Rotate Left-Right: " << node->value << endl;
				node->left = rotateLeft(node->left);
				node = rotateRight(node);
			}
		}
		return node;
	}

	//implement a right rotation
	Entry* rotateRight(Entry* node)
	{
		Entry* child = node->left;
		node->left = child->right;
		child->right = node;
		return child;
	}

	//implement a left rotation
	Entry* rotateLeft(Entry* node)
	{
		Entry* child = node->right;
		node->right = child->left;
		child->left = node;
		return child;
	}

	//check the balance of the given node by find difference in levels on both side
	int getBalance(Entry* node)
	{
		int rightHeight = getHeight(node->right);
		int leftHeight = getHeight(node->left);

		return (rightHeight - leftHeight);
	}

	//returns the max level(height) of a given node
	int getHeight(Entry* node)
	{
		if (!node)
			return 0;

		int rightHeight = getHeight(node->right);
		int leftHeight = getHeight(node->left);

		return rightHeight > leftHeight ? rightHeight + 1 : leftHeight + 1;
	}

	Entry* removeNode(Entry* node, int val)
	{
		if (val > node->value)
		{
			node->right = removeNode(node->right, val);
		}
		else if (val < node->value)
		{
			node->left = removeNode(node->left, val);
		}
		else
		{
			//node to be removed has no child
			if (!node->left && !node->right)
			{
				return nullptr;
			}

			//if node to be removed has only one child at right
			if (!node->left && node->right)
			{
				Entry* orphan = node->right;
				return orphan;
			}

			//if node to be removed has only one child at left
			if (node->left && !node->right)
			{
				Entry* orphan = node->left;
				return orphan;
			}

			//node has 2 children
			//so search for successor
			Entry* successorParent = node;
			Entry* successor = node->right;

			while (successor->left)
			{
				successorParent = successor;
				successor = successor->left;
			}

			//if is not child of node to be removed
			//assign node to be removed's children to successor
			//if successor has a child pass it successors parent
			if (successor != node->right)
			{
				successorParent->left = successor->right;
				successor->right = node->right;
				successor->left = node->left;
				return successor;
			}
			//if successor is the immediate child of 
			//node to be removed
			//assign nodes left child to successor's left
			//and return successor
			else
			{
				successor->left = node->left;
				return successor;
			}
		}
		return node;
	}

public:
	//Constructor
	Superstructure(int s = 10)
	{
		table = new Entry * [size = s];
		for (int i = 0; i < size; i++)
			table[i] = nullptr;

		head = nullptr;
		tail = nullptr;

		root = nullptr;
	}

	//Hash function to calculate offset(index) for table
	int hash(int k)
	{
		return k % size;
	}


	//function to add an entry in superStructure
	bool add(int val, int k)
	{
		Entry* newEntry = new Entry(val, k);

		if (!newEntry)
			return false;

		// add new entry in hashTable    
		int offset = hash(k);

		if (!table[offset])
		{
			table[offset] = newEntry;
		}
		else
		{
			//check if key already exists
			for (Entry* current = table[offset]; current; current = current->nextEntry)
			{
				//return false if key exists
				if (current->key == k)
					return false;
			}

			newEntry->nextEntry = table[offset];
			table[offset] = newEntry;
		}

		// add new entry in List
		if (!head)
		{
			head = newEntry;
			tail = newEntry;
		}
		else
		{
			tail->next = newEntry;
			newEntry->prev = tail;
			tail = newEntry;
		}

		//add new entry in AVL tree
		root = addNode(root, newEntry);
	}

	//print double linked list from head to tail
	void printForward()
	{
		cout << endl << "Printing Sequence: ";
		for (Entry* current = head; current; current = current->next)
		{
			if (current != tail)
				cout << current->value << " -> ";
			else
				cout << current->value << endl << endl;
		}
	}

	//print hashTable for debugging
	void printTable()
	{
		for (int i = 0; i < size; i++)
		{
			cout << i << ": ";

			for (Entry* current = table[i]; current; current = current->nextEntry)
			{
				cout << "[" << current->key << " : " << current->value << "]";
			}
			cout << endl;
		}
	}

	//removes an entry whose key matches with the given key
	bool remove(int key)
	{
		int offset = hash(key);

		Entry* prev = nullptr;
		for (Entry* current = table[offset]; current; prev = current, current = current->nextEntry)
		{
			//if the key if found remove the entry
			if (current->key == key)
			{
				//remove for hashtable
				if (prev == nullptr)
				{
					table[offset] = current->nextEntry;
				}
				else
				{
					prev->nextEntry = current->nextEntry;
				}

				//remove for list
				if (head == tail)
				{
					head = tail = nullptr;
				}
				else if (current == head)
				{
					current->next->prev = nullptr;
					head = current->next;
				}
				else if (current == tail)
				{
					current->prev->next = nullptr;
					tail = current->prev;
				}
				else
				{
					current->prev->next = current->next;
					current->next->prev = current->prev;
				}

				//remove for tree
				root = removeNode(root, current->value);
				delete current;

				if (root)
					root = rebalance(root);

				return true;
			}
		}

		return false;
	}

	//Data abstractor for printTree function
	void printAVLTree()
	{
		if (root)
			printTree(root);
	}

	//Prints values in ascending order
	void printAscending()
	{
		if (root)
		{
			cout << endl << "Ascending Order: ";
			printAsc(root);
			cout << endl << endl;
		}
	}
};
