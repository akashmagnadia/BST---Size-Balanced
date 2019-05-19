//Author: Akash Magnadia
//Class: CS 251

#include <iostream>
#include "pch.h"
#include <array>
#include <stdio.h>
#include<stdlib.h>


using namespace std;

int main() {
	int x;
	int h;
	bool printExtra = false;
	int ithValue = 0;
	bool getSuccess;

	bst<int> *t = new bst<int>();

	while ((cin >> x))
		t->insert(x);

	/*cout << "Before" << endl;
	t->print2D();
	t->checkViolationAndFix();
	cout << "After" << endl;
	t->print2D();*/

	if (t->size() <= 20) {
		//t->inorder();
		//t->preorder();
	}

	h = t->height();
	cout << "\n#### Reported height of tree:   " << h << "\n";


	//print the values of the tree
	for (int i = 1; i < t->size() + 1; i++) {
		getSuccess = t->get_ith(i, ithValue);
		if (getSuccess) {
			cout << "Value at " << i << " is " << ithValue << endl;
		}
		else {
			cout << i << " is not in the tree" << endl;
		}
	}

	cout << endl;
	int removeNum = 51;
	cout << "removing " << removeNum << endl;
	
	cout << "Before" << endl;
	t->print2D();

	t->remove(removeNum);
	cout << "After" << endl;
	t->print2D();

	

	//checking get_ith
	for (int i = 0; i < t->size(); i++) {
		int ithValue1, ithValue2;
		bool success1, success2;
		success1 = t->get_ith(i, ithValue1);
		success2 = t->get_ith_SLOW(i, ithValue2);
		if ((ithValue1 != ithValue2) || (success1 != success2)) {
			cout << "At get_ith ";
			cout << "THERE MUST BE A BUG!";
			for (int j = 0; j < 10; j++) {
				cout << " BUG! ";
			}
			cout << endl;
		}
	}


	if (printExtra) {
		cout << "Calling get_ith_SLOW to get each ith value" << endl;
	}

	for (int i = 1; i < t->size() + 1; i++) {
		getSuccess = t->get_ith_SLOW(i, ithValue);
		if (getSuccess) {
			cout << "Value at " << i << " is " << ithValue << endl;
		}
		else {
			cout << i << " is not in the tree" << endl;
		}
	}

	cout << endl;

	if (printExtra) {
		cout << endl;
		cout << "Calling get_ith to get each ith value" << endl;
	}

	for (int i = 1; i < t->size() + 1; i++) {
		getSuccess = t->get_ith(i, ithValue);
		if (getSuccess) {
			cout << "Value at " << i << " is " << ithValue << endl;
		}
		else {
			cout << i << " is not in the tree" << endl;
		}


		// cross  checking get_ith and position_of
		int pos1 = i;
		t->get_ith(pos1, ithValue);
		int pos2 = t->position_of(ithValue);
		if (pos1 != pos2) {
			cout << "At get_ith or pos_ith ";
			cout << "THERE MUST BE A BUG!";
			for (int j = 0; j < 10; j++) {
				cout << " BUG! ";
			}
			cout << endl;
		}
	}
	int testNum = 1000;
	getSuccess = t->get_ith(testNum, ithValue);
	if (getSuccess) {
		cout << "Value at " << testNum << " is " << ithValue << endl;
	}
	else {
		cout << testNum << " is not in the tree" << endl;
	}

	cout << endl;
	if (printExtra) {
		cout << "Calling position_of to get ith of a position" << endl;
	}

	array<int, 4> tempArr{ 22, 11, 88, 77 };
	int tempArrSize = tempArr.size();
	for (int i = 0; i < tempArrSize; i++) {
		cout << "Position of " << tempArr[i];
		int position = t->position_of(tempArr[i]);
		if (position == -1) {
			cout << " doesn't exist" << endl;
		}
		else {
			cout << " is " << position << endl;
		}
	}


	int num1 = 22;
	int num2 = 76;

	cout << endl;
	if (printExtra) {
		cout << "Calling num_leq_SLOW to get # of values less than or equal to " << num2 << endl;
	}
	cout << "# of values less than or equal to " << num2 << " : " << t->num_leq_SLOW(num2) << endl;

	if (printExtra) {
		cout << endl << "Calling num_leq to get # of values less than or equal to " << num2 << endl;
		cout << "# of values less than or equal to " << num2 << " : " << t->num_leq(num2) << endl;
	}


	//testing num_leq
	if (t->num_leq_SLOW(num2) != t->num_leq(num2)) {
		cout << "At num_leq ";
		cout << "THERE MUST BE A BUG!";
		for (int j = 0; j < 10; j++) {
			cout << " BUG! ";
		}
		cout << endl;
	}

	if (printExtra) {
		cout << endl << "Calling num_geq_SLOW to get # of values more than or equal to " << num1 << endl;
	}
	cout << "# of values more than or equal to " << num1 << " : " << t->num_geq_SLOW(num1) << endl;

	if (printExtra) {
		cout << endl << "Calling num_geq to get # of values more than or equal to " << num1 << endl;
		cout << "# of values more than or equal to " << num1 << " : " << t->num_geq(num1) << endl;
	}


	//testing num_geq
	if (t->num_geq_SLOW(num2) != t->num_geq(num2)) {
		cout << "At num_geq ";
		cout << "THERE MUST BE A BUG!";
		for (int j = 0; j < 10; j++) {
			cout << " BUG! ";
		}
		cout << endl;
	}

	int range;
	cout << endl;
	if (printExtra) {
		cout << "Calling num_range_SLOW to get range" << endl;
	}

	range = t->num_range_SLOW(num1, num2);
	cout << "Range between " << num1 << " and " << num2 << " is " << range << endl;

	if (printExtra) {
		cout << endl << "Calling num_range to get range" << endl;
		range = t->num_range(num1, num2);
		cout << "Range between " << num1 << " and " << num2 << " is " << range << endl;
	}
	cout << endl;

	//testing num_range
	bool printRange = false;
	int extraRange1 = 0;
	int extraRange2 = 1000;
	int range1, range2, ithValue1, ithValue2;
	bool extraRange = false;
	for (int i = 1; i < t->size() + 1; i++) {
		t->get_ith_SLOW(i, ithValue1);
		range1 = t->num_range_SLOW(0, ithValue1);
		range2 = t->num_range(0, ithValue1);
		if (printRange) {
			cout << "Range between 0 and " << ithValue1 << " range1: " << range1 << " range2: " << range2 << endl;
		}
		if (range1 != range2) {
			cout << "At num_range ";
			cout << "THERE MUST BE A BUG!";
			for (int j = 0; j < 10; j++) {
				cout << " BUG! ";
			}
			cout << endl;
		}
	}
	for (int i = 1; i < t->size() + 1; i++) {
		t->get_ith_SLOW(i, ithValue1);
		range1 = t->num_range_SLOW(ithValue1, 1000);
		range2 = t->num_range(ithValue1, 1000);
		if (printRange) {
			cout << "Range between 1000 and " << ithValue1 << " range1: " << range1 << " range2: " << range2 << endl;
		}
		if (range1 != range2) {
			cout << "At num_range ";
			cout << "THERE MUST BE A BUG!";
			for (int j = 0; j < 10; j++) {
				cout << " BUG! ";
			}
			cout << endl;
		}
	}
	for (int i = 1; i < t->size() + 1; i++) {
		for (int j = i; j < t->size() + 1; j++) {
			t->get_ith_SLOW(i, ithValue1); //lower value
			t->get_ith_SLOW(j, ithValue2); //higher value
			range1 = t->num_range_SLOW(ithValue1, ithValue2); //min, max
			range2 = t->num_range(ithValue1, ithValue2);

			if (printRange) {
				cout << "Range between " << ithValue1 << " and " << ithValue2 << " range1: " << range1 << " range2: " << range2 << endl;
			}
			if (range1 != range2) {
				cout << "At num_range ";
				cout << "THERE MUST BE A BUG!";
				for (int j = 0; j < 10; j++) {
					cout << " BUG! ";
				}
				cout << endl;
			}
		}
	}

	int min = 12;
	int max = 79;

	//t->print2D();

	vector<int>* temp = t->extract_range(min, max);

	cout << "Printing vector between " << min << " and " << max << endl;
	int tempSize = temp->size();
	for (int i = 0; i < tempSize; i++) {
		cout << temp->at(i) << "  ";
	}
	cout << endl;

	//add vector checking with finding ith of min and max and then cross checking

	cout << "Printing a vector made from tree" << endl;
	vector<int>* treeToVec = t->to_vector();
	int treeToVecSize = treeToVec->size();
	for (int i = 0; i < treeToVecSize; i++) {
		cout << i + 1 << ": " << treeToVec->at(i) << endl;
	}
	

	//making a new tree from vector using from_sorted_vec
	//use it to reattach
	cout << endl << "Making a new tree from range vector" << endl;

	bst<int>* newTree = new bst<int>();
	int tempNum = 0;

	newTree = t->from_sorted_vec(*treeToVec, treeToVec->size());

	for (int i = 1; i < newTree->size() + 1; i++) {
		if (newTree->get_ith_SLOW(i, tempNum)) {
			cout << i << ": " << tempNum << endl;
		}
		else {
			cout << i << ": doesn't exist" << endl;
		}
	}

	//newTree->print2D();

	delete t;
	delete temp;

	return 0;
}