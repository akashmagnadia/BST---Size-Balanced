//Author: Akash Magnadia
//Class: CS 251

#ifndef _BST_H
#define _BST_H

#include <iostream>
#include <vector>

using namespace std;

template <typename T>
class bst {

private:
	struct bst_node {
		T      val;
		bst_node* left;
		bst_node* right;
		int leftSize;
		int rightSize;

		bst_node(const T& _val = T{}, bst_node* l = nullptr, bst_node* r = nullptr, int leftSz = 0, int rightSz = 0)
			: val{ _val }, left{ l }, right{ r }, leftSize{ leftSz }, rightSize{ rightSz }
		{ }
	};



public:
	// constructor:  initializes an empty tree
	bst() {
		root = nullptr;
	}

private:
	// helper function which recursively deallocates nodes
	//   in a tree.
	static void delete_nodes(bst_node* r) {
		if (r == nullptr) return;
		delete_nodes(r->left);
		delete_nodes(r->right);
		delete r;
	}

public:
	// destructor
	~bst() {
		delete_nodes(root);
	}


	/////////////////////////////////for debugging purposes ///////////////////////////
public:
	void print2D() {
		// Pass initial space count as 0  
		cout << endl;
		cout << "Printing Actual Tree" << endl;
		print2DUtil(root, 0);
	}

private:
	void print2DUtil(bst_node* root, int space) {
		int COUNT = 10; //distance between levels
		// Base case  
		if (root == NULL)
			return;

		// Increase distance between levels  
		space += COUNT;

		// Process right child first  
		print2DUtil(root->right, space);

		// Print current node after space  
		// count  
		cout << endl;
		for (int i = COUNT; i < space; i++)
			cout << " ";
		cout << root->val << "\n";

		// Process left child  
		print2DUtil(root->left, space);
	}

	/////////////////////////////////for debugging purposes ///////////////////////////

private:

	/**
	 * function:  insert()
	 * desc:      recursive helper function inserting x into
	 *            binary search tree rooted  at r.
	 *
	 * returns:   pointer to root of tree after insertion.
	 *
	 * notes:     if x is already in tree, no modifications are made.
	 */
	static bst_node* _insert(bst_node * r, T & x, bool& success, int& leftSize, int& rightSize, bst_node* &violatingNode) {

		if (r == nullptr) {
			success = true;
			return new bst_node(x, nullptr, nullptr, 0, 0);
		}

		if (r->val == x) {
			success = false;
			return r;
		}
		if (x < r->val) {
			r->left = _insert(r->left, x, success, leftSize, rightSize, violatingNode);
			if (success) {
				r->leftSize++;  //left size increases
				if (isSizeBalanced(r) != nullptr) { //check if current node is violating node
					violatingNode = r;
				}
			}
			return r;
		}
		else { //x > r->val
			r->right = _insert(r->right, x, success, leftSize, rightSize, violatingNode);
			if (success) {
				r->rightSize++; //right size increases
				if (isSizeBalanced(r) != nullptr) { //check if current node is violating node
					violatingNode = r;
				}
			}
			return r;
		}
	}

	//rebalance the tree
	void rebalanceTree(bst_node * &violatingNode) {
		
		vector<T>* newTreeVec = new vector<T>();

		int minVal = _min_node(violatingNode)->val;
		int maxVal = _max_node(violatingNode)->val;

		newTreeVec = extract_range(minVal, maxVal);

		bst* newTree = nullptr;
		newTree = from_sorted_vec(*newTreeVec, newTreeVec->size()); //give vector and get tree

		violatingNode = newTree->root;
	}

