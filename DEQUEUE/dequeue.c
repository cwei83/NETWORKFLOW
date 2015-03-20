#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#define num_node 10

typedef struct adlist *adPointer;
typedef struct adlist {
    int vertex;
    int weight;
    adPointer link;
} adlist;

typedef struct queue *queuePointer;
typedef struct queue {
    int vertex;
    queuePointer link;
} queue;

void addq(int vertex);
void dq(int vertex);
int deleteq(void);

void addList(int i, int j, int n);

void shortestpath(int vertex);
void write(int pre[], int dis[], int s[]);

adPointer graph[num_node+1];
queuePointer front, rear;

int main() {

    FILE *fin;
    char input;
    int n=0, i=0, j=1, root=1, check=0;

    for(int x=1;x<num_node+1;x++)
	graph[x]=NULL;

    fin = fopen("input.txt", "r");
    input=fgetc(fin);

    while(1) {
	if(input<='9'&&input>='0') {
	    n*=10;
	    n+=input-'0';
	}
	if(input==' ') {
	    if(check==1)
		n*=-1;
	    if(n!=0) { 
		addList(i+1,j,n);
		//printf("i=%d j=%d n=%d\n", i+1, j , n);
	    }
	    n=0; i++; check=0;
	}
	if(input=='\n') {
	    if(check==1)
		n*=-1;
	    if(n!=0) {
		addList(i+1,j,n);
		//printf("i=%d j=%d n=%d\n", i+1, j , n);
	    }
	    i++; j++;
	    if(i==j)
		break;
	    n=0; i=0; check=0;
	}
	if(input=='-') 
	    check=1;
	input=fgetc(fin);
    }

    fclose(fin);

    shortestpath(1);

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

void dq(int vertex) {
    queuePointer temp;
    
    temp = malloc(sizeof(queue));
    temp->vertex=vertex;
    temp->link=front;
    front=temp;
}

int deleteq(void) {
    queuePointer temp = front;
    int vertex;

    front = temp->link;
    vertex = temp->vertex;
    free(temp);

    return vertex;
}

void shortestpath(int vertex) {
    int current_vertex;
    int pre[num_node+1], s[num_node+1], dis[num_node+1];
    adPointer ptr;
    front = rear = NULL;

    for(int i=1;i<num_node+1;i++) {
	pre[i]=-1;
	s[i]=0;
	dis[i]=INT_MAX;
    }

    dis[vertex] = 0;
    addq(vertex);

    while(front) {
	current_vertex = deleteq();
	//s[current_vertex] = -1;
	//printf("s[%d] = %d\n", current_vertex, s[current_vertex]);
	write(pre, dis, s);
	for(ptr=graph[current_vertex];ptr;ptr=ptr->link) {
	    //if(s[ptr->vertex]==0) {
		if(dis[current_vertex]+ptr->weight<dis[ptr->vertex]) {
		    dis[ptr->vertex]=dis[current_vertex]+ptr->weight;
		    pre[ptr->vertex]=current_vertex;
		    //printf("dis[%d]=%d dis[%d]=%d\n", current_vertex, dis[current_vertex], ptr->vertex, dis[ptr->vertex]);
		    //printf("dis[%d]+%d<dis[%d]\n", current_vertex, ptr->weight, ptr->vertex);
	   	 }
		addq(ptr->vertex);
	    //}
	    //else {
	    if(s[ptr->vertex]==-1) {
		dq(ptr->vertex);
		s[ptr->vertex] = 0;
	    }
	}
	s[current_vertex] = -1;

    }

    //for (int i = 1; i < num_node+1; i++) {
    	//printf("dis[%d] = %d\n", i, dis[i]);
    //}
}

void write(int pre[], int dis[], int s[]) {

    printf("\nd(i): ");
    for(int i=1;i<num_node+1;i++)
	printf("%5d", dis[i]);
    printf("\np(i): ");
    for(int i=1;i<num_node+1;i++)
	printf("%5d", pre[i]);
    printf("\ns(i): ");
    for(int i=1;i<num_node+1;i++)
	printf("%5d", s[i]);
    printf("\n");

}
