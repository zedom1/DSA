
// ���鼯

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
	/* ��s�в���ֵΪx��Ԫ�������ļ��� */
	/* MaxsizeΪȫ�ֱ�����s����󳤶� */
	int i;
	for( i=0; i<MaxSize&& s[i].data!=x; i++ );
	if( i>=MaxSize ) return -1;
	int tem=i,tem1=s[tem].parent;
	for( ; s[i].parent>=0; i=s[i].parent );

	// �����Ż���ÿ�ΰ���;�ڵ�ĸ��ڵ㶼����Ϊ��
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
	/* ���ϵĲ����� */
	int root1,root2;
	root1=Find(s,x1);
	root2=Find(s,x2);
	if(root1!=root2)
		s[root2].parent = root1;
}

template <typename T>
void Union1( Node<T>* s, T x1 , T x2)
{
	/* �Ż��ļ��ϲ���������С���ϲ���󼯺� */
	/* parentΪ���������Ըýڵ�Ϊ���Ľڵ������ĸ��� */
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