	static bst_node* _replace(bst_node* r, T& x, bst_node* fixedNode) {

		if (r == nullptr) {
			return nullptr;
		}
		if (r->val == x) {
			r = fixedNode;
			return fixedNode;
		}

		if (x < r->val) {
			r->left = _replace(r->left, x, fixedNode);
		}
		else {
			r->right = _replace(r->right, x, fixedNode);
		}
		return r;
	}

public:
	/////////////////////////////////for debugging purposes ///////////////////////////
	void checkViolationAndFix() { //main function to be called 
		bst_node* violatingNode = nullptr;
		if (isSizeBalanced(root) != nullptr) {
			violatingNode = root;
		}
		else {
			_checkViolation(root, violatingNode);
		}

		bst_node* problemNode = violatingNode;

		if (violatingNode != nullptr) { //fix if violation detected
			rebalanceTree(violatingNode); //violatingNode gets replaced with new tree
			//this violatingNode is fixed node now

			root = _replace(root, problemNode->val, violatingNode);
		}
	}


private:
	void _checkViolation(bst_node *r, bst_node* &violatingNode) {
		if (r == nullptr) {
			return;
		}

		if (violatingNode != nullptr) {
			return;
		}

		_checkViolation(r->left, violatingNode); //go left

		if (isSizeBalanced(r) != nullptr) { //check if current node is violating node
			violatingNode = r;
		}

		if (violatingNode != nullptr) {
			return;
		}

		_checkViolation(r->right, violatingNode); //go right

		if (isSizeBalanced(r) != nullptr) { //check if current node is violating node
			violatingNode = r;
		}

		if (violatingNode != nullptr) {
			return;
		}
	}
	/////////////////////////////////for debugging purposes ///////////////////////////

private:
	static bst_node* isSizeBalanced(bst_node* t) {
		int max, min;
		if (t != nullptr) {
			if (t->leftSize > t->rightSize) {
				max = t->leftSize;
				min = t->rightSize;
			}
			else {
				max = t->rightSize;
				min = t->leftSize;
			}

			if (!(max <= (2 * min + 1))) { //apply formula
				return t;
			}
			else {
				return nullptr;
			}
		}
		else {
			return nullptr; //if root itself is null shouldn't get here but just in case
		}
	}

public:
	/**
	 * function:  insert
	 * desc:      inserts x into BST given by t.  Note that
	 *            a BST stores a SET -- no duplicates.  Thus,
	 *            if x is already in t when call made, no
	 *            modifications to tree result.
	 *
	 * note:      helper function does most of the work.
	 *
	 */
	bool insert(T & x) {
		bool success;
		int leftSize = 0;
		int rightSize = 0;

		bst_node* violatingNode = nullptr;

		root = _insert(root, x, success, leftSize, rightSize, violatingNode);
		bst_node* problemNode = violatingNode;

		if (violatingNode != nullptr) { //fix if violation detected
			rebalanceTree(violatingNode); //violatingNode gets replaced with new tree
			//this violatingNode is fixed node now

			root = _replace(root, problemNode->val, violatingNode);
		}

		return success;
	}

	/**
	 * function:  contains()
	 * desc:      returns true or false depending on whether x is an
	 *            element of BST (calling object)
	 *
	 */
	bool contains(const T & x) {
		bst_node* p = root;

		while (p != nullptr) {

			if (p->val == x)
				return true;
			if (x < p->val) {
				p = p->left;
			}
			else
				p = p->right;
		}
		return false;
	}

private:
	// returns pointer to node containing
	//   smallest value in tree rooted at r
	static bst_node* _min_node(bst_node * r) {
		if (r == nullptr)
			return nullptr; // should never happen!
		while (r->left != nullptr)
			r = r->left;
		return r;
	}

	// returns pointer to node containing
	//   largest value in tree rooted at r
	static bst_node * _max_node(bst_node * r) {
		if (r == nullptr)
			return nullptr; // should never happen!
		while (r->right != nullptr)
			r = r->right;
		return r;
	}

	// recursive helper function for node removal
	// returns root of resulting tree after removal.
	static bst_node * _remove(bst_node * r, T & x, bool& success, bst_node* &violatingNode) {
		bst_node* tmp;
		bool sanity;

		if (r == nullptr) {
			success = false;
			return nullptr;
		}
		if (r->val == x) {
			success = true;

			if (r->left == nullptr) {
				tmp = r->right;
				delete r;
				return tmp;
			}
			if (r->right == nullptr) {
				tmp = r->left;
				delete r;
				return tmp;
			}
			// if we get here, r has two children
			r->val = _min_node(r->right)->val;
			r->rightSize--; //experimental
			r->right = _remove(r->right, r->val, sanity, violatingNode);
			if (!sanity)
				std::cerr << "ERROR:  remove() failed to delete promoted value?\n";
			return r;
		}
		if (x < r->val) {
			r->left = _remove(r->left, x, success, violatingNode);
			if (success) {
				r->leftSize--;
				if (isSizeBalanced(r) != nullptr) { //check if current node is violating node
					violatingNode = r;
				}
			}
		}
		else {
				r->right = _remove(r->right, x, success, violatingNode);
			if (success) {
				r->rightSize--;
				if (isSizeBalanced(r) != nullptr) { //check if current node is violating node
					violatingNode = r;
				}
			}
		}
		return r;

	}

public:

