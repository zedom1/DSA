template < typename T >
struct node
{
    T data;
    node<T> *succ;
    node( T const & e , node<T> * suc=NULL):data(e),succ(suc) {}
    node() {succ=NULL;}
    node<T> * insertAft( T const & e );
};

template < typename T >
node<T> * node<T>::insertAft( T const & e )
{
	node<T> *tem=new node<T>( e , succ );
	succ=tem;
	return tem;
}

template < typename T >
struct list
{
private:
    node<T> *header;
    int _size;
public:
	void init ();
	int clear ();
	void copyNodes ( node<T> *p , int n );     
	//从p开始复制n个节点到链表末尾 
	void merge ( node<T> *& p1, int n1, node<T> *&p2, int n2 ); 
	//[p1,p1+n) 本串p1开始n1个节点与 L串p2开始n2个节点归并排序
	void mergeSort ( node<T> *&p, int n );      
	// 对从p开始连续的n个节点归并排序

	list() { init(); }
	list( node<T> *p , int n );     // 从p开始复制n个节点 
	~list();

	// 只读接口
	bool empty() { return _size==0; }
	int size() { return _size; }
	T& operator[] ( int r ) const;    // 实现寻秩访问
	node<T> * find( T const&e ) const;
	node<T> * find( T const& e,  node<T>* p, int n ) const; 
	// (无序)从节点p向后n个节点（不含p）内找e
	node<T> * search( T const & e , node<T> * p , int n ) const; 
	// (有序) 不大于e的最后一个
	void show();

	// 可写接口
	node<T> * insertBefore( node<T> *p, T const & e );
	node<T> * insertAfter( node<T> *p, T const & e );
	node<T> * insertLast( T const & e );
	node<T> * insertFirst( T const & e );
	void createListT( T const * e , int n ); // 尾插法创建链表
	void createListH( T const * e , int n ); // 头插法创建链表
	T remove( node<T> *p );
	int deduplicate (); // 无序列表去重
	int uniquify ();    // 有序列表去重
	void sort(); // 排序
	void reverse(); // 反转
};

template <typename T>
void list<T>::init()
{
	header = new node<T>;
	_size=0;
}

template <typename T>
list<T>::~list()
{
	clear();
	delete header;
}

template <typename T>
T list<T>::remove( node<T> * p )
{
	T tem=p->data;
	node<T> *pre=header;
	while( pre->succ!=p )
		pre=pre->succ;
	pre->succ=p->succ;
	delete p;
	_size--;
	return tem;
}

template <typename T>
int list<T>::clear()
{
	int old_size=_size;
	if( header->succ==NULL )
		return 0;
	while( header->succ )
		remove( header->succ );
	return old_size;
}

template <typename T>
T& list<T>::operator[] (int r) const
{
	node<T> *p=header->succ;
	while( r-- && p )
		p=p->succ;
	return p->data;
}

template <typename T>
list<T>::list( node<T> * p, int n )
{
	init();
	copyNodes( p , n );
}

template < typename T >
void list<T>::copyNodes( node<T> * p, int n )
{
	node<T> *tem=header;
	while( tem->succ )
		tem=tem->succ;
	node<T> *create;
	_size+=n;
	while(n--)
	{
		create=new node<T> ( p->data );
		tem->succ=create;
		p=p->succ;
		tem=tem->succ;
	}
	return;
}

template < typename T >
void list<T>::createListT( T const * e , int n )
{
	node<T> * last=header;
	node<T> * tem;
	_size+=n;
	for( int i=0; i<n; i++ )
	{
		tem = new node<T>( e[i] );
		last->succ=tem;
		last=tem;
	}
	return;
}

template < typename T >
void list<T>::createListH( T const * e , int n )
{
	node<T> * tem;
	_size+=n;
	for( int i=0; i<n; i++ )
	{
		tem = new node<T>( e[i], header->succ );
		header->succ=tem;
	}
	return;
}

template < typename T >
node<T> * list<T>::insertBefore( node<T> *p, T const & e )
{
    _size++; 
    node<T> *tem=header;
    while( tem->succ!=p )
		tem=tem->succ;
    return tem->insertAft(e); 
}

template < typename T >
node<T> * list<T>::insertAfter( node<T> *p, T const & e)
{ 
	_size++; 
	return p->insertAft(e); 
}

template < typename T >
node<T> * list<T>::insertLast( T const & e )
{ 
	_size++; 
	node<T> *tem=header;
	while( tem->succ )
		tem=tem->succ;
	return tem->insertAft(e);
}

template < typename T >
node<T> * list<T>::insertFirst( T const & e )
{ 
	_size++; 
	return header->insertAft(e);
}

template < typename T >
node<T> * list<T>::find( T const&e ) const
{
	return find( e, header, ,_size );
}

template <typename T>
node<T> * list<T>::find( T const& e, node<T>* p , int n ) const 
{
	while(n--)
		if( e==( p=p->succ )->data )
			return p;
	return NULL;
}

template <typename T>
node<T> * list<T>::search( T const & e, node<T> * p, int n ) const
{
	node<T> *tem=header;
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
void list<T>::merge ( node<T> * &p1, int n1, node<T> *&p2, int n2 )
{
	node<T> *head;
	node<T> *pree=header;
	while( pree->succ!=p1 )
		pree=pree->succ;
	head=pree;
	node<T> *px1=p1,*px2=p2,*px3;
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
void list<T>::mergeSort( node<T> *&p, int n )
{ // [ p, p+n ]
	if(n<2) return;
	int mid=n/2;
	node<T> *tem=p;
	for(int i=0; i<mid; i++)  tem=tem->succ;
	mergeSort( p, mid );
	mergeSort( tem, n-mid );
	merge( p,mid,  tem , n-mid );
	return;
}

template <typename T>
int list<T>::deduplicate()
{
	if( _size<2 )
		return 0;
	node<T> *p=header->succ->succ;
	node<T> *pre=header->succ;
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
int list<T>::uniquify()
{
	if(_size<2) 
		return 0;
	int old_size=_size;
	node<T> *p=header->succ,*q;
	while( NULL != ( q = p->succ ) )
		(p->data == q->data) ? remove(q) : (p=q);
	return old_size-_size;
}

template <typename T>
void list<T>::show()
{
	node<T> * tem=header->succ;
	while(tem)
	{
		cout<<tem->data<<" ";
		tem=tem->succ;
	}
	cout<<endl;
	return;
}

template <typename T>
void list<T>::sort()
{
	mergeSort( header->succ , _size );
	return;
}

template <typename T>
void list<T>::reverse()
{
	if(_size<2)
		return;
	node<T> *fir=header->succ;
	node<T> *tem;
	while( fir->succ )
	{
		tem=fir->succ;
		fir->succ=tem->succ;
		tem->succ=header->succ;
		header->succ=tem;
	}
	return;
}