#include <iostream>
#include "SuperStructure.h"
using namespace std;

int main()
{
	Superstructure ss;
	int val;

	/*for (int i = 1; i < 14; i++)
		ss.add(i, i);

	ss.printAVLTree();
	ss.printTable();
	ss.printForward();*/

	while (cout << "Enter value (0 to Stop): ", cin >> val, val)
	{
		ss.add(val, val);

		cout << endl;
		ss.printAVLTree();
	}

	while (cout << "Enter value (0 to Stop): ", cin >> val, val)
	{
		ss.remove(val);

		cout << endl;
		ss.printAVLTree();
	}

	while (cout << "Enter value (0 to Stop): ", cin >> val, val)
	{
		ss.add(val, val);

		cout << endl;
		ss.printAVLTree();
	}

	ss.printForward();
	ss.printTable();
	ss.printAVLTree();
	ss.printAscending();
}