	bool remove(T & x) {
		bool success;
		bst_node* violatingNode = nullptr;
		

		root = _remove(root, x, success, violatingNode);
		bst_node* problemNode = violatingNode;

		if (violatingNode != nullptr) { //fix if violation detected
			rebalanceTree(violatingNode); //violatingNode gets replaced with new tree
			//this violatingNode is fixed node now

			root = _replace(root, problemNode->val, violatingNode);
		}

		return success;
	}


private:
	// recursive helper function to compute size of
	//   tree rooted at r
	static int _size(bst_node * r) {
		if (r == nullptr) return 0;
		return _size(r->left) + _size(r->right) + 1;
	}

public:
	int size() {
		int size = _size(root);
		return size;
	}

private:

	static int _height(bst_node * r) {
		int l_h, r_h;

		if (r == nullptr) return -1;
		l_h = _height(r->left);
		r_h = _height(r->right);
		return 1 + (l_h > r_h ? l_h : r_h);
	}

public:

	int height() {
		return _height(root);
	}

	bool min(T & answer) {
		if (root == nullptr) {
			return false;
		}
		answer = _min_node(root)->val;
		return true;
	}

	T max() {
		return _max_node(root)->val;
	}

	/******************************************
	 *
	 * "stubs" for assigned TODO functions below
	 *
	 *****************************************/

	 /* TODO
	  * Function:  to_vector
	  * Description:  allocates a vector of type T and populates
	  *               it with the elements of the tree in sorted
	  *               order.  A pointer to the vector is returned.
	  *
	  * runtime:  O(n) where n is the tree size.
	  *
	  */
	std::vector<T>* to_vector() {
		vector <int>* inOrderVector = new vector <T>;

		if (root == nullptr) {
			return inOrderVector; //its just empty vector returned
		}

		inOrder(inOrderVector, root);

		return inOrderVector;
	}

	void inOrder(vector<T>* inOrderVector, bst_node * temp) {
		if (temp == nullptr) {
			return;
		}

		inOrder(inOrderVector, temp->left);
		inOrderVector->push_back(temp->val);
		inOrder(inOrderVector, temp->right);
	}

	/* DONE
	 * Function:  get_ith
	 * Description:  determines the ith smallest element in t and
	 *    "passes it back" to the caller via the reference parameter x.
	 *    i ranges from 1..n where n is the number of elements in the
	 *    tree.
	 *
	 *    If i is outside this range, false is returned.
	 *    Otherwise, true is returned (indicating "success").
	 *
	 * Runtime:  O(h) where h is the tree height
	 */
	bool get_ith(int i, T & x) {
		int n = size();
		if (i < 1 || i > n) {
			return false; //out of range
		}

		int currentIth = -1;

		_get_ith(root, i, x, currentIth);
		return true;
	}

private:
	// recursive helper function that does most of the work
	static void _get_ith(bst_node * t, int i, T & x, int& currentIth) {
		if (currentIth == -1) { //if its the root node
			currentIth = t->leftSize + 1; //guaranted to be correct ith value
		}

		int nextIth = -1;

		if (i == currentIth) { //when the correct ith value is reached
			x = t->val; //store that value in x
			return; //then return back
		}
		else if (i < currentIth) { //aiming to go left
			if (t->left == nullptr) {
				cout << "Error. No left node found" << endl;
				return;
			}
			else {
				if (t->left->right != nullptr) {
					nextIth = currentIth - (t->left->rightSize + 1);
				}
				else if (t->left->left != nullptr) {
					nextIth = currentIth - 1;
				}
				else {
					//cout << "_get_ith When currentIth wjth left: " << currentIth << endl;
					nextIth = currentIth - 1;//shouldn't get here but just in case
				}
			}
			_get_ith(t->left, i, x, nextIth);
		}
		else { //(i > currentIth) //aiming to go right
			if (t->right == nullptr) {
				cout << "Error. No right node found" << endl;
				return;
			}
			else {
				if (t->right->left != nullptr) {
					nextIth = currentIth + t->right->leftSize + 1;
				}
				else if (t->right->right != nullptr) {
					nextIth = currentIth + 1;
				}
				else {
					//cout << "_get_ith When currentIth with right: " << currentIth << endl;
					nextIth = currentIth + 1;//shouldn't get here but just in case
				}
			}
			_get_ith(t->right, i, x, nextIth);
		}
	}

public:
	/*
	 * get_ith_SLOW:  slow version of get_ith which gives the right answer, but
	 *   does so by "walking the entire tree".
	 * might be helpful for debugging?
	 * Private helper below does most of the work
	 */
	bool get_ith_SLOW(int i, T & x) {
		int n = size();
		int sofar = 0;

		if (i < 1 || i > n)
			return false;

		_get_ith_SLOW(root, i, x, sofar);
		return true;
	}

private:
	// recursive helper function that does most of the work
	static void _get_ith_SLOW(bst_node * t, int i, T & x, int& sofar) {
		if (t == nullptr)
			return;
		_get_ith_SLOW(t->left, i, x, sofar);

		if (sofar == i)
			return;
		sofar++;
		if (sofar == i) {
			x = t->val;
			return;
		}
		_get_ith_SLOW(t->right, i, x, sofar);
	}

public:

