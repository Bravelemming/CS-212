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

	AvlNode<T> *left = dynamic_cast<AvlNode<T> * >(root->getLeftChild());
	AvlNode<T> *right = dynamic_cast<AvlNode<T> * >(root->getRightChild());

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
