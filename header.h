#define SIZE 8

struct pos
{
    int* x;
    int* y;
    int size;
};

typedef struct xy
{
	int x;
	int y;
}xy;


struct pos* get_neigh(int board_size,char** board,char ch);//Obtains an array containing array of all one color pawns
int is_empty(int board_size,char** board);
void count_pawns(int board_size,char ** board,int* w,int* b);


typedef struct node{
	int visits;
	float reward;
	char** state;
	struct node** children;
	int len_child;
	xy* children_move;
	int chmov;
	struct node* parent;
}Node;

typedef struct front_turn{
	Node* node;
	int turn;
}FT;
//void print_grid(char** g);
void print_board(int board_size, char** board);
void flip(int board_size,char** board,char ch,int x, int y);
int* legal_moves(char** state);
Node* create_node(Node* parent, char** state);
void append_child(Node* parent, Node* child);
void append_child_move(Node* parent, xy);
void addChild(Node* node, char** child_state, xy move);
void update(Node* node,float reward);
int fully_explored(Node* node);
char** MCTS(int mx_itr, char** root, float factor);
FT* treePolicy(Node* node, int turn, float factor);
char** copy_state(char** org);
Node* expand(Node* node, int turn);
Node* bestChild(Node* node, float factor);
int defaultPolicy(char** state,int turn);
void backup(Node* node, int reward, int turn);
int check_fill(char** g);
int check_winner(char** g, int cst);

