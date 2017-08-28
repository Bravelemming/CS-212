//MA #1 TODO: Implement these three functions
template<typename T>
BinaryNode<T> * AvlTree<T>::balance(AvlNode<T> *root)
{
	if (root == nullptr)
	{
		return root;
	}

	AvlNode<T> *left = dynamic_cast<AvlNode<T> *>(root->getLeftChild());
	AvlNode<T> *right = dynamic_cast<AvlNode<T> *>(root->getRightChild());

	//get balance factor at root, if balance factor > 1 rotate left, if balance factor < -1 rotate right. 
	if (root->getBalanceFactor() > 1)
	{
		return rotateLeft(root);
	}
	if (root->getBalanceFactor() < -1)
	{
		return rotateRight(root);
	}

	return root;
}

template<typename T>
BinaryNode<T> * AvlTree<T>::rotateLeft(BinaryNode<T> *root)
{
	BinaryNode<T> *current_root = root;
	BinaryNode<T> *new_root = current_root->getRightChild();
	if (new_root->getLeftChild() != nullptr)
	{
		BinaryNode<T> *hold_root = new_root;
		new_root = new_root->getLeftChild();
		new_root->setRightChild(hold_root);
	}
	current_root->setRightChild(new_root->getLeftChild());
	new_root->setLeftChild(current_root);
	return new_root;
}
template<typename T>
BinaryNode<T> * AvlTree<T>::rotateRight(BinaryNode<T> *root)
{
	BinaryNode<T> *current_root = root;
	BinaryNode<T> *new_root = current_root->getLeftChild();
	if (new_root->getRightChild() != nullptr)
	{
		BinaryNode<T> *hold_root = new_root;
		new_root = new_root->getRightChild();
		new_root->setLeftChild(hold_root);
	}
	current_root->setLeftChild(new_root->getRightChild());
	new_root->setRightChild(current_root);
	return new_root;
}