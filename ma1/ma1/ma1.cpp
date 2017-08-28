#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
using namespace std;

template <typename T>
class BinaryNode
{
private:

	//underlying value in our node
	T _value;

	//a pointer to our left child
	BinaryNode<T> *_left_child;

	//a pointer to our right child
	BinaryNode<T> *_right_child;

	//disallow copy constructors
	BinaryNode(const BinaryNode<T> &other);
	BinaryNode(BinaryNode<T> &&other);

	//disallow assignment operator
	BinaryNode<T> &operator=(const BinaryNode<T> &other);
	BinaryNode<T> &operator=(BinaryNode<T> &&other);

protected:

public:

	//default constructor
	BinaryNode()
	{
		_left_child = nullptr;
		_right_child = nullptr;
	}

	BinaryNode(T value)
	{
		_left_child = nullptr;
		_right_child = nullptr;
		_value = value;
	}

	//constructor that takes in two binary nodes to set as left and right
	BinaryNode(BinaryNode<T> *left, BinaryNode<T> *right)
	{
		_left_child = left;
		_right_child = right;
	}

	virtual bool isLeaf()
	{
		return _left_child == nullptr && _right_child == nullptr;
	}

	BinaryNode<T> *getLeftChild()
	{
		return _left_child;
	}

	const BinaryNode<T> *getLeftChild() const
	{
		return _left_child;
	}

	void setLeftChild(BinaryNode<T> *left)
	{
		_left_child = left;
	}

	BinaryNode<T> *getRightChild()
	{
		return _right_child;
	}

	const BinaryNode<T> *getRightChild() const
	{
		return _right_child;
	}

	void setRightChild(BinaryNode<T> *right)
	{
		_right_child = right;
	}

	void setValue(T value)
	{
		_value = value;
	}

	T &getValue()
	{
		return _value;
	}

	const T &getValue() const
	{
		return _value;
	}
};

template <typename T>
class AvlNode : public BinaryNode<T>
{
private:
	int _height = 0;

public:

	//carry over constructors from parent
	AvlNode() : BinaryNode<T>()
	{
	}

	AvlNode(T value) : BinaryNode<T>(value)
	{
	}

	AvlNode(AvlNode<T> *left, AvlNode<T> *right) : BinaryNode<T>(left, right)
	{
	}

	int getHeight() const
	{
		return _height;
	}

	void setHeight(const T& height)
	{
		_height = height;
	}

	int getBalanceFactor()
	{
		AvlNode<T> *left = dynamic_cast<AvlNode<T> *>(BinaryNode<T>::getLeftChild());
		AvlNode<T> *right = dynamic_cast<AvlNode<T> *>(BinaryNode<T>::getRightChild());

		int left_height = (left == nullptr) ? -1 : left->getHeight();
		int right_height = (right == nullptr) ? -1 : right->getHeight();

		return right_height - left_height;
	}
};

template <typename T>
class BinarySearchTree
{
private:

	//keeps track of the remove direction (left or right)
	unsigned int _remove_counter = 0;

	//keeps track of BST size
	unsigned int _size_counter = 0;

#pragma region protected methods

protected:
	BinaryNode<T> *_root = nullptr;

	//remotes the largest element from the subtree starting at
	//*root
	BinaryNode<T> * removeLargest(BinaryNode<T> *root)
	{
		//ASSUMPTION: non-null root
		if (root == nullptr)
		{
			return root;
		}

		//BASE CASE: root is largest (has no right node)
		if (root->getRightChild() == nullptr)
		{
			BinaryNode<T> *left = root->getLeftChild();
			delete root;

			//return left information to parent
			return left;
		}
		else
		{
			//RECURSIVE CASE:
			BinaryNode<T> *new_right = removeLargest(root->getRightChild());
			root->setRightChild(new_right);
			return root;
		}
	}

	//removes the smallest element in the subtree starting at
	//*root
	BinaryNode<T> * removeSmallest(BinaryNode<T> *root)
	{
		//ASSUMPTION: non-null root
		if (root == nullptr)
		{
			return root;
		}

		//BASE CASE: root is smallest (has no left node)
		if (root->getLeftChild() == nullptr)
		{
			BinaryNode<T> *right = root->getRightChild();
			delete root;

			//return right information to parent
			return right;
		}
		else
		{
			//RECURSIVE CASE:
			BinaryNode<T> *new_right = removeSmallest(root->getLeftChild());
			root->setLeftChild(new_right);
			return root;
		}
	}

	BinaryNode<T> * findSmallestIter(BinaryNode<T> *root)
	{
		while (root != nullptr && root->getLeftChild() != nullptr)
		{
			root = root->getLeftChild();
		}
		return root;
	}

	BinaryNode<T> * findLargestIter(BinaryNode<T> *root)
	{
		while (root != nullptr && root->getRightChild() != nullptr)
		{
			root = root->getRightChild();
		}
		return root;
	}

