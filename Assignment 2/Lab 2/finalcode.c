/*
Locally, the program can be run in Visual Studio Code latest version or any Software that has GCC Compiler installed (Ex: CodeBlocks, Dev CPP version 5.11  etc)

The instructions to run the code is given in Run Instructions.txt
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#define WHITE 0
#define GRAY 1
#define BLACK 2
#define oo 1000000000

int n;  
int residualEdges;  
struct edge {
  int tail,head,capacity,flow,inverse; 
};
typedef struct edge edgeType;
edgeType *edgeTab;
int *firstEdge;  // Table indicating first in range of edges with a common tail

int *color;      // for BFS               
int *pred;       // Array for storing augmenting path
int *predEdge;   // edgeTab subscript of edge used to reach vertex i in BFS


int min (int x, int y)
{
return x<y ? x : y;  // returns minimum of x and y
}



int head,tail;
int *q;

void enqueue (int x)
{
q[tail] = x;
tail++;
color[x] = GRAY;
}

int dequeue ()
{
int x = q[head];
head++;
color[x] = BLACK;
return x;
}

// Breadth-First Search for an augmenting path

int bfs (int start, int target)

{
int u,v,i;

for (u=0; u<n; u++)
  color[u] = WHITE;
head = tail = 0;  
enqueue(start);
pred[start] = -1;
while (head!=tail)
{
  u=dequeue();
  if (u==target)
    return 1;

  
  for (i=firstEdge[u]; i<firstEdge[u+1]; i++)
  {
    v=edgeTab[i].head;
    if (color[v]==WHITE && edgeTab[i].capacity-edgeTab[i].flow>0)
    {
      enqueue(v);
      pred[v] = u;
      predEdge[v]=i;
    }
  }
}

return 0;
}

// Ford-Fulkerson Algorithm

int max_flow (int source, int sink)
{
int i,j,u;
int max_flow;
int APcount=0;

printf("Scaling parameter is 1024\n");
color=(int*) malloc(n*sizeof(int));
pred=(int*) malloc(n*sizeof(int));
predEdge=(int*) malloc(n*sizeof(int));
q=(int*) malloc(n*sizeof(int));
if (!color || !pred || !predEdge || !q)
{
  printf("malloc failed %d\n",__LINE__);
  exit(0);
}


max_flow = 0;
for (i=0; i<residualEdges; i++)
  edgeTab[i].flow=0;

// Increment the flow if path exists.

while (bfs(source,sink))
{
  
  int increment = oo;
  APcount++;
  for (u=sink; pred[u]!=(-1); u=pred[u])
  {
    i=predEdge[u];
    increment = min(increment,edgeTab[i].capacity-edgeTab[i].flow);
  }
  
  for (u=sink; pred[u]!=(-1); u=pred[u])
  {
    i = edgeTab[predEdge[u]].inverse;
    edgeTab[predEdge[u]].flow += increment;
    edgeTab[i].flow -= increment;  
  }
  if (n<=500)
  {
  	//Determine the path to print the flow of increment
    for (u=sink; pred[u]!=(-1); u=pred[u])
      printf("%d<-",u);
    printf("%d adds %d incremental flow\n",source,increment);
  }
  
  max_flow += increment;
  
}
printf("%d augmenting paths\n",APcount);

free(color);
free(pred);
free(predEdge);
free(q);

return max_flow;
}


int tailThenHead(const void* xin, const void* yin)

{
int result;
edgeType *x,*y;

x=(edgeType*) xin;
y=(edgeType*) yin;
result=x->tail - y->tail;
if (result!=0)
  return result;
else
  return x->head - y->head;
}


void read_input_file()
{
int tail,head,capacity,i,j;
int inputEdges;     
int workingEdges;   
                    
edgeType work;
edgeType *ptr;
clock_t startCPU,stopCPU;
double cpu_time;


scanf("%d %d",&n,&inputEdges);

edgeTab=(edgeType*) malloc(2*inputEdges*sizeof(edgeType));
if (!edgeTab)
{
  printf("edgeTab malloc failed %d\n",__LINE__);
  exit(0);
}
// read edges, each with a capacity
workingEdges=0;
for (i=0; i<inputEdges; i++)
{
  scanf("%d %d %d",&tail,&head,&capacity);
  
  // Test for wrong edge, including incoming to source and outgoing from sink
  
  if (tail<0 || tail>=n-1 || head<1 || head>=n || capacity<=0)
  {
    printf("Invalid input %d %d %d at %d\n",tail,head,capacity,__LINE__);
    exit(0);
  }
  
  edgeTab[workingEdges].tail=tail;
  edgeTab[workingEdges].head=head;
  edgeTab[workingEdges].capacity=capacity;
  workingEdges++;
  
  edgeTab[workingEdges].tail=head;
  edgeTab[workingEdges].head=tail;
  edgeTab[workingEdges].capacity=0;
  workingEdges++;
}


startCPU=clock();
qsort(edgeTab,workingEdges,sizeof(edgeType),tailThenHead);
stopCPU=clock();
cpu_time = ((double) (stopCPU - startCPU)) / CLOCKS_PER_SEC;
printf("qsort CPU %f\n",cpu_time);

// Coalesce parallel edges into a single edge by adding their capacities.
residualEdges=0;
for (i=1; i<workingEdges; i++)
  if (edgeTab[residualEdges].tail==edgeTab[i].tail
  &&  edgeTab[residualEdges].head==edgeTab[i].head)
    edgeTab[residualEdges].capacity+=edgeTab[i].capacity;  // || case
  else
  {
    residualEdges++;
    edgeTab[residualEdges].tail=edgeTab[i].tail;
    edgeTab[residualEdges].head=edgeTab[i].head;
    edgeTab[residualEdges].capacity=edgeTab[i].capacity;
  }
residualEdges++;

// Set field in each edgeTab struct to point to inverse
startCPU=clock();
for (i=0; i<residualEdges; i++)
{
  work.tail=edgeTab[i].head;
  work.head=edgeTab[i].tail;
  ptr=(edgeType*) bsearch(&work,edgeTab,residualEdges,sizeof(edgeType),
    tailThenHead);
  if (ptr==NULL)
  {
    printf("bsearch %d failed %d\n",i,__LINE__);
    exit(0);
  }
  edgeTab[i].inverse=ptr-edgeTab;  // ptr arithmetic to get subscript
}
stopCPU=clock();
cpu_time = ((double) (stopCPU - startCPU)) / CLOCKS_PER_SEC;
printf("set inverses CPU %f\n",cpu_time);



firstEdge=(int*) malloc((n+1)*sizeof(int));
if (!firstEdge)
{
  printf("malloc failed %d\n",__LINE__);
  exit(0);
}
j=0;
for (i=0; i<n; i++)
{
  firstEdge[i]=j;
  // Skip over edges with vertex i as their tail.
  for ( ;
       j<residualEdges && edgeTab[j].tail==i;
       j++)
    ;
}
firstEdge[n]=residualEdges;  //Sentinel

}
int main ()
{
int i,j;
clock_t startCPU,stopCPU;
double cpu_time;
read_input_file();
startCPU=clock();
printf("total flow is %d\n",max_flow(0,n-1));  // 0=source, n-1=sink
stopCPU=clock();
cpu_time = ((double) (stopCPU - startCPU)) / CLOCKS_PER_SEC;

printf("Ford-Fulkerson time %f\n",cpu_time);

free(edgeTab);
free(firstEdge);
}
