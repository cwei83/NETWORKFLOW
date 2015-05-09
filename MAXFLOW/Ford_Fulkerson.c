#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#define num_node 6

typedef struct adlist *adPointer;
typedef struct adlist {
    int vertex;
    int capacity;
    adPointer link;
} adlist;

typedef struct queue *queuePointer;
typedef struct queue {
    int vertex;
    queuePointer link;
} queue;

void addq(int vertex);
int deleteq(void);
void addList(int i, int j, int n);
queuePointer bfs(int start, int end);
void ff(int start, int end);
void residual(int start, int end, int min_flow);

adPointer graph[num_node+1];
queuePointer front, rear;

int main() {

    FILE *fin;
    int x, y, z;

    for(int i=1;i<num_node+1;i++)
	graph[i]=NULL;

    fin = fopen("input.txt", "r");

    while(fscanf(fin, "%d %d %d", &x, &y, &z)==3) 
	addList(y, x, z);

    fclose(fin);
    ff(1,6);

    return 0;
}

void addList(int i, int j, int n) {
    adPointer ptr, pre;

    if(!graph[j]) {
	graph[j]=malloc(sizeof(adlist));
	graph[j]->link=NULL;
	graph[j]->vertex=i;
	graph[j]->capacity=n;
    } else {
	ptr=graph[j];
	for(;ptr->link!=NULL;ptr=ptr->link)
	    ;
	pre=malloc(sizeof(adlist));
	pre->link=NULL;
	pre->vertex=i;
	pre->capacity=n;
	ptr->link=pre;
    }
}

void addq(int vertex) {
    queuePointer temp;
    
    temp = malloc(sizeof(queue));
    temp->vertex=vertex;
    temp->link=NULL;

    if(front)
	rear->link=temp;
    else
	front=temp;
    rear=temp;
}

int deleteq(void) {
    queuePointer temp = front;
    int vertex;

    front = temp->link;
    vertex = temp->vertex;
    free(temp);

    return vertex;
}

queuePointer bfs(int start, int end) {
    bool visited[num_node+1];
    int pre[num_node+1];
    adPointer node;
    queuePointer bfs_start, temp;
    int pre_index=end, vertex=start;
    front = rear = NULL;

    for(int i=1;i<num_node+1;i++){
	visited[i]=0; 
	pre[i]=-1;
    }

    visited[vertex]=1;
    addq(vertex);

    while(front) {
	vertex=deleteq();
	for(node=graph[vertex];node;node=node->link)
	    if(!visited[node->vertex]&&node->capacity>0) {
		pre[node->vertex]=vertex;
		addq(node->vertex);
		visited[node->vertex]=1;
	    }
    }

    if(pre[pre_index]==-1)
	bfs_start=NULL;
    else {
	temp = malloc(sizeof(queue));
	temp->vertex = end;
	temp->link = bfs_start;
	bfs_start = temp;
	while(pre[pre_index]!=-1) {
	    temp = malloc(sizeof(queue));
	    temp->vertex = pre[pre_index];
	    temp->link = bfs_start;
	    bfs_start = temp;
	    pre_index = pre[pre_index];
	}
    }
    return bfs_start;
}

void ff(int start, int end) {

    int min_flow, max_flow=0;
    int bfs_start, bfs_end;
    queuePointer bfs_head=bfs(start, end);
    queuePointer ptr;
    adPointer pre;

    while(bfs_head) {
	//search min flow
	min_flow = INT_MAX;
	ptr = bfs_head;
	bfs_start = ptr->vertex;
	ptr = ptr->link;
	for(;ptr;ptr=ptr->link) {
	    bfs_end = ptr->vertex;
	    pre=graph[bfs_start];
	    for(;pre;pre=pre->link) {
		if(pre->vertex==bfs_end)
		    if(pre->capacity<min_flow) {
			min_flow = pre->capacity;
			break;
		    }
	    }
	    bfs_start = bfs_end;
	    if(bfs_start==end)
		break;
	}
	//calculate max_flow
	max_flow += min_flow;

	//deliver min_flow from start to end
	ptr = bfs_head;
	bfs_start = ptr->vertex;
	ptr = ptr->link;
	for(;ptr;ptr=ptr->link) {
	    bfs_end = ptr->vertex;
	    pre=graph[bfs_start];
	    for(;pre;pre=pre->link) {
		if(pre->vertex==bfs_end) {
		    pre->capacity -= min_flow;
		    //create residual arc = min_flow
		    residual(bfs_start, bfs_end, min_flow);
		}
	    }
	    bfs_start = bfs_end;
	    if(bfs_start==end)
		break;
	}
	bfs_head = bfs(start, end);
    }

    printf("MAX FLOW FROM %d TO %d = %d\n", start, end, max_flow);

}

void residual(int start, int end, int min_flow) {

    adPointer ptr, pre;
    for(ptr=graph[end];ptr;ptr=ptr->link) {
	if(ptr->vertex==start) {
	    ptr->capacity += min_flow;
	    return;
	}
    }
    if(!graph[end]) {
	graph[end]=malloc(sizeof(adlist));
	graph[end]->link=NULL;
	graph[end]->vertex=start;
	graph[end]->capacity=min_flow;
    } else {
	for(ptr=graph[end];ptr->link!=NULL;ptr=ptr->link)
	    ;
	pre = malloc(sizeof(adlist));
	pre->link=NULL;
	pre->vertex=start;
	pre->capacity=min_flow;
	ptr->link=pre;
    }

}


