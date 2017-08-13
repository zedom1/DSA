template <typename T>
struct queue:public List<T> //利用双向链表版
{ // size和empty直接沿用
	void enqueue ( T const& e ) {insertAsLast(e);}
	T dequeue() { return remove(first()); }
	T& front() { return first()->data; }
	T& rear() { return last()->data; }
};

template <typename T>
struct queue  // 数组模拟版
{
private:
	T *data;
	int first,last,maxsize;

protected:
	void expand();

public:
	queue();
	~queue() {delete []data;}
	bool empty()const {return first==last;}
	int size()const {return last-first;}
	T& front()const { if(!empty())return data[first]; else exit(1);}
	T& rear()const { if(!empty())return data[last-1]; else exit(1);}
	void enqueue( T const & e);
	T dequeue();
};

template <typename T>
queue<T>::queue()
{
	maxsize=4;
	data=new T[maxsize];
	first=last=0;
}

template <typename T>
void queue<T>::expand()
{
	if( size()> maxsize*3/4)
	{
		T *tem=new T [maxsize*2];
		maxsize*=2;
		for(int i=first,m=0 ; i<last ; i++)
			tem[m++]=data[i];
		delete []data;
		data=tem;
		tem=NULL;
	}
	if( maxsize-last<2)
	{
		T *tem=new T [maxsize];
		for(int i=first,m=0 ; i<last ; i++)
			tem[m++]=data[i];
		delete []data;
		data=tem;
		tem=NULL;
	}
	return;
}

template <typename T>
void enqueue( T const &e )
{
	expand();
	data[last++]=e;
}

template <typename T>
T queue<T>::dequeue()
{
	return data[ (first++)-1];
}