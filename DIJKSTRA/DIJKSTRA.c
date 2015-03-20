#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define num_node 9

typedef struct adlist *adPointer;
typedef struct adlist {
	int vertex;
	int weight;
	adPointer link;
} adlist;

void addList(int i, int j, int n);

void dijkstra(int vertex);

adPointer graph[num_node+1];

int main() {

	FILE *fin;
	char input;
	int n=0, i=0, j=1, root=1;

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
			if(n>0)
				addList(i+1,j,n);
			n=0; i++;
		}
		if(input=='\n') {
			if(n>0) {
				printf("i=%d j=%d n=%3d\n", i, j, n);
//				printf("\n");
				addList(i+1,j,n);
			}
			i++; j++;
			if(i==j) 
				break;
			n=0; i=0;
		}
		input=fgetc(fin);
	}

	fclose(fin);

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

