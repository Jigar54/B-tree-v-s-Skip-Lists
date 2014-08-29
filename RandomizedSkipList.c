#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

#define MAX_LEVEL 16
#define P 0.5

struct sn {
	int value;
	int height;
	struct sn ** forward;
};

typedef struct sn SkipNode; 
typedef struct {
	SkipNode *header;
	int level;
}SkipSet;

int val[100000000];

float frand() {
	return (float) rand() / RAND_MAX;
}

int random_level() {
	static int first=1;
	int lvl=0;
	if(first) {
		srand((unsigned)time( NULL ));
		first=0;
	}
	while(frand() < P && lvl < MAX_LEVEL)
		lvl++;
//	printf("Random Level Generated : %d\n", lvl);
	return lvl;
}

SkipNode* make_node(int level, int value) {
	SkipNode *sn = (SkipNode *)malloc(sizeof(SkipNode));
	sn->forward = (SkipNode **)malloc(sizeof(SkipNode *)*(level+1));
	sn->value = value;
	sn->height = level;
	return sn;
}

SkipSet* make_skipset() {
	SkipSet* ss = (SkipSet *)malloc(sizeof(SkipSet));
	ss->header = make_node(MAX_LEVEL, -1000000000);
	ss->level = 0;
	return ss;
}

void print_skipset(SkipSet *ss) {
	SkipNode *x = ss->header->forward[0];
	printf("Set => {");
	while(x!=NULL) {
		printf("%d", x->value);
		x = x->forward[0];
		if(x!=NULL)
			printf(",");
	}
	printf("}\n");
	x = ss->header->forward[0];
	printf("Heights => {");
	while(x!=NULL) {
		printf("%d", x->height);
		x = x->forward[0];
		if(x!=NULL)
			printf(",");
	}
	printf("}\n");
}

int contains(SkipSet *ss, int search_value) {
	int i;
	SkipNode *x = ss->header;
	for(i=ss->level; i>=0; i--) {
		while(x->forward[i]!=NULL && x->forward[i]->value < search_value) {
			x = x->forward[i];
		}
	}
	x = x->forward[0];
	if(x!=NULL && x->value == search_value) {
		int j=1;
		x = ss->header;
//		printf("%d\n", x->forward[0]->value);
		while(x->forward[0]!=NULL && x->forward[0]->value < search_value) {
			j++;
			x = x->forward[0];
		}
		return j;
	}
	return 0;
}

void insert(SkipSet *ss, int value) {
	int i;
	SkipNode *x = ss->header;
	SkipNode *update[MAX_LEVEL+1];
	memset(update, 0, MAX_LEVEL+1);
	for(i=ss->level; i>=0; i--) {
		while(x->forward[i]!=NULL && x->forward[i]->value < value) {
			x = x->forward[i];
		}
		update[i] = x;
	}
//	int j;
//	for(j=0; j<ss->level+1; j++)
//		printf("%d ", update[j]->value);
//	printf("\n");
	x = x->forward[0];
	if(x == NULL || x->value != value) {
		int lvl = random_level();
		if(lvl > ss->level) {
			for(i=ss->level+1; i<=lvl; i++) {
				update[i] = ss->header;
			}
			ss->level = lvl;
		}
		x = make_node(lvl, value);
		for(i=0; i<=lvl; i++) {
			x->forward[i] = update[i]->forward[i];
			update[i]->forward[i] = x;
		}
	}
	val[value] = 1;
//	for(j=0; j<ss->level+1; j++)
//		printf("%d ", update[j]->value);
//	printf("\n");
//	print_skipset(ss);
}

void delete(SkipSet *ss, int value) {
	if(val[value] == 0) {
		printf("Fool, how can I delete %d if it's not Inserted yet!!\n", value);
		return;
	}
	int i;
	SkipNode *x = ss->header;
	SkipNode *update[MAX_LEVEL+1];
	memset(update, 0, MAX_LEVEL+1);
	for(i=ss->level; i>=0; i--) {
		while(x->forward[i]!=NULL && x->forward[i]->value < value) {
			x = x->forward[i];
		}
		update[i] = x;
	}
//	int j;
//	for(j=0; j<ss->level+1; j++)
//		printf("%d ", update[j]->value);
//	printf("\n");
	x = x->forward[0];
	if(x->value == value) {
		for(i=0; i<=ss->level; i++) {
			if(update[i]->forward[i]!=x)
				break;
			update[i]->forward[i] = x->forward[i];
		}
		free(x);
		while(ss->level > 0 && ss->header->forward[ss->level] == NULL) {
			ss->level--;
		}
	}
/*	FOR TESTING	*/
//	for(j=0; j<ss->level+1; j++)
//		printf("%d ", update[j]->value);
//	printf("\n");
}

int main() {
	SkipSet *ss = make_skipset();
	int a, x;
	char c;
	printf("(In each command x = number of numbers wanted to insert, delete or search)\n1.Insert command format => Ix and then the numbers\n2.Delete command format => Dx and then the numbers\n3.Search command format => Fx and then the numbers\n4.Print command format => P\nPress Q and then enter to quit\n");
	memset(val, 0, 100000000);
	while((c=getchar())!='Q')
	{
		switch(c)
		{
			case 'I':
				scanf("%d", &x);
				while(x--) {
					scanf("%d", &a);
					insert(ss, a);
				}
				break;
			case 'D':
				scanf("%d", &x);
				while(x--) {
					scanf("%d", &a);
					delete(ss, a);
				}
				break;
			case 'F': 
				scanf("%d", &x);
				while(x--) {
					scanf("%d", &a);
					if(contains(ss, a))
						printf("%d is there in the Set at position : %d\n", a, contains(ss, a));
					else
						printf("%d is not in the Set!\n", a);
				}
				break;
			case 'P':
				print_skipset(ss);
				break;
		}
	}
/*	TEST CASE	*/
//	print_skipset(ss);
//	insert(ss, 5);
//	insert(ss, 10);
//	insert(ss, 7);
//	insert(ss, 7);
//	insert(ss, 6);
//	print_skipset(ss);
//	if(contains(ss, 7)) {
//		printf("7 is in the list\n");
//	}
//	delete(ss, 7);
//	print_skipset(ss);
//	if(!contains(ss, 7)) {
//		printf("7 has been deleted\n");
//	}
	return 0;
}