	/* DONE
	 * Function: position_of
	 * Description:  this is like the inverse of
	 *       get_ith:  given a value x, determine the
	 *       position ("i") where x would appear in a
	 *       sorted list of the elements and return
	 *       the position as an integer.
	 *       If x is not in the tree, -1 is returned.
	 *       Examples:
	 *           if x happens to be the minimum, 1 is returned
	 *           if x happens to be the maximum, n is returned where
	 *               n is the tree size.
	 *
	 *       Notice the following property:  for a bst t with n nodes,
	 *           pick an integer pos:1 <= pos <= n.
	 *           Now consider the following:
	 *
					T x;
					int pos, pos2;
					// set pos to a value in {1..n}
					t.get_ith(pos, x);  // must return true since pos is in {1..n}

					// now let's find the position of x (just retrieved)
					pos2 = t.position_of(x);
					if(pos != pos2) {
						std::cout << "THERE MUST BE A BUG!\n";
					}

				 See how position_of performs the inverse operation of get_ith?
	 *
	 * Return:  -1 if x is not in the tree; otherwise, returns the position where x
	 *          would appear in the sorted sequence of the elements of the tree (a
	 *          value in {1..n}
	 *
	 * Runtime:  O(h) where h is the tree height
	 */
	int position_of(const T & x) {
		int targetIth = -1;
		_position_of(root, x, targetIth);
		return targetIth;
	}

private:
	static void _position_of(bst_node * t, const T & x, int& currentIth) {
		if (currentIth == -1) { //if its the root node
			currentIth = t->leftSize + 1; //guaranted to be correct ith value
		}

		int nextIth = -1;

		if (x == t->val) { //when the target value is found
			//preserves the currentIth
			return;
		}
		else if (x < t->val) { //when the target value is left of current node
			if (t->left == nullptr) {
				currentIth = -1; //value is not in the tree
				return;
			}
			else {
				if (t->left->right != nullptr) {
					nextIth = currentIth - (t->left->rightSize + 1);
				}
				else if (t->left->left != nullptr) {
					nextIth = currentIth - 1;
				}
				else { //if t->left is the dead end
					nextIth = currentIth - 1;
				}
			}
			_position_of(t->left, x, nextIth);
			currentIth = nextIth; //return the change
		}
		else { //x > t->val //when the target value is right of current node
			if (t->right == nullptr) {
				currentIth = -1; //value is not in the tree
				return;
			}
			else {
				if (t->right->left != nullptr) {
					nextIth = currentIth + t->right->leftSize + 1;
				}
				else if (t->right->right != nullptr) {
					nextIth = currentIth + 1;
				}
				else { //if t->right is the dead end
					nextIth = currentIth + 1;
				}
			}
			_position_of(t->right, x, nextIth);
			currentIth = nextIth; //return the change
		}
	}

public:
	/* DONE
	 * Function:  num_geq
	 * Description:  returns the number of elements in tree which are
	 *       greater than or equal to x.
	 *
	 * Runtime:  O(h) where h is the tree height
	 */

