
// 并查集

template <typename T>
struct Node
{
	T data;
	int parent;
	Node( T d , int parent = -1 ):data(d){}
	Node(){parent=-1;}
};

template <typename T>
int Find( Node<T>* s , T x )
{
	/* 在s中查找值为x的元素所属的集合 */
	/* Maxsize为全局变量，s的最大长度 */
	int i;
	for( i=0; i<MaxSize&& s[i].data!=x; i++ );
	if( i>=MaxSize ) return -1;
	int tem=i,tem1=s[tem].parent;
	for( ; s[i].parent>=0; i=s[i].parent );

	// 查找优化，每次把沿途节点的父节点都设置为根
	while( tem1!=-1 && tem1!=i )
	{
		s[tem].parent = i;
		tem=tem1;
		tem1=s[tem].parent;
	}
	return i;
}

template <typename T>
void Union( Node<T>* s, T x1 , T x2)
{
	/* 集合的并操作 */
	int root1,root2;
	root1=Find(s,x1);
	root2=Find(s,x2);
	if(root1!=root2)
		s[root2].parent = root1;
}

template <typename T>
void Union1( Node<T>* s, T x1 , T x2)
{
	/* 优化的集合并操作，把小集合并入大集合 */
	/* parent为负数代表以该节点为根的节点总数的负数 */
	int root1,root2;
	root1=Find(s,x1);
	root2=Find(s,x2);
	if(root1!=root2)
	{	
		if( s[root1].parent<s[root2].parent )
		{	
			s[root1].parent += s[root2].parent;
			s[root2].parent = root1;
		}
		else
		{	
			s[root2].parent += s[root1].parent;
			s[root1].parent = root2;
		}
	}
}