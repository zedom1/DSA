







////// �ڽӾ����ʾ�� ///////

// ���㱣��int����
// ���ڴ���ʱҪ������Ȩֵ
// ͬһ�����ȨֵΪ0
// ʹ��DFS,BFSʱ��Ҫһ��ȫ��Bool����visited


#include <Queue.h>

typedef int Vertex;
typedef int Edge;
#define MaxSize 100
#define INF 0x7fffffff
bool visited[MaxSize]={0};
enum GraphType{ DG, UG, DN, UN };
/* ����ͼ������ͼ��������ͼ��������ͼ */

struct Graph
{
	Vertex vertex[MaxSize];      // �����
	Edge edge[MaxSize][MaxSize]; // �ڽӾ��󣬱߱�
	int n , e;                   // ���������ͱ�����
	GraphType type;
	Graph()
	{
		memset(vertex,0,sizeof(vertex));
		memset(edge,0,sizeof(edge));
	}
};

void CreateGraph ( Graph * g)
{
	g->type = UN;
	int i,j,w;
	scanf("%d%d",&g->n,&g->e);   // �������ͱ���
	for( i=0; i<g->n; i++ )
		scanf("%d",&(g->vertex[i]));
	for( int k=0; k<g->e ; k++ )
	{
		scanf("%d%d%d",&i,&j,&w);  // ����e���ߵ�Ȩֵ
		g->edge[i][j]=w;
		g->edge[j][i]=w;
	}
}

int FirstAdjv( Graph *g , int v )
{
	int i;
	for(i=v ; g->edge[v][i]==0 && i<g->n  ; i++ );
	return g->edge[v][i]==0?-1:i;
}

int NextAdjv( Graph *g , int v ,int w )
{
	for( w+=1 ; g->edge[v][w]==0 && w<g->n ; w++ );
	return g->edge[v][w]==0?-1:w;
}

void DFS( Graph * g , int i )
{
	visited[i]=true;
	printf("Visited %d \n", g->vertex[i]);
	for( int j = FirstAdjv(g,i); j!=-1&&j<g->n ; j=NextAdjv(g,i,j))
		if(!visited[j])
			DFS(g,j);
}

void BFS( Graph * g , int i )
{
	Queue<int> q;
	int v;
	memset(visited, 0, sizeof(visited));
	for( ; i<g->n; i++)
		if(!visited[i])
		{
			visited[i]=true;
			printf("Visited %d \n", g->vertex[i]);
			q.enqueue(i);
			while(!q.empty())
			{
				v = q.dequeue();
				for( int j = FirstAdjv(g,v) ; j!=-1&&j<g->n ; j=NextAdjv(g,v,j))
					if(!visited[j])
					{
						visited[j]=true;
						printf("Visited %d \n", g->vertex[j]);
						q.enqueue(j);
					}
			}
		}
}



////// �ڽӱ��ʾ�� ///////

// ���㱣��int����
// ���ڴ���ʱҪ������Ȩֵ
// ʹ��DFS,BFSʱ��Ҫһ��ȫ��Bool����visited

typedef int Vertex;
typedef int Edge;
#define MaxSize 1000
#define INF 0x7fffffff
enum GraphType{ DG, UG, DN, UN };
/* ����ͼ������ͼ��������ͼ��������ͼ */

struct ENode
{
	int adjv;    // ָ�򶥵�����
	ENode * next;
	int weight;
};

struct VNode
{
	Vertex vertex;
	ENode *firstEdge;
};

struct Graph
{
	VNode nodeList[MaxSize];
	int n,e;
	GraphType type;
};

void CreateGraph( Graph *g )
{
	int i,j;
	ENode *edge;
	g->type = DG;
	scanf("%d%d",&(g->n),&(g->e));
	for( i=0 ; i<g->n ; i++)
	{
		scanf("%d",&(g->nodeList[i].vertex));
		g->nodeList[i].firstEdge=NULL;
	}
	for( int k=0; k<g->e ; k++ )
	{
		scanf("%d %d",&i,&j);
		edge = new ENode();
		edge->adjv = j;
		edge->next = g->nodeList[i].firstEdge;
		g->nodeList[i].firstEdge = edge;
	}
}

void DFS( Graph *g , int i )
{
	ENode *w;
	visited[i]=true;
	printf("visit %d \n",g->nodeList[i].vertex);
	for( w= g->nodeList[i].firstEdge ; w ; w=w->next )
		if(!visited[w->adjv])
			DFS(g,w->adjv);
}

void BFS( Graph *g , int i )
{
	Queue<int> q;
	memset(visited,false,sizeof(visited));
	q.enqueue(i);
	visited[i]=true;
	ENode *w;
	int v;
	while(!q.empty())
	{
		v=q.dequeue();
		printf("visit %d \n",g->nodeList[v].vertex);
		w=g->nodeList[v].firstEdge;
		while(w)
		{
			if(!visited[w->adjv])
				q.enqueue(w->adjv);
			w=w->next;
		}
	}
}















