	int num_geq(const T & x) {
		return _num_geq(root, x);  // placeholder
	}

private:
	int _num_geq(bst_node * r, const T & x) {
		if (r == nullptr) {
			return 0;
		}

		if (x > r->val) { //target value is smaller than current value
			return _num_geq(r->right, x); //go right
		}
		else if (x < r->val) { //target value is bigger than current value
			return 1 + r->rightSize + _num_geq(r->left, x); //go left
		}
		else { //if x is found
			return r->rightSize + 1;
		}
	}

public:
	/*
	 * function:     num_geq_SLOW
	 * description:  same functionality as num_geq but sloooow (linear time)
	 *                (private helper does most of the work)
	 *
	 */
	int num_geq_SLOW(const T & x) {
		return _num_geq_SLOW(root, x);
	}

private:
	static int _num_geq_SLOW(bst_node * t, const T & x) {
		int total;

		if (t == nullptr) return 0;
		total = _num_geq_SLOW(t->left, x) + _num_geq_SLOW(t->right, x);

		if (t->val >= x)
			total++;
		return total;
	}

public:

	/* DONE
	 * Function:  num_leq
	 * Description:  returns the number of elements in tree which are less
	 *      than or equal to x.
	 *
	 * Runtime:  O(h) where h is the tree height
	 *
	 **/
	int num_leq(const T & x) {
		return _num_leq(root, x);
	}

private:
	//helper function for num_leq
	int _num_leq(bst_node * r, int x) {
		if (r == nullptr) {
			return 0;
		}

		if (x > r->val) { //target value is bigger than current value
			return 1 + r->leftSize + _num_leq(r->right, x); //go right
		}
		else if (x < r->val) { //target value is smaller than current value
			return _num_leq(r->left, x); //go left
		}
		else { //if x is found
			return r->leftSize + 1;
		}
	}


public:
	/*
	 * function:     num_leq_SLOW
	 * description:  same functionality as num_leq but sloooow (linear time)
	 *               (private helper _num_leq_SLOW does most of the work)
	 */
	int num_leq_SLOW(const T & x) {
		return _num_leq_SLOW(root, x);
	}
private:

	// helper for num_leq_SLOW
	static int _num_leq_SLOW(bst_node * t, const T & x) {
		int total;

		if (t == nullptr) return 0;
		total = _num_leq_SLOW(t->left, x) + _num_leq_SLOW(t->right, x);

		if (t->val <= x)
			total++;
		return total;
	}

public:

	/* DONE
	 * Function:  num_range
	 * Description:  returns the number of elements in tree which are
	 *       between min and max (inclusive).
	 *
	 * Runtime:  O(h) where h is the tree height
	 *
	 **/
	int num_range(const T & min, const T & max) {
		if (root == nullptr) {
			return 0;
		}

		int size = root->leftSize + root->rightSize + 1;
		return (size - (num_leq_modified(min) + num_geq_modified(max)));
	}

private:
	//helper function
	int num_geq_modified(const T & x) {
		return _num_geq_modified(root, x);  // placeholder
	}

	int num_leq_modified(const T & x) {
		return _num_leq_modified(root, x);  // placeholder
	}

	int _num_geq_modified(bst_node * r, const T & x) {
		if (r == nullptr) {
			return 0;
		}

		if (x > r->val) { //target value is smaller than current value
			return _num_geq_modified(r->right, x); //go right
		}
		else if (x < r->val) { //target value is bigger than current value
			return 1 + r->rightSize + _num_geq_modified(r->left, x); //go left
		}
		else { //if x is found
			return r->rightSize; //subtracted 1
		}
	}

