#include<Stack.h>
#include<Queue.h>

#define stature(p) ((p)?(p->height):-1)

template<typename T>
struct v
{
	void operator() ( T const & e ) { cout<<e<<" "; }
};


template <typename T>
struct BinNode
{
	T data;
	BinNode<T>* lChild,*rChild,*parent;
	int height;

	BinNode() { lChild=rChild=parent= NULL ; height=0; }
	BinNode( T e , BinNode<T>* pa=NULL , BinNode<T>* lc=NULL, BinNode<T>* rc=NULL)
		:data(e),parent(pa),lChild(lc),rChild(rc) {}
	int size() const ;
	BinNode<T> * insertAsLC( T const & e ) //作为左孩子插入
	{ return lChild=new BinNode(e,this); }
	BinNode<T> * insertAsRC( T const & e ) //作为右孩子插入
	{ return rChild=new BinNode(e,this); }

	BinNode<T> * succ();   // 中序遍历时的后继
	
	template <typename V>
	void travLevel( V & visit );  // 子树层级遍历
	// 递归版本
	template <typename V>
	void travPre_R( V & visit );   // 子树先序遍历
	template <typename V>
	void travIn_R( V & visit );    // 子树中序遍历
	template <typename V>
	void travPost_R( V & visit );   // 子树后序遍历

	// 迭代版本
	template <typename V>
	void travPre_I( V & visit );   // 子树先序遍历
	template <typename V>
	void travIn_I( V & visit );    // 子树中序遍历
	template <typename V>
	void travPost_I( V & visit );   // 子树后序遍历

	// 借助直接后继的中序遍历迭代版
	template <typename V>
	void travIn_S( V & visit );
};

template <typename T>
int BinNode<T>::size() const
{
	int s=1;
	if(lChild)
		s+=lChild->size();
	if(rChild)
		s+=rChild->size();
	return s;
}

template <typename T>
BinNode<T> *BinNode<T>::succ()
{
	BinNode<T>* s=this;
	if(rChild)
	{
		s=rChild;
		while(s->lChild)
			s=s->lChild;
	}
	else
	{
		while( s->parent && s==s->parent->rChild )
			s=s->parent;
		s=s->parent;
	}
	return s;
}
template <typename T> template <typename V>
void BinNode<T>::travLevel( V & visit )
{
	Queue<BinNode<T>*> q;
	BinNode<T>* x=this;
	q.enqueue(x);
	while( !q.empty() )
	{
		x=q.dequeue();
		if(x->lChild)  q.enqueue(x->lChild);
		if(x->rChild)  q.enqueue(x->rChild);
		visit(x->data);
	}
	return;
}

template <typename T> template <typename V>
void BinNode<T>::travPre_R( V & visit )
{
	visit(data);
	if(lChild) lChild->travPre_R(visit);
	if(rChild) rChild->travPre_R(visit);
}
template <typename T> template <typename V>
void BinNode<T>::travIn_R( V & visit )
{
	if(lChild) lChild->travPre_R(visit);
	visit(data);
	if(rChild) rChild->travPre_R(visit);
}
template <typename T> template <typename V>
void BinNode<T>::travPost_R( V & visit )
{
	if(lChild) lChild->travPre_R(visit);
	if(rChild) rChild->travPre_R(visit);
	visit(data);
}

template <typename T> template <typename V>
void BinNode<T>::travPre_I( V & visit )
{
	Stack<BinNode<T> *> s;
	BinNode<T> * x=this;
	s.push(x);
	while( !s.empty() )
	{
		x=s.pop();
		while(x)
		{
			visit(x->data);
			if(x->rChild) 
				s.push(x->rChild);
			x=x->lChild;
		}
	}
}
template <typename T> template <typename V>
void BinNode<T>::travIn_I( V & visit )
{
	Stack<BinNode<T> *> s;
	BinNode<T> * x=this;
	while(1)
	{
		while(x)
		{
			s.push(x);
			x=x->lChild;
		}
		if(s.empty()) 
			break;
		x=s.pop();
		visit(x->data);
		x=x->rChild;
	} 
}
template <typename T> template <typename V>
void BinNode<T>::travPost_I( V & visit )
{
	Stack<BinNode<T> *> s;
	BinNode<T> * x=this;
	s.push(x);
	while(!s.empty())
	{
		if(s.top()!=x->parent)
		{
			x=s.top();
			while(x)
			{
				if(x->rChild) 
					s.push(x->rChild);
				if(x->lChild) 
				{
					s.push(x->lChild);
					x=x->lChild;
				}
				else
					x=x->rChild;
			}
		}
		x=s.pop();
		visit(x->data);
	}
}

