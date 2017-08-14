#include <iostream>
using namespace std;

const char Prior[9][9] = 
{  
//        +   -   *   /   ^   !   (   )  \0
/* + */  '>','>','<','<','<','<','<','>','>',
/* - */  '>','>','<','<','<','<','<','>','>',
/* * */  '>','>','>','>','<','<','<','>','>',
/* / */  '>','>','>','>','<','<','<','>','>',
/* ^ */  '>','>','>','>','<','<','<','>','>',
/* ! */  '>','>','>','>','>','>',' ','>','>',
/* ( */  '<','<','<','<','<','<','<','=',' ',
/* ) */  ' ',' ',' ',' ',' ',' ',' ',' ',' ',
/* \0*/  '<','<','<','<','<','<','<',' ','='
};

template <typename T>
struct Stack
{
private:
	T *data;
	int _size,maxsize;

protected:
	void expand();
	void shrink();

public:
	Stack(){ _size =0 ; maxsize=4; data=new T[maxsize]; };
	Stack( T * origin , int n );
	~Stack() { delete []data; }
	void push( T const & e ) { expand(); data[_size++]=e;  }
	T pop( ) { shrink(); return data[--_size];   }
	T & top( ) { return data[_size-1];};
	int empty()const {return _size==0;}
	int size()const {return _size;}
	int find( T const & e )const;
};

template <typename T>
int Stack<T>::find( T const & e ) const
{
	for( int i=0 ; i<_size ; i++)
		if( data[i] == e )
			return i;
	return -1;
}

template <typename T>
void Stack<T>::expand() 
{
	if(_size < (maxsize*3/4) )
		return;
	maxsize*=2;
	T *tem = new T [maxsize];
	for(int i=0; i<_size ; i++)
		tem[i]=data[i];
	delete [] data;
	data=tem;
	tem=NULL;
	return;
}

template <typename T>
void Stack<T>::shrink() 
{
	if(_size >= (maxsize/4)|| maxsize==4 )
		return;
	maxsize/=2;
	T *tem = new T [maxsize];
	for(int i=0; i<_size ; i++)
		tem[i]=data[i];
	delete [] data;
	data=tem;
	tem=NULL;
	return;
}

template <typename T>
Stack<T>::Stack( T * origin , int n )
{
	_size=n;
	maxsize=_size+2;
	data=new T [maxsize];
	for(int i=0 ; i<n; i++)
		data[n-i-1]=origin[i];
}

