# include <DoubleList.h>

////   版本一：利用已实现的双向链表
template <typename T>
struct Queue:public List<T> 
{ // size和empty直接沿用
	void enqueue ( T const& e ) {insertAsLast(e);}
	T dequeue() { return remove(first()); }
	T& front() { return first()->data; }
	T& rear() { return last()->data; }
};

////   版本二：数组模拟版
template <typename T>
struct Queue
{
private:
	T *data;
	int first,last,maxsize;

protected:
	void expand();

public:
	Queue();
	~Queue() {delete []data;}
	bool empty()const {return first==last;}
	int size()const {return last-first;}
	T& front()const { if(!empty())return data[first]; else exit(1);}
	T& rear()const { if(!empty())return data[last-1]; else exit(1);}
	void enqueue( T const & e) { expand(); data[last++]=e; }
	T dequeue() { return data[first++]; }
};

template <typename T>
Queue<T>::Queue()
{
	maxsize=4;
	data=new T[maxsize];
	first=last=0;
}

template <typename T>
void Queue<T>::expand()
{
	if( size()>= maxsize*3/4)
	{
		T *tem=new T [maxsize*2];
		maxsize*=2;
		for(int i=first,m=0 ; i<last ; i++)
			tem[m++]=data[i];
		delete []data;
		data=tem;
		tem=NULL;
	}
	if( maxsize-last<=2)
	{
		T *tem=new T [maxsize];
		for(int i=first,m=0 ; i<last ; i++)
			tem[m++]=data[i];
		delete []data;
		data=tem;
		tem=NULL;
		last-=first;
		first=0;
	}
	return;
} 


////   版本三：环状数组版
template <typename T>
struct Queue
{
private:
	T *data;
	int maxsize;
	int first;
	int last;
	int size;

public:
	Queue(int size=0)
	{
		maxsize=size+1;
		last=0;
		first=1;
		data= new T[maxsize];
	}
	~Queue()
	{
		delete []data;
	}
	void clear()
	{
		last=first=0;
	}
	void enqueue(const T & it)
	{
		last=(last+1)%maxsize;
		data[last]=it;
	}
	T dequeue()
	{
		T it = data[first];
		first = (first+1)%maxsize;
		return it;
	}
	const T firstValue()
	{
		return data[first];
	}
	int length()
	{
		return ((last+maxsize)-first+1)%maxsize;
	}

};