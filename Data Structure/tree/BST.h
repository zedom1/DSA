
template <typename T>
struct TreeNode
{
	T data;
	TreeNode<T> * left;
	TreeNode<T> * right;
	TreeNode ( T d , TreeNode<T>* l = NULL, TreeNode<T>* r = NULL )
		:data(d),left(l),right(r){}
};

template <typename T>
TreeNode<T> * Find(T x , TreeNode<T> * BST)
{
	while(BST)
	{
		if(x>BST->data)
			BST=BST->right;
		else if(x<BST->data)
			BST=BST->left;
		else
			return BST;
	}
	return NULL;
}

template <typename T>
TreeNode<T> * FindMax( TreeNode<T> * BST)
{
	if(BST)
		while(BST->right)
			BST=BST->right;
	return BST;
}
template <typename T>
TreeNode<T> * FindMin( TreeNode<T> * BST)
{
	if(BST)
		while(BST->left)
			BST=BST->left;
	return BST;
}
template <typename T>
TreeNode<T> * Insert( T x , TreeNode<T> * BST)
{
	if(!BST)
		return BST = new TreeNode<T>(x);
	else
	{
		if(x<BST->data)
			BST->left=Insert(x,BST->left);
		else if(x>BST->data)
			BST->right=Insert(x,BST->right);
	}
	return BST;
}

template <typename T>
TreeNode<T> * Delete( T x , TreeNode<T> * BST)
{
	TreeNode<T> *tmp;
	if(!BST) return NULL;
	else if(x<BST->data)
		BST->left=Delete(x,BST->left);
	else if(x>BST->data)
		BST->right=Delete(x,BST->right);
	else
	{
		if(BST->left && BST->right)
		{
			tmp=FindMin(BST->right);
			BST->data=tmp->data;
			BST->right=Delete(BST->data,BST->right);
		}
		else
		{
			tmp=BST;
			if(BST->left)
				BST=BST->left;
			else
				BST=BST->right;
			delete tmp;
		}
	}
	return BST;
}