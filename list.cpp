#include <iostream>
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <algorithm>
#include <string>
#include <string.h>
#include <cstring>
using namespace std;

template <typename T>
struct ListNode
	//列表节点模板类（以双向链表形式实现）
{
	T data;
	ListNode * pred;
	ListNode * succ;
	
	ListNode () {}
	ListNode (T e, ListNode * p = NULL, ListNode *a = NULL)
		:data(e),pred(p),succ(a){}

	ListNode * insertAsPred ( T const & e );
	ListNode * insertAsSucc ( T const & e );
};

ListNode* ListNode<T>::insertAsPred( T const & e )
{
	ListNode *a=new ListNode( e, pred, this );
	pred->succ=a;
	pred=a;
	return a;
}

ListNode* ListNode<T>::insertAsSucc( T const & e )
{
	ListNode *a=new ListNode( e, this, succ );
	succ->pred=a;
	succ=a;
	return a;
}

template <typename T>
struct List
{
private:
	int _size ;
	ListNode * header,*trailer;

protected:
	void init ();
	int clear ();
	void copyNodes ( ListNode *p , int n );     // 从p开始复制n个节点 
// 	void merge ( ListNode * &p1, int n1, List<T>&, ListNode *p2, int n2); //归并
// 	void mergeSort ( ListNode *&p, int n);      // 对从p开始连续的n个节点归并排序
	void selectionSort ( ListNode *p, int n);   // [ p,p+n ) 选择排序
	void insertionSort ( ListNode *p, int n);   // 对从p开始连续的n个节点插入排序

public:
	List() { init(); }
	List() ( ListNode *p , int n );     // 从p开始复制n个节点 
	~List();

	// 只读接口
	bool empty() { return _size==0; }
	T & operator [] ( int r ) const;    // 实现寻秩访问
	ListNode * first() const 
		{return header->succ;}
	ListNode * last() const 
		{return trailer->pred;}
	ListNode * find( T const&e ) const
		{return find(e,_size,trailer);}
	ListNode * find( T const& e, int n, ListNode* p ) const; // (无序)从节点p向前n个节点（不含p）内找e
	ListNode * search( T const & e , int n , ListNode * p ) const; // (有序) 不大于e的最后一个
	ListNode * selectMax ( ListNode *p , int n )const ;

	// 可写接口
	ListNode * insertBefore( ListNode *p, T const & e);
		{ _size++; return p->insertAsPred(e); }
	ListNode * insertAfter( ListNode *p, T const & e);
		{ _size++; return p->insertAsSucc(e); }
	ListNode * insertLast( T const & e )
		{ _size++; return trailer->insertAsPred(e);}
	ListNode * insertFirst( T const & e )
		{ _size++; return header->insertAsSucc(e);}
	T remove( ListNode *p );
	int deduplicate (); // 无序列表去重
	int uniquify ();    // 有序列表去重
};

template <typename T>
void List<T>::init()
{
	header = new ListNode <T>;
	trailer = new ListNode <T>;
	header->succ=trailer;
	trailer->pred=header;
	_size=0;
	trailer->succ=header->pred=NULL;
}

template <typename T>
T & List<T>::operator [] (int r)
{
	ListNode *p=header->succ;
	while(r-- && p)
		p=p->succ;
	return p->data;
}

template <typename T>
ListNode * List<T>::find( T const& e, int n, ListNode* p )
{
	while(n--)
		if( e==( p=p->pred )->data )
			return p;
	return NULL;
}

template <typename T>
List<T>::List( ListNode * p, int n )
{
	init();
	copyNodes( p , n );
}

template <typename T>
void List<T>::copyNodes( ListNode * p, int n )
{
/////  方法一：
// 	ListNode *a=header;
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
T List<T>::remove( ListNode * p )
{
	T tem=p->data;
	p->pred->succ=p->succ;
	p->succ->pred=p->pred;
	delete p;
	_size--;
	return tem;
}

template <typename T>
int List<T> clear()
{
// 	ListNode *a=header->succ;
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
int List<T> deduplicate()
{
	ListNode *p=first();
	int old_size=_size;
	int r=1;
	p=p->succ;
	while( p!=trailer )
	{
		if( find( p->data , r , p )!=NULL )
		{	
			p=p->succ;
			remove( p->pred );
			continue;
		}
		r++;
		p=p->succ;
	}
	return old_size-_size;
}

template <typename T>
int List<T> uniquify()
{
	if(_size<2) return 0;
	int old_size=_size;

////方法一：
// 	ListNode *p=header;
// 	while( trailer!=( p=p->succ ) )
// 	{
// 		while( p->data==p->succ->data && p->succ!=trailer )
// 			remove(p->succ);
// 	}

////方法二：
	ListNode *p=first(),*q;
	while( trailer != ( q = p->succ ) )
		p->data == q->data ? remove(q) : p=q;
	return old_size-_size;
}

template <typename T>
ListNode * List<T>::selectMax ( ListNode *p , int n )
{
	ListNode *tem=p;
	while(n--&&p)
	{
		if(p->data>=tem->data)
			tem=p;
		p=p->succ;
	}
	return tem;
}

template <typename T>
void List<T>::selectionSort( ListNode *p, int n )
{
	if(n<2) return;
	ListNode *head=p->pred , *tail=p;
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
ListNode * List<T>::search( T const & e, int n, ListNode * p )
{
	while( n--&&p )
	{
		if( p->data<=e )
			break;
		p=p->pred;
	}
	return;
}

template <typename T>
void List<T>::insertionSort( ListNode * p , int n )
{
	int r=0;
	while( r<n )
	{
		insertAfter( search( p->data , r , p ) , p->data );
		p=p->succ;
		remove( p->pred );
		r++;
	}
}