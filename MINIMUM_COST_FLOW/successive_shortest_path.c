#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#define num_node 6

typedef struct adlist *adPointer;
typedef struct adlist {
    int vertex;
    int capacity;
    int cost;
    adPointer link;
} adlist;

typedef struct queue *queuePointer;
typedef struct queue {
    int vertex;
    queuePointer link;
} queue;

typedef struct element {
    int vertex;
    int cost;
} element;

/* FOR DIJKSTRA */
void push(element item);
element pop();
queuePointer dijkstra(int start, int end);

/* FOR NETWORK */
void addList(int i, int j, int n, int k);
void residual(int start, int end, int min_flow);

/* MIN COST FLOW */
void ssp(int start, int end, int flow);

/* INITIALIZING VARIABLES */
int heap_index = 0;
int pi[num_node+1];
int total_cost = 0;
int original_distance[num_node+1][num_node+1];
adPointer graph[num_node+1];
element heap[num_node];

int main() {

    FILE *fin;
    int _start, _end, _capacity, _cost;
    int n=0, i=0, j=1;

    for(int i=1;i<num_node+1;i++) {
	pi[i] = 0;
	graph[i] = NULL;
    }

    fin = fopen("input.txt", "r");

    while(fscanf(fin, "%d %d %d %d", &_start, &_end, &_capacity, &_cost)==4) {
	addList(_end, _start, _capacity, _cost);
	original_distance[_start][_end] = _cost;
	original_distance[_end][_start] = _cost*-1;
    }

    fclose(fin);

    printf("SEND FLOW=9 FROM NODE1 TO NODE6 BY DEFAULT\n");

    ssp(1, 6, 9);

    return 0;

}

void addList(int i, int j, int n, int k) {
    adPointer ptr, pre;

    if(!graph[j]) {
	graph[j] = malloc(sizeof(adlist));
	graph[j]->link = NULL;
	graph[j]->vertex = i;
	graph[j]->capacity = n;
	graph[j]->cost = k;
    } else {
	for(ptr=graph[j];ptr->link!=NULL;ptr=ptr->link)
	    ;
	pre = malloc(sizeof(adlist));
	pre->link = NULL;
	pre->vertex = i;
	pre->capacity = n;
	pre->cost = k;
	ptr->link = pre;
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
	if(child<heap_index&&heap[child].cost>heap[child+1].cost)
	    child++;
	if(temp.cost<=heap[child].cost)
	    break;
	heap[parent] = heap[child];
	parent = child;
	child *= 2;
    }

    heap[parent] = temp;
    return item;
}

void push(element item) {
    int i = ++heap_index;

    while(i!=1&&item.cost<heap[i/2].cost) {
	heap[i] = heap[i/2];
	i/=2;
    }

    heap[i] = item;
}

queuePointer dijkstra(int start, int end) {
    int d[num_node+1], pre[num_node+1];
    bool visit[num_node+1];
    element temp, top;
    adPointer ptr;
    int current_vertex, current_cost;

    /* initialize queue */
    queuePointer dijkstra_start, qtemp;
    int pre_index=end, vertex=start;

    for(int i=1;i<num_node+1;i++) {
	d[i] = INT_MAX;
	visit[i] = 0;
	pre[i] = -1;
    }

    d[start] =0;
    temp.vertex = start;
    temp.cost = 0;
    push(temp);

    while(heap_index!=0) {
	top = pop();
	current_vertex = top.vertex;
	current_cost = top.cost;
	visit[current_vertex] = 1;

	for(ptr=graph[current_vertex];ptr;ptr=ptr->link)
	    if(visit[ptr->vertex]==0)
		if(d[current_vertex]+ptr->cost<d[ptr->vertex]&&ptr->capacity>0) {
		    d[ptr->vertex] = d[current_vertex]+ptr->cost;
		    pre[ptr->vertex] = current_vertex;

		    temp.vertex = ptr->vertex;
		    temp.cost = d[ptr->vertex];
		    push(temp);
		}
    }

    if(pre[pre_index]==-1)
	dijkstra_start = NULL;
    else {
	qtemp = malloc(sizeof(queue));
	qtemp->vertex = end;
	qtemp->link = dijkstra_start;
	dijkstra_start = qtemp;
	while(pre[pre_index]!=-1) {
	    qtemp = malloc(sizeof(queue));
	    qtemp->vertex = pre[pre_index];
	    qtemp->link = dijkstra_start;
	    dijkstra_start = qtemp;
	    pre_index = pre[pre_index];
	}
    }

    for(int i=1;i<num_node+1;i++)
	pi[i] = pi[i] - d[i];

    return dijkstra_start;
}