template <typename T> template <typename V>
void BinNode<T>::travIn_S(  V & visit )
{
	BinNode<T>* x=this;
	while(x->lChild)
		x=x->lChild;
	do
	{
		visit(x->data);
		x=x->succ();
	}
	while(x);
}

template <typename T>
struct BinTree
{
protected:
	int _size;
	BinNode<T>* _root;
	virtual int updateHeight( BinNode<T>* x); // 更新x节点的高度（用virtual适应不同树对高度的定义）
	void updateHeightAbove( BinNode<T>* x);  // 更新x及x的祖先的高度

public:
	BinTree() {_size=0; _root=NULL;}
	~BinTree() { remove(_root); }
	BinTree( T const& e) {_size=1; _root=new BinNode<T> (e);}
	int size() const {return _size;}
	bool empty() const {return !_root;}
	BinNode<T> * root() const {return _root;}
	BinNode<T> * insertAsRC ( BinNode<T>* x , T const& e );  // 构造右孩子
	BinNode<T> * insertAsLC ( BinNode<T>* x , T const& e );  // 构造左孩子
	BinNode<T> * attachAsLC ( BinNode<T>* x , BinTree<T>*& subtree ); // 接入子树作为左孩子
	BinNode<T> * attachAsRC ( BinNode<T>* x , BinTree<T>*& subtree ); // 接入子树作为右孩子
	int remove( BinNode<T>* x );   // 删除某个节点（及其子树）
	void removeAt( BinNode<T>* x );
	BinTree<T> * secede( BinNode<T>* x );    // 分离子树，返回子树头

	// 遍历
	template <typename V>
	void travLevel( V & visit )  // 层级遍历
	{ _root->travLevel(visit); }
	template <typename V>
	void travPre( V & visit )  // 先序遍历
	{ rand()%2==0?_root->travPre_R(visit):_root->travPre_I(visit); }
	template <typename V>
	void travIn( V & visit )  // 中序遍历
	{ rand()%2==0?_root->travIn_R(visit):_root->travIn_I(visit); }
	template <typename V>
	void travPost( V & visit )   // 后序遍历
	{ rand()%2==0?_root->travPost_R(visit):_root->travPost_I(visit); }

	// rebuild
	void rebuild_PI( T *pre,T *ins,int len );   //  前序+中序
	void rebuild_IP( T *ins,T *post,int len );   //  中序+后序
	void rebuild_PP( T *pre,T *post,int len );   //  (前序+后序)*真二叉树
};