	int _num_leq_modified(bst_node * r, int x) {
		if (r == nullptr) {
			return 0;
		}

		if (x > r->val) { //target value is bigger than current value
			return 1 + r->leftSize + _num_leq_modified(r->right, x); //go right
		}
		else if (x < r->val) { //target value is smaller than current value
			return _num_leq_modified(r->left, x); //go left
		}
		else { //if x is found
			return r->leftSize; //subtracted 1
		}
	}

public:
	/*
	 * function:     num_range_SLOW
	 * description:  same functionality as num_range but sloooow (linear time)
	 *               (private helper _num_range_SLOW does most of the work)
	 *
	 */
	int num_range_SLOW(const T & min, const T & max) {
		return _num_range_SLOW(root, min, max);
	}

private:
	static int _num_range_SLOW(bst_node * t, const T & min, const T & max) {
		int total;

		if (t == nullptr) return 0;
		total = _num_range_SLOW(t->left, min, max) +
			_num_range_SLOW(t->right, min, max);

		if (t->val >= min && t->val <= max)
			total++;
		return total;
	}



	///////////Experimental////////////////
public:
	std::vector<T>* mod_extract_range(const T& min, const T& max) {
		vector <int>* extractVector = new vector <T>;
		_mod_extract_range(root, min, max, extractVector);
		return extractVector;
	}

private:
	static void _mod_extract_range(bst_node* r, const T& min, const T& max, vector<T>* extractVector) {
		if (r == nullptr) {
			return;
		}

		if (min < r->val) {
			_mod_extract_range(r->left, min, max, extractVector);
		}
		extractVector->push_back(r->val);
		if (min > r->val) {
			_mod_extract_range(r->right, min, max, extractVector);
		}
	}
	///////////Experimental////////////////



public:

	/*
	 * TODO
	 * function:     extract_range
	 * Description:  allocates a vector of element type T
	 *               and populates it with the tree elements
	 *               between min and max (inclusive) in order.
	 *               A pointer to the allocated and populated
	 *               is returned.
	 *
	 * notes/comments:  even if the specified range is empty, a
	 *                  vector is still allocated and returned;
	 *                  that vector just happens to be empty.
	 *                  (The function NEVER returns nullptr).
	 *
	 * runtime:  the runtime requirement is "output dependent".
	 *           Let k be the number of elements in the specified range
	 *           (and so the length of the resulting vector) and let h
	 *           be the height of the tree.  The runtime must be:
	 *
	 *                  O(h + k)
	 *
	 *           So...while k can be as large as n, it can be as small
	 *           as zero.
	 *
	 */
	std::vector<T> * extract_range(const T & min, const T & max) {
		vector <int>* extractVector = new vector <T>;
		bool pushing = false;
		bool overBoard = false;

		bst_node* tempRoot = new bst_node;
		tempRoot = root;
		while (!((min <= tempRoot->val) && (max >= tempRoot->val))) {
			if (min < tempRoot->val) {
				tempRoot=tempRoot->left;
			}
			else {
				tempRoot=tempRoot->right;
			}
		}

		if (tempRoot->left != nullptr) {
			_extract_range(tempRoot->left, min, max, extractVector, pushing, overBoard);
		}
		
		extractVector->push_back(tempRoot->val);
		overBoard = false;

		if (tempRoot->right != nullptr) {
			_extract_range(tempRoot->right, min, max, extractVector, pushing, overBoard);
		}

		return extractVector;
	}

private:
	static void _extract_range(bst_node * r, const T & min, const T & max, vector<T> * extractVector, bool& pushing, bool& overBoard) {
		if (r->val >= max) { //if max is reached
			if (r->left != nullptr) {
				if (r->left->val <= max) {
					overBoard = true;
				}
				_extract_range(r->left, min, max, extractVector, pushing, overBoard);
			}
			if (r->val == max) {
				extractVector->push_back(r->val);
			}
			pushing = true;
			return;
		}


		if (overBoard) {
			if ((r->left == nullptr) && (r->right == nullptr)) { //reach dead end node
				if ((r->val <= max) && (r->val >= min)) {
					extractVector->push_back(r->val);
				}
				return;
			}

			if (r->left != nullptr) {
				_extract_range(r->left, min, max, extractVector, pushing, overBoard);
				if ((r->val <= max) && (r->val >= min)) {
					extractVector->push_back(r->val);
				}
			}

			if (r->right != nullptr) {
				if (extractVector->empty()) { //if vector is empty
					if ((r->val <= max) && (r->val >= min)) {
						extractVector->push_back(r->val);
					}
				}
				else {
					if ((extractVector->back()) != (r->val)) {
						if ((r->val <= max) && (r->val >= min)) {
							extractVector->push_back(r->val);
						}
					}
				}
				_extract_range(r->right, min, max, extractVector, pushing, overBoard);
			}

			return;
		}
		else {
			//go left
			if (r->left != nullptr) {
				_extract_range(r->left, min, max, extractVector, pushing, overBoard);
			}

			//experminetal
			if (extractVector->empty()) { //if vector is empty
				if ((r->val <= max) && (r->val >= min)) {
					extractVector->push_back(r->val);
				}
			}
			else {
				if ((extractVector->back()) != (r->val)) {
					if ((r->val <= max) && (r->val >= min)) {
						extractVector->push_back(r->val);
					}
				}
			}
			

			if (r->right != nullptr) {
				_extract_range(r->right, min, max, extractVector, pushing, overBoard);
			}
		}
	}