void residual(int start, int end, int min_flow) {
    adPointer ptr, pre;

    total_cost += min_flow*original_distance[start][end];

    for(ptr=graph[end];ptr;ptr=ptr->link) {
	if(ptr->vertex==start) {
	    ptr->capacity += min_flow;
	    return;
	}
    }
    if(!graph[end]) {
	graph[end] = malloc(sizeof(adlist));
	graph[end]->link = NULL;
	graph[end]->vertex = start;
	graph[end]->capacity = min_flow;
    } else {
	for(ptr=graph[end];ptr->link!=NULL;ptr=ptr->link)
	    ;
	pre = malloc(sizeof(adlist));
	pre->link = NULL;
	pre->vertex = start;
	pre->capacity = min_flow;
	ptr->link = pre;
    }
}

void ssp(int start, int end, int flow) {
    int min_flow, max_flow=0;
    int dijkstra_start, dijkstra_end;
    queuePointer dijkstra_head = dijkstra(start, end);
    queuePointer ptr;
    adPointer pre;

    while(dijkstra_head) {

	min_flow=0;

	//UPDATE COST PI
	for(int i=1;i<num_node+1;i++)
	    for(adPointer temp=graph[i];temp;temp=temp->link)
		temp->cost = original_distance[i][temp->vertex] - pi[i] + pi[temp->vertex];

	printf("SEND FLOW ALONG ");
	
	//SEARCH MIN FLOW
	min_flow = INT_MAX;
	ptr = dijkstra_head;
	dijkstra_start = ptr->vertex;
	ptr = ptr->link;
	for(;ptr;ptr=ptr->link) {
	    printf("%d ", dijkstra_start);
	    dijkstra_end = ptr->vertex;
	    pre = graph[dijkstra_start];
	    for(;pre;pre=pre->link) {
		if(pre->vertex==dijkstra_end)
		    if(pre->capacity<min_flow) {
			min_flow = pre->capacity;
			break;
		    }
	    }
	    dijkstra_start = dijkstra_end;
	    if(dijkstra_start == end)
		break;
	}
	
	if(max_flow+min_flow>flow)
	    min_flow = flow-max_flow;

	printf("%d FLOW=%d\n", dijkstra_start, min_flow);

	//CALCULATE MAX_FLOW
	max_flow += min_flow;

	//DELIVER MIN_FLOW ALONG DIJKSTRA PATH
	ptr = dijkstra_head;
	dijkstra_start = ptr->vertex;
	ptr = ptr->link;
	for(;ptr;ptr=ptr->link) {
	    dijkstra_end = ptr->vertex;
	    pre = graph[dijkstra_start];
	    for(;pre;pre=pre->link) {
		if(pre->vertex==dijkstra_end) {
		    pre->capacity -= min_flow;
		    //create residual arc = min_flow
		    residual(dijkstra_start, dijkstra_end, min_flow);
		}
	    }
	    dijkstra_start = dijkstra_end;
	    if(dijkstra_start==end)
		break;
	}

	printf("maxflow = %d\n", max_flow);

	if(max_flow>=flow)
	    break;
	dijkstra_head = dijkstra(start, end);
    }

    if(max_flow>=flow)
	printf("TOTOAL COST = %d\n", total_cost);
    else
	printf("INFEASIBLE\n");
}