template <typename T>
int BinTree<T>::updateHeight( BinNode<T>* x)
{
	return x->height=1+max(stature(x->lChild),stature(x->rChild));
}
template <typename T>
void BinTree<T>::updateHeightAbove( BinNode<T>* x)
{
	while(x)
	{	
		updateHeight(x);
		x=x->parent;
	}
}
template <typename T>
BinNode<T> * BinTree<T>::insertAsRC ( BinNode<T>* x , T const& e )
{
	_size++;
	x->insertAsRC(e);
	updateHeightAbove(x);
	return x->rChild;
}
template <typename T>
BinNode<T> * BinTree<T>::insertAsLC ( BinNode<T>* x , T const& e )
{
	_size++;
	x->insertAsLC(e);
	updateHeightAbove(x);
	return x->lChild;
}
template <typename T>
BinNode<T> * BinTree<T>::attachAsLC ( BinNode<T>* x , BinTree<T>*& subtree )
{
	x->lChild=subtree->root();
	x->lChild->parent=x;
	_size+=subtree->size();
	updateHeightAbove(x);
	subtree->_root=NULL;
	subtree->_size=0;
	release(subtree);
	subtree=NULL;
	return x;
}
template <typename T>
BinNode<T> * BinTree<T>::attachAsRC ( BinNode<T>* x , BinTree<T>*& subtree )
{
	x->rChild=subtree->root();
	x->rChild->parent=x;
	_size+=subtree->size();
	updateHeightAbove(x);
	subtree->_root=NULL;
	subtree->_size=0;
	release(subtree);
	subtree=NULL;
	return x;
}
template <typename T>
int BinTree<T>::remove( BinNode<T>* x )
{
	int old_size=_size;
	_size-=x->size();
	if(x->parent)
		x->parent->lChild==x ? x->parent->lChild=NULL : x->parent->rChild=NULL;
	updateHeightAbove(x->parent);
	x->parent=NULL;
	removeAt(x);
	return old_size-_size;
}
template <typename T>
void BinTree<T>::removeAt( BinNode<T>* x )
{
	if(!x) return;
	removeAt(x->lChild);
	removeAt(x->rChild);
	if(x->parent)
		x->parent->lChild==x ? x->parent->lChild=NULL : x->parent->rChild=NULL;
	delete x;
	return;
}
template <typename T>
BinTree<T> * BinTree<T>::secede( BinNode<T>* x )
{
	BinTree<T> *a=new BinTree<T>;
	a->_root=x;
	if(x->parent)
		x->parent->lChild==x ? x->parent->lChild=NULL : x->parent->rChild=NULL;
	updateHeightAbove(x->parent);
	x->parent=NULL;
	a->_size=x->size();
	_size-=a->_size;
	return a;
}

template <typename T>
void BinTree<T>::rebuild_PP( T *pre,T *post,int len )
{
	_root=new BinNode<T> (pre[0]);
	rebuildSub_PP( _root,pre,post,len );
	return;
}
template <typename T>
BinNode<T> * rebuildSub_PP( T *pre , T *post, int len )
{ 
	BinNode<T> *s=NULL;
	if( len<=0 ) return s;
	if(len==1)
	{
		s=new BinNode<T>(*pre);
		return s;
	}
	int i;
	for(i=0; *(post+i)!=*(pre+1); i++);
	s=new BinNode<T>(*pre);
	s->lChild=rebuildSub_PP(pre+1,post,i+1);
	s->rChild=rebuildSub_PP(pre+i+2,post+i+1,len-i-2);
	return s;
}

template <typename T>
void BinTree<T>::rebuild_PI( T* pre, T* ins, int len )
{
	_root=rebuildSub_PI(pre,ins,len);
}
template <typename T>
BinNode<T> * rebuildSub_PI( T* pre, T* ins, int len ) // 先序+中序重构
{
	BinNode<T> *s=NULL;
	if(len<=0) return s;
	s=new BinNode<T>(*pre);
	if(len==1)
		return s;
	int i;
	for(i=0; *(ins+i)!=*(pre); i++);
	s->lChild=rebuildSub_PI( pre+1, ins, i );
	s->rChild=rebuildSub_PI( pre+1+i, ins+i+1, len-i-1 );
	return s;
}

template <typename T>
void BinTree<T>::rebuild_IP( T* ins, T* post, int len )
{
	_root=rebuildSub_IP(ins,post,len);
}
template <typename T>
BinNode<T> * rebuildSub_IP( T* ins, T* post, int len ) // 中序+后序重构
{
	BinNode<T> *s=NULL;
	if(len<=0) return s;
	if(len==1)
	{
		s=new BinNode<T>(* (post+len-1) );
		return s;
	}
	int i;
	for(i=0; *(ins+i)!=*(post+len-1); i++);
	s=new BinNode<T>(*(post+len-1));
	s->lChild=rebuildSub_IP( ins, post, i );
	s->rChild=rebuildSub_IP( ins+1+i, post+i, len-i-1 );
	return s;
}