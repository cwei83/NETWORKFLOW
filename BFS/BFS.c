#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define num_node 9

typedef struct adlist *adPointer;
typedef struct adlist {
    int vertex;
    adPointer link;
} adlist;

typedef struct queue *queuePointer;
typedef struct queue {
    int vertex;
    queuePointer link;
} queue;

void addq(int vertex);
int deleteq(void);

void addList(int i, int j);

void bfs(int vertex);

adPointer graph[num_node+1];
queuePointer front, rear;

int main() {

    FILE *fin;
    char input;
    int n=0, i=0, j=1, root=1;

    for(int x=1;x<num_node+1;x++)
	graph[x]=NULL;

    fin = fopen("input_bfs.txt", "r");
    input=fgetc(fin);


    while(1) {
	if(input==' ') {
	    i++;
	    if(n>0) {
		addList(i,j);
		n=0;
	    }
	}
	else if(input=='1')
	    n=1;
	else if(input=='\n') {
	    i++; 
	    if(n>0) {
		addList(i,j);
		n=0;
	    }
	    if(i==j)
		break;
	    i=0; j++;
	}

	input=fgetc(fin);
    }
    fclose(fin);

    bfs(root);

    return 0;

}

void addList(int i, int j) {
    adPointer ptr, pre;

    if(!graph[j]) {
	graph[j]=malloc(sizeof(adlist));
	graph[j]->link=NULL;
	graph[j]->vertex=i;
    } else {
	ptr=graph[j];
	for(;ptr->link!=NULL;ptr=ptr->link)
	    ;
	pre=malloc(sizeof(adlist));
	pre->link=NULL;
	pre->vertex=i;
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

void bfs(int vertex) {
    bool visited[num_node+1];
    int pre[num_node+1];
    adPointer node;
    front = rear = NULL;

    for(int i=1;i<num_node+1;i++){
	visited[i]=0; 
	pre[i]=-1;
    }

    printf("Visted Order: %3d", vertex);
    visited[vertex]=1;
    addq(vertex);

    while(front) {
	vertex=deleteq();
	for(node=graph[vertex];node;node=node->link)
	    if(!visited[node->vertex]) {
		pre[node->vertex]=vertex;
		printf("%3d", node->vertex);
		addq(node->vertex);
		visited[node->vertex]=1;
	    }
    }
    printf("\n");
    for(int i=1;i<num_node+1;i++)
	if(pre[i]!=-1)
	    printf("The Preceder Of %d is %d\n", i, pre[i]);
}
