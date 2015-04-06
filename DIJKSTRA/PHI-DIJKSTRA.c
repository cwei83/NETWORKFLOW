#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#define num_node 13390

typedef struct adlist *adPointer;
typedef struct adlist {
    int vertex;
    int weight;
    adPointer link;
} adlist;

typedef struct element {
    int vertex;
    int weight;
} element;

void push(element item);
element pop();

void addList(int i, int j, int n);
void write(int pre[], int dis[]);
void dijkstra(int vertex, int goal);

adPointer graph[num_node+1];
element heap[num_node];

int heap_index=0;

int main() {

    FILE *fin;
    char input;
    int n=0, i=0, j=1, root=1, goal;
    int x, y, z, check=0;

    for(int x=1;x<num_node+1;x++)
	graph[x]=NULL;

    fin = fopen("Philadelphia_network.txt", "r");

    while(fscanf(fin, "%d %d %d", &x, &y, &z)==3)
	addList(y, x, z);

    fclose(fin);

//    printf("Enter your root: ");
//    scanf("%d", &root);
//    printf("Enter your goal: ");
//    scanf("%d", &goal);

    root=1; goal=1;
    dijkstra(root, goal);

    return 0;

}

void addList(int i, int j, int n) {
    adPointer ptr, pre;

    if(!graph[j]) {
	graph[j]=malloc(sizeof(adlist));
	graph[j]->link=NULL;
	graph[j]->vertex=i;
	graph[j]->weight=n;
    } else {
	ptr=graph[j];
	for(;ptr->link!=NULL;ptr=ptr->link)
	    ;
	pre=malloc(sizeof(adlist));
	pre->link=NULL;
	pre->vertex=i;
	pre->weight=n;
	ptr->link=pre;
    }
}

element pop() {
    element item, temp;
    int parent, child;

    item = heap[1];
    temp = heap[heap_index--];
    parent = 1;
    child = 2;

    while(child<=heap_index) {
	if(child<heap_index&&heap[child].weight>heap[child+1].weight)
	    child++;
	if(temp.weight<=heap[child].weight)
	    break;
	heap[parent] = heap[child];
	parent = child;
	child*=2;
    }

    heap[parent] = temp;
    return item;
}

void push(element item) {
    int i=++heap_index;

    while(i!=1&&item.weight<heap[i/2].weight) {
	heap[i] = heap[i/2];
	i/=2;
    }

    heap[i] = item;
}

void dijkstra(int vertex, int goal) {
    int distance[num_node+1], pre[num_node+1];
    bool visit[num_node+1];
    element temp, top;
    adPointer ptr;
    int current_vertex, current_weight, temp_pre;

    for(int i=0;i<num_node+1;i++) {
	distance[i] = INT_MAX;
	visit[i] = 0;
	pre[i] = 0;
    }

//    visit[vertex] = 1;
    distance[vertex] = 0;
    temp.vertex = vertex;
    temp.weight = 0;

    push(temp);
    while(heap_index!=0) {
	top = pop();
	current_vertex = top.vertex;
	current_weight = top.weight;
	visit[current_vertex]=1;
//	write(pre, distance);

	for(ptr=graph[current_vertex];ptr;ptr=ptr->link) 
	    if(visit[ptr->vertex]==0) {
//		printf("ptr->vertex=%d\n", ptr->vertex);
		if(distance[current_vertex]+ptr->weight<distance[ptr->vertex]) {
		    distance[ptr->vertex]=distance[current_vertex]+ptr->weight;
		    //printf("distance[%d]=%d distance[%d]=%d\n", current_vertex, distance[current_vertex], ptr->vertex, distance[ptr->vertex]);
		    //printf("distance[%d]+%d<distance[%d]\n", current_vertex, ptr->weight, ptr->vertex);
		    pre[ptr->vertex]=current_vertex;
		
		temp.vertex = ptr->vertex;
		temp.weight = distance[ptr->vertex];
		push(temp);
		}
	    }
    }

    for(int i=1;i<num_node+1;i++) 
	printf("distance[%d] = %d\n", i, distance[i]);
/*	
    printf("distance= %d\n", distance[goal]);
    temp_pre = pre[goal];
    printf("%3d", goal);
    while(pre[temp_pre]!=0) {
	printf("%8d", temp_pre);
	temp_pre = pre[temp_pre];
    }
    printf("\n");
*/
}

void write(int pre[], int dis[]) {

    printf("\nd(i): ");
    for(int i=1;i<num_node+1;i++)
	printf("%5d", dis[i]);
    printf("\np(i): ");
    for(int i=1;i<num_node+1;i++)
	printf("%5d", pre[i]);
    printf("\n");

}