	//AC Note: pass by const reference is generally
	//faster than pass by value for larger items
	virtual BinaryNode<T> * addElementHelper(BinaryNode<T> *root, const T& item)
	{
		//BASE CASE: create new node
		if (root == nullptr)
		{
			root = new BinaryNode<T>{};
			root->setValue(item);
			return root;
		}

		//RECURSIVE CASE: figure out which child to call
		//is "item" larger than us?
		if (item >= root->getValue())
		{
			//AC: super tricky way to do this
			//root->setRightChild(addElementHelper(root->getRightChild(), item));

			//AC: more explicit way to do the code above
			BinaryNode<T> *right_child = root->getRightChild();
			BinaryNode<T> *result = addElementHelper(right_child, item);
			root->setRightChild(result);
		}
		else
		{
			//AC note: this code is the same as when we add to the right,
			//except we replace "right" with "left"
			BinaryNode<T> *left_child = root->getLeftChild();
			BinaryNode<T> *result = addElementHelper(left_child, item);
			root->setLeftChild(result);
		}
		return root;
	}

	virtual BinaryNode<T> *removeElementHelper(BinaryNode<T> *root,
		const T& item)
	{
		//ASSUMPTION: root is not null
		if (root == nullptr)
		{
			return root;
		}

		//three possibilities:
		// we found the item (root value == item)
		// item is less than root (item < root)
		// item is greater than root (item > root)
		if (item == root->getValue())
		{
			//increment removal counter
			_remove_counter++;

			//we found the item
			//do we remove from the left or right?
			if (_remove_counter % 2 == 0)
			{
				//check to see if left is not nullptr
				if (root->getLeftChild() != nullptr)
				{
					//left subtree remove
					//we need the largest from the left side
					BinaryNode<T> *largest = findLargestIter(root->getLeftChild());

					//take the largest's value, put inside root
					root->setValue(largest->getValue());

					//having gotten the value, we can now remove the node
					//from the tree
					BinaryNode<T> * result = removeLargest(root->getLeftChild());
					root->setLeftChild(result);
					return root;
				}
				else
				{
					//else, delete us, return the result
					return removeSmallest(root);
				}
			}
			else
			{
				//right subtree remove
				if (root->getRightChild() != nullptr)
				{
					//find the smallest value on the right
					BinaryNode<T> *smallest = findSmallestIter(root->getRightChild());

					//take the smallest value, put into root
					root->setValue(smallest->getValue());

					//now, it's safe to remove the smallest value
					BinaryNode<T> * result = removeSmallest(root->getRightChild());
					root->setRightChild(result);
					return root;
				}
				else
				{
					return removeLargest(root);
				}
			}
		}
		else if (item < root->getValue())
		{
			//item is less than root
			BinaryNode<T> *result = removeElementHelper(
				root->getLeftChild(), //send along our left child
				item				  //and the same item
			);

			//the recursive call *might* have altered our
			//left child's structure.  Inform root of this
			//change
			root->setLeftChild(result);
		}
		else
		{
			//item is greater than root
			BinaryNode<T> *result = removeElementHelper(
				root->getRightChild(),
				item
			);
			root->setRightChild(result);
		}

		//if we didn't return any sooner, return the root now
		return root;
	}
#pragma endregion

	void traverse(BinaryNode<T> *root)
	{
		//make sure root isn't null
		if (root == nullptr)
		{
			return;
		}

		//pre-order is Us - Left - Right
		cout << root->getValue() << " ";
		traverse(root->getLeftChild());
		traverse(root->getRightChild());
	}

public:

	//AC Note: iterative approach to node deletion
	virtual ~BinarySearchTree()
	{
		vector<BinaryNode<T> *> nodes{};
		nodes.push_back(_root);
		while (nodes.size() > 0)
		{
			BinaryNode<T> *last = nodes[nodes.size() - 1];
			nodes.pop_back();

			//make sure that we're dealing with an actual
			//binary node
			if (last != nullptr)
			{
				//add last's children to the stack
				nodes.push_back(last->getLeftChild());
				nodes.push_back(last->getRightChild());

				//having gotten all of the information out of
				//last, we can now delete the node
				delete last;
			}
		}
	}

	void removeElement(T item)
	{
		_root = removeElementHelper(_root, item);
		_size_counter--;
	}

	virtual void addElement(T item)
	{
		_root = addElementHelper(_root, item);
		_size_counter++;
	}

	void traverse()
	{
		traverse(_root);
	}

	virtual bool isEmpty() const
	{
		return _root == nullptr;
	}
	virtual int getSize() const
	{
		return _size_counter;
	}

	//for MA #5 unit testing.  Will be removed after.
	BinaryNode<T> *getRoot()
	{
		return _root;
	}
};


template <typename T>
class AvlTree : public BinarySearchTree<T>
{
protected:

	virtual BinaryNode<T> * balance(AvlNode<T> *root);
	virtual BinaryNode<T> *rotateLeft(BinaryNode<T> *root);
	virtual BinaryNode<T> *rotateRight(BinaryNode<T> *root);

