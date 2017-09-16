
template <typename T>
struct Heap
{
	T *data;
	int size;
	int capacity;
};

template <typename T>
Heap<T>* Create( int MaxSize)
{
	Heap<T>* h = new Heap<T>();
	h->data = new Heap[MaxSize+1];
	h->size = 0;
	h->capacity = MaxSize;
	h->data[0] = 0x7fffffff;
	return h;
}

template <typename T>
void Insert( Heap<T>* h, T item)
{
	int i;
	if( IsFull(h) )
		return;
	i = ++h->size;
	for( ; h->data[i/2]<item ; i/=2 )
		h->data[i] = h->data[i/2];
	h->data[i] = item;
}

template <typename T>
T DeleteMax( Heap<T>* h )
{
	int parent , child;
	T maxn,tem;
	if( IsEmpty(h) ) 
		return;
	maxn = h->data[1];
	tem = h->data[h->size--];
	for( parent=1 ; parent*2<= h->size ; parent = child )
	{
		child = parent *2;
		if( child != h->size  && h->data[child]<h.data[child+1])
			child++;
		if( tem>= h->data[child] ) 
			break;
		else
			h->data[parent] = h->data[child];
	}
	h->data[parent]=tem;
	return maxn;
}

template <typename T>
T DeleteMin( Heap<T>* h )
{
	int parent , child;
	T minx,tem;
	if( IsEmpty(h) ) 
		return;
	minx = h->data[1];
	tem = h->data[h->size--];
	for( parent=1 ; parent*2<= h->size ; parent = child )
	{
		child = parent *2;
		if( child != h->size  && h->data[child]>h.data[child+1])
			child++;
		if( tem<= h->data[child] ) 
			break;
		else
			h->data[parent] = h->data[child];
	}
	h->data[parent]=tem;
	return minx;
}

template <typename T>
Heap<T>* BuildMinHeap( Heap<T>* h )
{

}

























