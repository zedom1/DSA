template <typename T>
struct ListNode
	//列表节点模板类（以双向链表形式实现）
{
	T data;
	ListNode<T> * pred;
	ListNode<T> * succ;
	
	ListNode () {}
	ListNode ( T const & e, ListNode<T> * p = NULL, ListNode<T> *a = NULL)
		:data(e),pred(p),succ(a){}

	ListNode<T> * insertAsPred ( T const & e );
	ListNode<T> * insertAsSucc ( T const & e );
};

template <typename T>
ListNode<T>* ListNode<T>::insertAsPred( T const & e )
{
	ListNode<T> *a=new ListNode<T>( e, pred, this );
	pred->succ=a;
	pred=a;
	return a;
}

template <typename T>
ListNode<T>* ListNode<T>::insertAsSucc( T const & e )
{
	ListNode<T> *a=new ListNode<T>( e, this, succ );
	succ->pred=a;
	succ=a;
	return a;
}

template <typename T>
struct List
{
private:

	int _size ;
	ListNode<T> * header,*trailer;

protected:
	void init ();
	void merge ( ListNode<T> * p1, int n1 , List<T>&L,  ListNode<T> *p2, int n2); //[p1,p1+n) 本串p1开始n1个节点与 L串p2开始n2个节点归并排序 
	void mergeSort ( ListNode<T> *p, int n );      // 对从p开始连续的n个节点归并排序
	void selectionSort ( ListNode<T> *p, int n );   // [ p,p+n ) 选择排序
	void insertionSort ( ListNode<T> *p, int n );   // 对从p开始连续的n个节点插入排序

public:
	List() { init(); }
	List( ListNode<T> *p , int n );     // 从p开始复制n个节点 
	List( T *p , int n );
	~List();

	// 只读接口
	int size() const { return _size; }
	bool empty() { return _size==0; }
	T & operator [] ( int r ) const;    // 实现寻秩访问
	ListNode<T> * first()const
		{return header->succ;}
	ListNode<T> * last() const 
		{return trailer->pred;}
	ListNode<T> * find( T const&e ) const
		{return find(e,_size,trailer);}
	ListNode<T> * find( T const& e, int n, ListNode<T>* p ) const; // (无序)从节点p向前n个节点（不含p）内找e
	ListNode<T> * search( T const & e , int n , ListNode<T> * p ) const; // (有序) 不大于e的最后一个
	ListNode<T> * selectMax ( ListNode<T> *p , int n )const ;
	void show() const;

	// 可写接口
	ListNode<T> * insertBefore( ListNode<T> *p, T const & e)
		{ _size++; return p->insertAsPred(e); }
	ListNode<T> * insertAfter( ListNode<T> *p, T const & e)
		{ _size++; return p->insertAsSucc(e); }
	ListNode<T> * insertLast( T const & e )
		{ _size++; return trailer->insertAsPred(e);}
	ListNode<T> * insertFirst( T const & e )
		{ _size++; return header->insertAsSucc(e);}
	T remove( ListNode<T> *p );
	int deduplicate (); // 无序列表去重
	int uniquify ();    // 有序列表去重
	int clear ();
	void copyNodes ( ListNode<T> *p , int n );     // 从p开始复制n个节点 
	void sort( ListNode<T> *p, int n, int mod );
};

template <typename T>
void List<T>::init()
{
	header = new ListNode<T>;
	trailer = new ListNode<T>;
	header->succ=trailer;
	trailer->pred=header;
	_size=0;
	trailer->succ=header->pred=NULL;
}

template <typename T>
T & List<T>::operator [] (int r) const
{
	ListNode<T> *p=header->succ;
	while(r-- && p)
		p=p->succ;
	return p->data;
}

template <typename T>
ListNode<T> * List<T>::find( T const& e, int n, ListNode<T>* p )const
{
	while( 0<n-- )
		if( e==( p=p->pred )->data )
			return p;
	return NULL;
}

template <typename T>
List<T>::List( ListNode<T> * p, int n )
{
	init();
	copyNodes( p , n );
}

template <typename T>
List<T>::List( T *e , int n)
{
	init();
	ListNode<T> * las=header;
	ListNode<T> * tem;
	_size+=n;
	for( int i=0; i<n; i++ )
	{
		tem = new ListNode<T>( e[i], las , trailer );
		las->succ=tem;
		las=tem;
	}
	las->succ=trailer;
	trailer->pred=las;
	return;
}

template <typename T>
void List<T>::copyNodes( ListNode<T> * p, int n )
{
/////  方法一：
// 	ListNode<T> *a=header;
// 	while(n--)
// 	{
// 		a=insertAfter(a,p->data);
// 		p=p->succ;
// 	}

/////  方法二：
	while(n--)
	{
		insertLast(p->data);
		p=p->succ;
	}
}

template <typename T>
T List<T>::remove( ListNode<T> * p )
{
	T tem=p->data;
	p->pred->succ=p->succ;
	p->succ->pred=p->pred;
	delete p;
	_size--;
	return tem;
}

template <typename T>
int List<T>::clear()
{
// 	ListNode<T> *a=header->succ;
// 	while(a!=trailer)
// 	{
// 		delete a;
// 		a=header->succ;
// 	}
// 	_size=0;
	int old_size=_size;
	while( !empty() )
		remove(first());
	return old_size;
}

template <typename T>
List<T>::~List()
{
	clear();
	delete header;
	delete trailer;
}