	virtual BinaryNode<T> * setHeight(AvlNode<T> * root)
	{
		//check for null roots
		if (root == nullptr)
		{
			return root;
		}

		//get left and right children, convert into AVL Nodes
		AvlNode<T> *left = dynamic_cast<AvlNode<T> *>(root->getLeftChild());
		AvlNode<T> *right = dynamic_cast<AvlNode<T> *>(root->getRightChild());

		//start at height 1 because I'm representing nullptrs with values of -1
		int height = 1;

		//add in largest of left or right heights
		int left_height = left == nullptr ? -1 : left->getHeight();
		int right_height = right == nullptr ? -1 : right->getHeight();
		height += (left_height < right_height) ? right_height : left_height;

		//reassign new height to root
		root->setHeight(height);

		//check to see if balance factor is out of whack
		int balance_factor = root->getBalanceFactor();
		if (balance_factor < -1 || balance_factor > 1)
		{
			return balance(root);
		}
		return root;
	}

	virtual BinaryNode<T> * addElementHelper(BinaryNode<T> *root, const T& item)
	{
		//BASE CASE: create new node
		if (root == nullptr)
		{
			root = new AvlNode<T>{};
			root->setValue(item);
			return root;
		}

		//else, just call BST helper
		BinaryNode<T> *bst_result = BinarySearchTree<T>::addElementHelper(root, item);

		//if everything went according to plan, we should be able to cast the BinaryNode
		//into an AvlNode
		AvlNode<T> *avl_node = dynamic_cast<AvlNode<T>*>(bst_result);

		//return balanced node
		return setHeight(avl_node);
	}

	virtual BinaryNode<T> *removeElementHelper(BinaryNode<T> *root, const T& item)
	{
		//call parent's method
		BinaryNode<T> *bst_result = BinarySearchTree<T>::removeElementHelper(root, item);

		//if everything went according to plan, we should be able to cast the BinaryNode
		//into an AvlNode
		AvlNode<T> *avl_node = dynamic_cast<AvlNode<T>*>(bst_result);

		//if we don't have a nullptr, balance node
		if (avl_node != nullptr)
		{
			return setHeight(avl_node);
		}
		return avl_node;
	}
};

// -------------------------------------------------------------------

//MA #1 Jack Daniel Kinne
//CS 212  

template<typename T>
BinaryNode<T> * AvlTree<T>::balance(AvlNode<T> *root)
{
	//if root is null
	if (root == nullptr)
	{
		return root; //return null
	}

	AvlNode<T> *left = dynamic_cast<AvlNode<T> * >(root->getLeftChild() );
	AvlNode<T> *right = dynamic_cast<AvlNode<T> * >(root->getRightChild() );

	// Get balance factor at root
	if (root->getBalanceFactor() > 1)
	{
		return rotateLeft(root); // If balance factor is > 1, rotate left.
	}
	else if (root->getBalanceFactor() < -1)
	{
		return rotateRight(root); // If Bfactor is < -1, rotate right.
	}
	return root; //error catching.  
}



template<typename T>
BinaryNode<T> * AvlTree<T>::rotateLeft(BinaryNode<T> *root)
{
	BinaryNode<T> *CurrentRoot = root;// let current root = original root
	BinaryNode<T> *NewRoot = CurrentRoot->getRightChild(); //newroot is currents right child.
	
	if (NewRoot->getRightChild() == nullptr) //does newroot have a right child?  if it does:
	{
		CurrentRoot->setRightChild(nullptr); //set currentroots left child = new roots right
		NewRoot->getLeftChild()->setLeftChild(CurrentRoot); // new roots right child = current root
		return rotateRight(NewRoot); 
	}
	else
	{
		CurrentRoot->setRightChild(NewRoot->getLeftChild()); //double rotate, recursive
		NewRoot->setLeftChild(CurrentRoot);
		return NewRoot;
	}
}
template<typename T>
BinaryNode<T> * AvlTree<T>::rotateRight(BinaryNode<T> *root)
{
	BinaryNode<T> *CurrentRoot = root; // let current root = original root
	BinaryNode<T> *NewRoot = CurrentRoot->getLeftChild(); //newroot is currents left child.
	
	if (NewRoot->getLeftChild() == nullptr) //does newroot have a right child?  if it does:
	{
		CurrentRoot->setLeftChild(nullptr); //set currentroots left child = new roots right
		NewRoot->getRightChild()->setRightChild(CurrentRoot); // new roots right child = current root
		return rotateLeft(NewRoot);
	}
	else
	{
		CurrentRoot->setLeftChild(NewRoot->getRightChild()); //double rotate, recursive
		NewRoot->setRightChild(CurrentRoot); 
		return NewRoot;
	}
}

// -------------------------------------------------------------------


int main(int argc, char* argv[])
{
	string line = "";
	getline(cin, line);
	AvlTree<int> tree{};
	while (line.length() > 0)
	{
		int next_number = stoi(line);
		tree.addElement(next_number);
		getline(cin, line);
	}

	//In-order traversal
	tree.traverse();


	//remove for hackerrank!
	system("PAUSE");
}