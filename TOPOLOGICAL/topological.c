#include <stdio.h>
#include <stdlib.h>
#define num_node 8

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
void topological(void);

adPointer graph[num_node+1];
int indegree[num_node+1];
queuePointer front, rear;

int main() {

    FILE *fin;
    char input;
    int n=0, i=0, j=1;

    for(int x=1;x<num_node+1;x++)
	graph[x]=NULL;

    fin = fopen("input_topological.txt", "r");
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

    topological();

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
    indegree[i]++;
}

void addq(int vertex) {
    queuePointer temp;

    temp=malloc(sizeof(queue));
    temp->vertex=vertex;
    temp->link=NULL;

    if(front)
	rear->link=temp;
    else
	front=temp;
    rear=temp;
}

int deleteq(void) {
    queuePointer temp=front;
    int vertex;

    front = temp->link;
    vertex=temp->vertex;
    free(temp);

    return vertex;
}

void topological(void) {
    int vertex, temp, j=1;
    adPointer ptr;
    front = rear = NULL;

    for(int i=1;i<num_node+1;i++)
	if(indegree[i]==0)
	    addq(i);

    while(front) {
	vertex=deleteq();
	printf("Sequence %d : vertex %2d\n", j, vertex);
	j++;
	for(ptr=graph[vertex];ptr;ptr=ptr->link) {
	    temp=ptr->vertex;
	    indegree[temp]--;
	    if(indegree[temp]==0) 
		addq(temp);
	}
    }
}
