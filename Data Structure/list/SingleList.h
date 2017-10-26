template < typename T >
struct Node
{
    T data;
    Node<T> *succ;
    Node( T const & e , Node<T> * suc=NULL):data(e),succ(suc) {}
    Node() {succ=NULL;}
    Node<T> * insertAft( T const & e );
};

template < typename T >
Node<T> * Node<T>::insertAft( T const & e )
{
	Node<T> *tem=new Node<T>( e , succ );
	succ=tem;
	return tem;
}

template < typename T >
struct List
{
private:
    Node<T> *header;
    int _size;
public:
	void init ();
	int clear ();
	void copyNodes ( Node<T> *p , int n );     
	//从p开始复制n个节点到链表末尾 
	void merge ( Node<T> *& p1, int n1, Node<T> *&p2, int n2 ); 
	//[p1,p1+n) 本串p1开始n1个节点与 L串p2开始n2个节点归并排序
	void mergeSort ( Node<T> *&p, int n );      
	// 对从p开始连续的n个节点归并排序

	List() { init(); }
	List( Node<T> *p , int n );     // 从p开始复制n个节点 
	~List();

	// 只读接口
	bool empty() { return _size==0; }
	int size() { return _size; }
	T& operator[] ( int r ) const;    // 实现寻秩访问
	Node<T> * find( T const&e ) const;
	Node<T> * find( T const& e,  Node<T>* p, int n ) const; 
	// (无序)从节点p向后n个节点（不含p）内找e
	Node<T> * search( T const & e , Node<T> * p , int n ) const; 
	// (有序) 不大于e的最后一个
	void show();

	// 可写接口
	Node<T> * insertBefore( Node<T> *p, T const & e );
	Node<T> * insertAfter( Node<T> *p, T const & e );
	Node<T> * insertLast( T const & e );
	Node<T> * insertFirst( T const & e );
	void createListT( T const * e , int n ); // 尾插法创建链表
	void createListH( T const * e , int n ); // 头插法创建链表
	T remove( Node<T> *p );
	int deduplicate (); // 无序列表去重
	int uniquify ();    // 有序列表去重
	void sort(); // 排序
	void reverse(); // 反转
};

template <typename T>
void List<T>::init()
{
	header = new Node<T>;
	_size=0;
}

template <typename T>
List<T>::~List()
{
	clear();
	delete header;
}

template <typename T>
T List<T>::remove( Node<T> * p )
{
	T tem=p->data;
	Node<T> *pre=header;
	while( pre->succ!=p )
		pre=pre->succ;
	pre->succ=p->succ;
	delete p;
	_size--;
	return tem;
}

template <typename T>
int List<T>::clear()
{
	int old_size=_size;
	if( header->succ==NULL )
		return 0;
	while( header->succ )
		remove( header->succ );
	return old_size;
}

template <typename T>
T& List<T>::operator[] (int r) const
{
	Node<T> *p=header->succ;
	while( r-- && p )
		p=p->succ;
	return p->data;
}

template <typename T>
List<T>::List( Node<T> * p, int n )
{
	init();
	copyNodes( p , n );
}

template < typename T >
void List<T>::copyNodes( Node<T> * p, int n )
{
	Node<T> *tem=header;
	while( tem->succ )
		tem=tem->succ;
	Node<T> *create;
	_size+=n;
	while(n--)
	{
		create=new Node<T> ( p->data );
		tem->succ=create;
		p=p->succ;
		tem=tem->succ;
	}
	return;
}

template < typename T >
void List<T>::createListT( T const * e , int n )
{
	Node<T> * last=header;
	Node<T> * tem;
	_size+=n;
	for( int i=0; i<n; i++ )
	{
		tem = new Node<T>( e[i] );
		last->succ=tem;
		last=tem;
	}
	return;
}

template < typename T >
void List<T>::createListH( T const * e , int n )
{
	Node<T> * tem;
	_size+=n;
	for( int i=0; i<n; i++ )
	{
		tem = new Node<T>( e[i], header->succ );
		header->succ=tem;
	}
	return;
}

template < typename T >
Node<T> * List<T>::insertBefore( Node<T> *p, T const & e )
{
    _size++; 
    Node<T> *tem=header;
    while( tem->succ!=p )
		tem=tem->succ;
    return tem->insertAft(e); 
}

