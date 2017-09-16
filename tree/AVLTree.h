
template <typename T>
struct AVLTreeNode
{
	T data;
	AVLTreeNode<T>* left;
	AVLTreeNode<T>* right;
	int height;
	AVLTreeNode(T x ,int h=0, AVLTreeNode<T> * l = NULL, AVLTreeNode<T> * r = NULL)
		:data(x),height(h),left(l),right(r)
	{}
	AVLTreeNode(){left=right=NULL;}
};


template <typename T>
int GetHeight(AVLTreeNode<T> * A)
{
	if(!A) return 0;
	return A->height=max(GetHeight(A->left),GetHeight(A->right))+1;
}

template <typename T>
AVLTreeNode<T>* SingleLeftRotation(AVLTreeNode<T> * A)
{
	AVLTreeNode<T>* B= A->left;
	A->left = B->right;
	B->right = A;
	GetHeight(B);
	return B;
}

template <typename T>
AVLTreeNode<T>* SingleRightRotation(AVLTreeNode<T>* A)
{
	AVLTreeNode<T>* B = A->right;
	A->right = B->left;
	B->left = A;
	GetHeight(B);
	return B;
}

template <typename T>
AVLTreeNode<T>* DoubleLeftRightRotation(AVLTreeNode<T>* A)
{
	A->left = SingleRightRotation(A->left);
	return SingleLeftRotation(A);
}

template <typename T>
AVLTreeNode<T>* DoubleRightLeftRotation(AVLTreeNode<T>* A)
{
	A->right = SingleLeftRotation(A->right);
	return SingleRightRotation(A);
}

template <typename T>
AVLTreeNode<T>* AVL_Insertion( T x , AVLTreeNode<T>* A)
{
	if(!A)
		A = new AVLTreeNode<T>(x);
	else if( x < A->data )
	{
		A->left = AVL_Insertion(x , A->left);
		if(GetHeight(A->left)-GetHeight(A->right)==2)
			if( x<A->left->data )
				A = SingleLeftRotation(A);
			else
				A = DoubleLeftRightRotation(A);
	}
	else if( x > A->data )
	{
		A->right = AVL_Insertion( x, A->right );
		if( GetHeight(A->left)- GetHeight(A->right) == -2)
			if( x > A->right->data )
				A = SingleRightRotation(A);
			else
				A = DoubleRightLeftRotation(A);
	}
	GetHeight(A);
	return A;
}