template <typename T>
int List<T>::deduplicate()
{
	if(_size<2)
		return 0;
	ListNode<T> *p=header;
	ListNode<T> *q;
	int old_size=_size;
	int r=0;
	while( (p=p->succ)!=trailer )
	{
		q=find( p->data , r , p );
		q?remove(q):r++;
	}
	return old_size-_size;
}

template <typename T>
int List<T>::uniquify()
{
	if(_size<2) return 0;
	int old_size=_size;

////方法一：
// 	ListNode<T> *p=header;
// 	while( trailer!=( p=p->succ ) )
// 	{
// 		while( p->data==p->succ->data && p->succ!=trailer )
// 			remove(p->succ);
// 	}

////方法二：
	ListNode<T> *p=first(),*q;
	while( trailer != ( q = p->succ ) )
		p->data == q->data ? remove(q) : p=q;
	return old_size-_size;
}

template <typename T>
ListNode<T> * List<T>::selectMax ( ListNode<T> *p , int n )const
{
	ListNode<T> *tem=p;
	while(n--&&p)
	{
		if(p->data>=tem->data)
			tem=p;
		p=p->succ;
	}
	return tem;
}

template <typename T>
void List<T>::selectionSort( ListNode<T> *p, int n )
{
	if(n<2) return;
	ListNode<T> *head=p->pred , *tail=p;
	for( int i=0; i<n; i++ )
		tail=tail->succ;
	while( 1<n )
	{
		insertBefore( tail, remove( selectMax( head->succ , n ) ) );
		tail=tail->pred;
		n--;
	}
	return;
}

template <typename T>
ListNode<T> * List<T>::search( T const & e, int n, ListNode<T> * p )const
{
	while( 0<=n-- )
		if( (p=p->pred)->data<=e )
			break;
	return p;
}

template <typename T>
void List<T>::insertionSort( ListNode<T> * p , int n )
{
	for(int r=0; r<n; r++)
	{
		insertAfter( search( p->data , r , p ) , p->data );
		p=p->succ;
		remove( p->pred );
	}
}

template <typename T>
void List<T>::merge ( ListNode<T> * p1, int n1, List<T>& L, ListNode<T> *p2, int n2)
{
////方法一：
// 	ListNode<T> *pree;
// 	ListNode<T> *pre1=new ListNode<T>, *pre2=new ListNode<T>;
// 	ListNode<T> *aft=new ListNode<T>;
// 	pre1->succ=p1; p1->pred=pre1;
// 	pre2->succ=p2; p2->pred=pre2;
// 	pree=p2;
// 	for(int i=0; i<n2; i++)
// 		pree=pree->succ;
// 	aft->succ=pree;  pree->pred=aft;
// 	pree=p1->pred;
// 	for( int j=0 , k=0 ; j<n1 || k<n2 ; )
// 	{
// 		if( j<n1 && ( ( n2<k ) || ( pre1->succ->data < pre2->succ->data ) ) )
// 		{
// 			insertAfter( pree, remove(pre1->succ) );
// 			pree=pree->succ;
// 			j++;
// 		}
// 		if( k<n2 && ( ( n1<j ) || ( pre2->succ->data < pre1->succ->data ) ) )
// 		{
// 			insertAfter( pree, remove(pre2->succ) );
// 			pree=pree->succ;
// 			k++;
// 		}
// 	}
// 	aft->succ->pred=pree;  pree->succ=aft->succ;
// 	p1=pree->succ;
// 	delete aft,pre1,pre2;
// 	return;

////方法二：
	ListNode<T> *pree=p1->pred;
	while( 0<n2 )
	{
		if( 0<n1 && p1->data<=p2->data )
		{
			if( p2== (p1=p1->succ) )
				break;
			n1--;
		}
		else
		{	
			insertBefore( p1 , L.remove( (p2=p2->succ)->pred ) );
			n2--;
		}
	}
	p1=pree->succ;
	return;
}

template <typename T>
void List<T>::mergeSort( ListNode<T> *p, int n )
{
	if(n<2) return;
	int mid=n/2;
	ListNode<T> *tem=header;
	int count=0;
	while(tem!=p)
	{
		tem=tem->succ;
		count++;
	}
	for(int i=0; i<mid; i++) tem=tem->succ;
	mergeSort( p, mid );
	p=header; 
	for(int i=0 ; i<count; i++) p=p->succ;
	tem=p;
	for(int i=0; i<mid; i++) tem=tem->succ;
	mergeSort( tem, n-mid );
	p=header; 
	for(int i=0 ; i<count; i++) p=p->succ;
	tem=p;
	for(int i=0; i<mid; i++) tem=tem->succ;
	merge( p,mid, *this , tem , n-mid );
	p=header; 
	for(int i=0 ; i<count; i++) p=p->succ;
	tem=p;
	for(int i=0; i<mid; i++) tem=tem->succ;
	return;
}

template <typename T>
void List<T>::show() const
{
	ListNode<T> *tem=header->succ;
	while( tem!=trailer&&tem )
	{
		cout<<tem->data<<" ";
		tem=tem->succ;
	}
	cout<<endl;
	return;
}

template <typename T>
void List<T>::sort( ListNode<T> *p, int n, int mod ) const
{
	switch(mod)
	{
	case 1: selectionSort( p,n ); break;
	case 2: insertionSort( p,n ); break;
	default: mergeSort( p,n ); break;
	}
	return;
}