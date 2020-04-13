#include<math.h>
#include<stdlib.h>
#include<stdio.h>
#include "header.h"

int trcount(struct pos* pt,char** a)
{
	int c=0;	
	for(int i=0;i<pt->size;i++)
	{
		if(a[pt->x[i]][pt->y[i]]=='-')
			c+=1;
	}
	return c;
}

// 0 not over, ,1 means comp won, -1 means human
int gameover(char **board, int size, int turn)
{
	int w,b;
	count_pawns(SIZE,board,&w,&b);
	if(is_empty(size,board) == 0)
	{
		if(w == b)
			return 3;
		if(w>b)
			return -1;
		return 1;
	}
	if(turn == 1)
	{
		struct pos *legal = get_neigh(SIZE,board,'B');
		if(legal->size == 0)
			return -1;
		return 0;
	}
	if(turn == -1)
	{
		struct pos *legal = get_neigh(SIZE,board,'W');
		if(legal->size == 0)
			return 1;
		return 0;
	}
}

Node* create_node(Node* parent, char** state)
{
	Node *node = (Node*)malloc(sizeof(Node));
	node->reward = 0.0;
	node->visits = 1;
	node->parent = parent;
	node->len_child = 0;
	node->chmov = 0;
	node->children = NULL;
	node->children_move = NULL;
	node->state = state;
	/*node->state = (grid*)malloc(sizeof(grid));
	node->state->status = 0;
	for(int i = 0; i<7 ; i++)
	{
		node->state->top[i] = state->top[i];
		for(int j = 0; j<6 ;j++)
			node->state->gr[i][j] = state->gr[i][j];
	}*/

	node->children_move = NULL;
	node->children = NULL;
	return node;
}

void append_child(Node* parent, Node* child)
{
	int l = parent->len_child;
	parent->children = (Node**)realloc(parent->children, sizeof(Node*)*(l+1));
	parent->children[l] = child;
	parent->len_child += 1;
}

void append_child_move(Node* parent, xy coord)
{
	int l = parent->chmov;
	parent->children_move = (xy*)realloc(parent->children_move, sizeof(xy)*(l+1));
	parent->children_move[l] = coord;
	parent->chmov += 1;
}

void addChild(Node* node, char** child_state, xy move)
{
	Node* child = create_node(node,child_state);
	append_child(node,child);
	append_child_move(node,move);
}

void update(Node* node, float reward)
{
	node->reward += reward;
	node->visits += 1;
}

int fully_explored(Node* node)
{
	int nlm = 0;
	struct pos* lm = get_neigh(SIZE,node->state,'B');
	if(node->len_child == lm->size || trcount(lm,node->state)== node->len_child )
		return 1;
	return -1;
}

char** MCTS(int mx_itr, char** root_state, float factor)
{
	char** rsc = copy_state(root_state);
	Node* root = create_node(NULL,rsc);
	Node* front;
	int turn,winner;
	for(int i = 0; i < mx_itr; i++)
	{
		//printf("iteration %d\n",i+1);
		FT *ft = treePolicy(root,1,factor);
		front = ft->node;
		turn = ft->turn;
		//printf("%c\n",front->state[7][7]);
		winner = defaultPolicy(front->state,turn);
		backup(front,winner,turn);
	}
	Node* ans = bestChild(root,0);
	return ans->state;
}


FT* treePolicy(Node* node, int turn, float factor)
{
	//printf("tree policy\n");
	FT* ret = (FT*)malloc(sizeof(FT));
	struct pos* a=get_neigh(SIZE,node->state,'W');
	while(gameover(node->state,SIZE,turn) == 0 && trcount(a,node->state)>0  )
	{
		//printf("%cabc\n",node->state[7][7]);
		if(fully_explored(node) == -1)
		{
			ret->node = expand(node,turn);
			ret->turn = -turn;
			//printf("exit tree policy\n");
			//printf("%c\n",ret->node->state[7][7]);			
			return ret;
		}
		else
		{
			node = bestChild(node,factor);
			turn *= -1;
		}
	}
	//printf("exit tree policy\n");
	ret->node = node;
	ret->turn = turn;
	//printf("%c\n",ret->node->state[7][7]);
	return ret;
}

char** copy_state(char** input)
{
   
    char** b1=(char**)malloc(sizeof(char*)*SIZE);
    for(int i=0;i<SIZE;i++)
        b1[i]=(char*)malloc(sizeof(char)*SIZE);
    for(int i=0;i<SIZE;i++)
       		for(int j=0;j<SIZE;j++)
       			b1[i][j]=input[i][j];
    return b1;
}