template < typename T >
Node<T> * List<T>::insertAfter( Node<T> *p, T const & e)
{ 
	_size++; 
	return p->insertAft(e); 
}

template < typename T >
Node<T> * List<T>::insertLast( T const & e )
{ 
	_size++; 
	Node<T> *tem=header;
	while( tem->succ )
		tem=tem->succ;
	return tem->insertAft(e);
}

template < typename T >
Node<T> * List<T>::insertFirst( T const & e )
{ 
	_size++; 
	return header->insertAft(e);
}

template < typename T >
Node<T> * List<T>::find( T const&e ) const
{
	return find( e, header, ,_size );
}

template <typename T>
Node<T> * List<T>::find( T const& e, Node<T>* p , int n ) const 
{
	while(n--)
		if( e==( p=p->succ )->data )
			return p;
	return NULL;
}

template <typename T>
Node<T> * List<T>::search( T const & e, Node<T> * p, int n ) const
{
	Node<T> *tem=header;
	while( tem->succ != p )
		tem=tem->succ;
	while( n-- &&p )
	{
		if( p->data>e )
			break;
		p=p->succ;
		tem=tem->succ;
	}
	return tem;
}

template <typename T>
void List<T>::merge ( Node<T> * &p1, int n1, Node<T> *&p2, int n2 )
{
	Node<T> *head;
	Node<T> *pree=header;
	while( pree->succ!=p1 )
		pree=pree->succ;
	head=pree;
	Node<T> *px1=p1,*px2=p2,*px3;
	for(int i=0; i<n1-1 ; i++)
		px1=px1->succ;
	for(int i=0; i<n2-1 ; i++)
		px2=px2->succ;
	px3=px2->succ;
	px1->succ=px2->succ=NULL;
	px1=p1;
	while( n1&&n2 )
	{
		if( px1->data<=p2->data )
		{
			pree->succ=px1;
			pree=pree->succ;
			px1=px1->succ;
			n1--;
		}
		else
		{
			pree->succ=p2;
			pree=pree->succ;
			p2=p2->succ;
			n2--;
		}
	}
	if( n1 )
		pree->succ=px1;
	if( n2 )
		pree->succ=p2;
	while( pree->succ )
		pree=pree->succ;
	pree->succ=px3;
	p1=head->succ;
	return;
}

template <typename T>
void List<T>::mergeSort( Node<T> *&p, int n )
{ // [ p, p+n ]
	if(n<2) return;
	int mid=n/2;
	Node<T> *tem=p;
	for(int i=0; i<mid; i++)  tem=tem->succ;
	mergeSort( p, mid );
	mergeSort( tem, n-mid );
	merge( p,mid,  tem , n-mid );
	return;
}

template <typename T>
int List<T>::deduplicate()
{
	if( _size<2 )
		return 0;
	Node<T> *p=header->succ->succ;
	Node<T> *pre=header->succ;
	int old_size=_size;
	int r=1;
	while( p )
	{
		if( find( p->data , header , r )!=NULL )
		{	
			pre->succ=p->succ;
			delete p;
			p=pre->succ;
			continue;
		}
		r++;
		p=p->succ;
		pre=pre->succ;
	}
	return old_size-_size;
}

template <typename T>
int List<T>::uniquify()
{
	if(_size<2) 
		return 0;
	int old_size=_size;
	Node<T> *p=header->succ,*q;
	while( NULL != ( q = p->succ ) )
		(p->data == q->data) ? remove(q) : (p=q);
	return old_size-_size;
}

template <typename T>
void List<T>::show()
{
	Node<T> * tem=header->succ;
	while(tem)
	{
		cout<<tem->data<<" ";
		tem=tem->succ;
	}
	cout<<endl;
	return;
}

template <typename T>
void List<T>::sort()
{
	mergeSort( header->succ , _size );
	return;
}

template <typename T>
void List<T>::reverse()
{
	if(_size<2)
		return;
	Node<T> *fir=header->succ;
	Node<T> *tem;
	while( fir->succ )
	{
		tem=fir->succ;
		fir->succ=tem->succ;
		tem->succ=header->succ;
		header->succ=tem;
	}
	return;
}