#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#define num_node 5

typedef struct element {
    int s_t;
    int e_t;
    int cost;
} element;

/*FOR KRUSKAL's ALGORITHM*/
void kruskal(void);
void checkSet(int start, int end);

/*FOR MIN HEAP*/
void push(element item);
element pop();

/*INITIALIZING VARIABLES*/
int heap_index = 0;
int set[num_node+1][num_node+1];
element heap[num_node+1];
bool gvisit[num_node+1];
bool check_checkSet;

int main() {

    FILE *fin;
    int _start, _end, _cost;
    element temp;
    
    fin = fopen("input.txt", "r");

    while(fscanf(fin, "%d %d %d", &_start, &_end, &_cost)==3) {
	temp.s_t = _start;
	temp.e_t = _end;
	temp.cost = _cost;
	push(temp);
    }

    fclose(fin);

    kruskal();

    return 0;

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

void kruskal(void) {
    element temp;
    int count=0, start, end;
    int cost=0;
    bool check_cycle=0;

    for(int i=1;i<num_node+1;i++)
	for(int j=1;j<num_node+1;j++)
	    set[i][j]=0;

    temp = pop();
    while(count<num_node-1) {

	//printf("pop out: %d %d %d\n", temp.s_t, temp.e_t, temp.cost);

	start = temp.s_t;
	end = temp.e_t;

	for(int i=1;i<num_node+1;i++)
	    gvisit[i] = 0;

	check_checkSet=0;
	checkSet(start, end);
	if(check_checkSet==0) {
	    set[start][end] = 1;
	    set[end][start] = 1;
	    count++;
	    cost += temp.cost;
	    printf("%d %d %d\n", temp.s_t, temp.e_t, temp.cost);
	}
	temp = pop();
    }
    printf("Minimum Spanning Tree Cost: %d\n", cost);
}

void checkSet(int start, int end) {
    //printf("start=%d end=%d\n", start, end);
    for(int i=1;i<num_node+1;i++) {
	if(set[start][i]==1&&gvisit[i]==0) {
	    gvisit[i]=1;
	    if(i==end)
		check_checkSet=1;
	    checkSet(i, end);
	}
    }
}