Node* expand(Node* node, int turn)
{
	//printf("expansion begins\n");
	
	struct pos *legal = get_neigh(SIZE,node->state,'W');
	int *untried = (int*)malloc(sizeof(int)*legal->size);
	for(int i = 0; i<legal->size ; i++)
		untried[i] = 1;
	for(int i = 0; i<legal->size ; i++)
		for(int j = 0; j<node->chmov ; j++)
		{
			if(legal->x[i] == node->children_move[j].x && legal->y[i] == node->children_move[j].y)
				untried[i] = 0;
		}
	/*for(int i = 0;i<node->chmov;i++)
		legal[node->children_move[i]] = 0;
	grid* new_state;*/
	char** new_state;
	new_state = copy_state(node->state);
	xy move;
	for(int i = 0; i<legal->size ; i++)
	{
			if(untried[i] == 0)
				continue;
			int x_move, y_move;
			
			x_move  = legal->x[i];
			y_move = legal->y[i];
			move.x = x_move;
			move.y = y_move;
			new_state[x_move][y_move] = 'B';
			// printf("%d %d\n",x_move,y_move);
			flip(SIZE,new_state,'B',x_move+1,y_move+1);
			//print_board(SIZE, new_state);
			//printf("merihrgwkhghhhhhhhhhhhhhhhh\n");
			break;
		
	}
	//printf("%c\n",node->children[node->len_child-1]->state[1][1]);
	//printf("%c\n",new_state[2][2]);	
	addChild(node,new_state,move);
	//printf("expasion ends\n");
	return node->children[node->len_child-1];
}



Node* bestChild(Node* node, float factor)
{
	//printf("best child\n");
	float bestscore = -10000000.0;
	float exploit,explore,score;
	Node** bestChildren = (Node**)malloc(sizeof(Node*)*node->len_child);
	int l = 0;
	if(factor < 2)
		{
			//printf("move   score\n");
		}
	for(int i = 0; i<node->len_child; i++)
	{
		Node* c = node->children[i];
		exploit = c->reward/c->visits;  // Heuristic
		explore = sqrt(log(2.0*node->visits)/(float)(c->visits)); // UCB
		score = exploit + factor*explore;  
		if(factor < 2)
		{
			//printf("%d     %f \n",i+1,score);
		}
		if(score == bestscore)
		{
			bestChildren[l] = c;
			l += 1;
		}
		else if(score > bestscore)
		{
			bestChildren[l] = c;
			l += 1;
			bestscore = score;
		}
	}
	if(l == 0)
	{
		return node;
		printf("div by zero\n");
	}
	if(factor<2)
		return bestChildren[l-1];
	return bestChildren[rand()%l];
}
void printc(struct pos* a,char** state)
{
	for(int i=0;i<a->size;i++)
	printf("%d %d %c\n",a->x[i],a->y[i],state[a->x[i]][a->y[i]]);
	printf("\n");
}

char** next_state(char** state,int turn)
{
	struct pos* legal = get_neigh(SIZE,state,'B');
	struct pos* legal1 = get_neigh(SIZE,state,'W');	
	int r;
	//printf("%d %d aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n",legal->size,legal1->size);
	//printf("IN NEXT STATE\n");	
	//print_board(SIZE,state);
	//printf("%d %d\n",legal->size,legal1->size);
	char *s=malloc(sizeof(char));

	if(legal->size==0)
		return state;
	if(legal1->size==0)
		return state;	
	//r = rand()%legal->size;
	char** aux = copy_state(state);
	if(turn == 1)
	{
		//printf("mistaje venvevkmkevn\n");
		legal=get_neigh(SIZE,state,'W');
		if(legal->size==0)
		{
			state[0][0]='I';//Invalid
			return state;
		}
		r = rand()%legal->size;
		aux[legal->x[r]][legal->y[r]] = 'B';
		flip(SIZE,aux,'B',legal->x[r]+1,legal->y[r]+1);
		//print_board(SIZE, aux);
		//printf("%d\n",r);
	}
	else
	{
		legal=get_neigh(SIZE,state,'B');
		if(legal->size==0)
		{
			state[0][0]='I';//Invalid
			return state;
		}
		r = rand()%legal->size;
		aux[legal->x[r]][legal->y[r]] = 'W';
		flip(SIZE,aux,'W',legal->x[r]+1,legal->x[r]+1);
		//print_board(SIZE, aux);
		//printf("%d\n",r);
	
	}
	return aux;
}
int defaultPolicy(char** state,int turn)
{
	//printf("enter default policy\n");
	//printf("%c\n",state[7][7]);	
	//print_board(SIZE,state);	
	struct pos* a=get_neigh(SIZE,state,'B');
	//printf("----------1");
	struct pos* b1=get_neigh(SIZE,state,'W');
	char** state1;
	while(gameover(state,SIZE,turn) == 0 && trcount(a,state)>0 )
	{
		//printf("%d \n",a->size);				
		//printc(a,state);		
		//printf("IN DEFAULT POLICY\n");	
		//print_board(SIZE,state);
		//printc(a,state);	
		//printf("%d %d\n",a->size,b1->size);		
		state1 = next_state(state,turn);
		if(state1[0][0]=='I')
			break;
		else
		{
			state=state1;
		}
		
		turn *= -1;
		//printf("how many lol\n");
		//print_board(SIZE, state);
	}
	//printf("exit default policy\n");
	int w,b;
	count_pawns(SIZE,state,&w,&b);
	if(w == b)
	{	
		//printf("draw\n");
		return 0;
	}
	if(w>b)
		return -1;
	return 1;
}

void backup(Node* node, int reward, int turn)
{
	//printf("backing up\n");
	while(node != NULL)
	{
		node->visits += 1;
		node->reward -= turn*reward;
		node = node->parent;
		turn *= -1;
	}
}
