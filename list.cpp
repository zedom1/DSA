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

	ListNode * insertAsPre (T const & e);
	ListNode * insertAsAft (T const & e);
};

template <typename T>
struct List
{
private:
	int _size ;
	ListNode * header,*trailer;

protected:
	void init ();
	int clear ();
	void copyNode (ListNode *p , int n);
	void merge ( ListNode * &p1, int n1, List<T>&, ListNode *p2, int n2); //归并
	void mergeSort ( ListNode *&p, int n); //对从p开始连续的n个节点归并排序
	void selectionSort ( ListNode *p, int n); //对从p开始连续的n个节点选择排序
	void insertionSort ( ListNode *p, int n); //对从p开始连续的n个节点插入排序

public:
	List() {init();}
	~List();

	// 只读接口
	T & operator [] (int r) const;
	ListNode * first() const {return header->succ;}
	ListNode * last() const {return trailer->pred;}
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