	/***************************************************
	*  PREORDER AND INORDER TREE TRAVERSALS BELOW      *
	*                                                  *
	***************************************************/

private:
	static void indent(int m) {
		int i;
		for (i = 0; i < m; i++)
			std::cout << "-";
	}

	static void _inorder(bst_node * r) {
		if (r == nullptr) return;
		_inorder(r->left);
		std::cout << "[ " << r->val << " ]\n";
		_inorder(r->right);
	}

	static void _preorder(bst_node * r, int margin) {
		if (r == nullptr) {
			indent(margin);
			std::cout << " nullptr \n";
		}
		else {
			indent(margin);
			std::cout << "[ " << r->val << " ]\n";
			_preorder(r->left, margin + 3);
			_preorder(r->right, margin + 3);
		}
	}

public:
	void inorder() {
		std::cout << "\n======== BEGIN INORDER ============\n";
		_inorder(root);
		std::cout << "\n========  END INORDER  ============\n";
	}


	void preorder() {

		std::cout << "\n======== BEGIN PREORDER ============\n";
		_preorder(root, 0);
		std::cout << "\n========  END PREORDER  ============\n";

	}

	/***************************************************
	*  END PREORDER AND INORDER TREE TRAVERSALS        *
	*                                                  *
	***************************************************/

private:
	/*
	 * Recursive  helper function _from_vec, used by
	 * from_sorted_arr(...). The function must return a sub-tree that is
	 * perfectly balanced, given a sorted array of elements a.
	 */
	static bst_node* _from_vec(const std::vector<T> & a, int low, int hi, bool &success, int &leftSize, int &rightSize) {
		int m;
		bst_node* root;

		if (hi < low) {
			success = false;
			return nullptr;
		}
		success = true;
		m = (low + hi) / 2;
		root = new bst_node(a[m], nullptr, nullptr, 0, 0);
		
		
		root->left = _from_vec(a, low, m - 1,success, leftSize, rightSize);
		if (root->left != nullptr) {
			root->leftSize = root->left->leftSize + root->left->rightSize + 1;
		}

		root->right = _from_vec(a, m + 1, hi,success, leftSize, rightSize);
		if (root->right != nullptr) {
			root->rightSize = root->right->leftSize + root->right->rightSize + 1;
		}
		return root;
	}

public:
	static bst * from_sorted_vec(const std::vector<T> & a, int n) {

		bst* t = new bst();
		bool success;
		int leftSize = 0;
		int rightSize = 0;
		t->root = _from_vec(a, 0, n - 1, success, leftSize, rightSize);
		return t;
	}

private:

	/*
	 * DO NOT MODIFY THE TWO FUNCTIONS BELOW!!!
	 */
	static void _to_preorder_vec(bst_node * t, std::vector<T> & vec) {
		if (t == nullptr) return;
		vec.push_back(t->val);
		_to_preorder_vec(t->left, vec);
		_to_preorder_vec(t->right, vec);
	}
	std::vector<T>* to_preorder_vec() {
		std::vector<T>* vec = new std::vector<T>();
		_to_preorder_vec(root, *vec);
		return vec;
	}

	/*
	 * Finally, the data members of the bst class
	 */
private:
	bst_node* root;


}; // end class bst

#endif