//////// 栈应用之一：逆序输出：进制转换////////
void convert(  int n , int base ) 
{
	Stack<char>s;
	static char digit[]=
	{'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	while(n)
	{
		s.push( digit[n%base]);
		n/=base;
	}
	while( !s.empty() )
		cout<<s.pop();
	cout<<endl;
}
//////// 栈应用之一：逆序输出：进制转换////////


//////// 栈应用之二：递归嵌套：：括号匹配 ////////
bool judge ( char *s , int n ) 
{
	Stack<char>a;
	for(int i=0 ; i<n; i++)
	{
		if( s[i]!='(' && s[i]!=')' && s[i]!='[' && s[i]!=']' )
			continue;
		if( s[i]=='('|| s[i]=='[')
			a.push(s[i]);
		else if(a.empty())
			return false;
		else if( (s[i]==')'&&a.top()=='(') || (s[i]==']'&&a.top()=='[') )
			a.pop();
	}
	if(	a.empty() )
		return true;
	return false;
}
//////// 栈应用之二：递归嵌套：：括号匹配 ////////


//////// 栈应用之三：递归嵌套：栈混洗 ////////
template <typename T>
bool stackPermutation( T *origin, T * b , int n )
{
	Stack<T> s;
	Stack<T> ori( origin ,n );
	for( int j=0; j<n ; j++ )
	{	
		if( s.empty() )
		{	
			if( ori.empty() ) return false;
			s.push( ori.pop() );
			cout<<"push\n";
		}
		while( !s.empty() )
		{
			if( s.top()==b[j] )
			{
				s.pop();
				cout<<"pop\n";
				break;
			}
			else
			{
				if( ori.empty() ) return false;
				s.push( ori.pop() );
				cout<<"push\n";
			}
		}
	}
	if( !s.empty() || !ori.empty() )
		return false;
	return true;
}
//////// 栈应用之三：递归嵌套：栈混洗 ////////


//////// 栈应用之四：延迟缓冲：中缀表达式求值 ////////
//////// 栈应用之五：栈式计算：RPN转换 ////////
void readNumber( char *&s , Stack<float>& opnd)
{
	float ans=0;
	opnd.push( *s-'0' );
	while( *(++s)<='9'&& *s>='0' )
		opnd.push( opnd.pop()*10 + *s-'0' );
	if(*s!='.')
		return;
	float f=1;
	while( *(++s)<='9'&& *s>='0' )
		opnd.push( opnd.pop() + (*s-'0')*(f/=10.0) );
	return;
}
void getnum( char a, int & num)
{
	switch(a)
	{
	case '+': num=0;return;
	case '-': num=1;return;
	case '*': num=2;return;
	case '/': num=3;return;
	case '^': num=4;return;
	case '!': num=5;return;
	case '(': num=6;return;
	case ')': num=7;return;
	case '\0': num=8;return;
	}
}
char orderBetween ( char a, char b)
{
	int num1,num2;
	getnum( a, num1 );
	getnum( b, num2 );
	return Prior[num1][num2];
}
float cal( float num1, char op , float num2)
{
	switch (op)
	{
	case '+': return num1+num2;
	case '-': return num1-num2;
	case '*': return num1*num2;
	case '/': return num1/num2;
	case '^': return pow(num1,num2);
	default : exit(1);
	}
}
float cal( char op, float num1)
{
	if( abs(num1-0)<1e-6 )
		return 1;
	for(int i=1; i<num1 ; i++)
		num1*=i;
	return num1;
}
void append( char *&RPN, float num )
{
	int len=strlen(RPN);
	char buf[64];
	if( num!= (float)(int)num )
		sprintf( buf, "%.2f \0",num);
	else
		sprintf( buf, "%d \0",(int)num);
	RPN=(char *)realloc( RPN, sizeof( char )*( len+strlen( buf )+1 ) );
	strcat( RPN,buf );
}
void append( char *&RPN, char op )
{
	int len=strlen( RPN );
	RPN=(char *)realloc( RPN, sizeof( char )*( len + 3 ) );
	sprintf( RPN+len, "%c \0",op );
}
float evaluate ( char * s , char *&RPN )
{ // RPN转换速度非常非常慢
	Stack<float> opnd;
	Stack<char> optr;
	float num1,num2;
	s[strlen(s)]='\0';
	RPN[0]='\0';
	optr.push('\0'); // 结尾操作符
	while( !optr.empty() )
	{
		if( (*s)<='9'&&(*s)>='0' ) //当前是操作数
		{	
			readNumber( s, opnd );  // 可能是多位数
//////// RPN转换 ////////
			append( RPN, opnd.top());
//////// RPN转换 ////////
		}
		else
			switch( orderBetween( optr.top(),*s ) )
			{
			case '<': optr.push(*s); s++; break;
			case '=': optr.pop(); s++; break; // () \0
			case '>': 
			{
				char op=optr.pop();
//////// RPN转换 ////////
				append( RPN, op );
//////// RPN转换 ////////
				if(op=='!')
					opnd.push( cal( op, opnd.pop() ) );
				else
				{
					num2=opnd.pop();
					num1=opnd.pop();
					opnd.push( cal(num1,op,num2));
				}
			}
			}
	}
	return opnd.pop();
}
float rpnEvaluate( char * RPN )
{
	Stack<float> opnd;
	float num1,num2;
	int len=strlen(RPN);
	RPN[len++]='\0';
	while(len--)
	{
		if( (*RPN)<='9'&&(*RPN)>='0' ) //当前是操作数
		{	
			readNumber( RPN, opnd );  // 可能是多位数
		}
		else
		{
			char op=*RPN;
			if(op=='!')
				opnd.push( cal( op, opnd.pop() ) );
			else
			{
				num2=opnd.pop();
				num1=opnd.pop();
				opnd.push( cal(num1,op,num2));
			}
		}
	}
	return opnd.pop();
}
//////// 栈应用之四：延迟缓冲：中缀表达式求值 ////////
//////// 栈应用之五：栈式计算：RPN转换 ////////


//////// 栈应用之六：试探回溯：n皇后问题 ////////
struct Queen
{
	int x,y;
	Queen( int xx=0, int yy=0 ):x(xx),y(yy){}
	bool operator == ( Queen const & a ) const
	{
		if( x==a.x || y==a.y || x+y==a.x+a.y || x-y==a.x-a.y )
			return true;
		return false;
	}
	bool operator != ( Queen const & a ) const { return !( *this == a );}
};
int placeQueens( int n ) // n皇后
{
	int num_solution=0;
	Stack< Queen >a; 
	Queen q(0,0);
	do
	{
		if( a.size()>=n || q.y>=n )
		{	
			q=a.pop();
			q.y++;
		}
		while( q.y<n && a.find(q)>=0 )
			q.y++;
		if( q.y<n )
		{
			a.push( q );
			if( a.size()>=n )
				num_solution++;
			q.x++;
			q.y=0;
		}
	}
	while( !( q.x==0 && q.y>=n ) );
	return num_solution;
}
//////// 栈应用之六：试探回溯：n皇后问题 ////////