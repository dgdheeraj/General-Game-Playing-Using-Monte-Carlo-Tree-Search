#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "header.h"
#define SIZE 8


char** initialize_board(int board_size)
{
    // Initialising a 2-D Array representing the board
    char** board=(char**)malloc(sizeof(char*)*board_size);
    for(int i=0;i<board_size;i++)
        board[i]=(char*)malloc(sizeof(char)*board_size);
    
    //Initializing values to the board U -> Empty
    for(int i=0;i<board_size;i++)
    {
        for(int j=0;j<board_size;j++)
            board[i][j]='-';
    }
    int mid=(board_size-1)/2;
    board[mid][mid]='W';
    board[mid+1][mid]='B';
    board[mid][mid+1]='B';
    board[mid+1][mid+1]='W';
    return board;
}

void print_board(int board_size, char** board)
{
    printf("   ");
    for(int i=0;i<board_size;i++)
        printf("%d  ",i+1);
    printf("\n");
    for(int i=0;i<board_size;i++)
    {
        printf("%d  ",i+1);
        for(int j=0;j<board_size;j++)
            printf("%c  ",board[i][j]);
        printf("\n");
    }

}

int is_empty(int board_size,char** board)
{
    for(int i=0;i<board_size;i++)
    {
        for(int j=0;j<board_size;j++)
        {
            if(board[i][j]=='-')
                return 1;
        }
    }
    return 0;
}

int cord_in_list(int x,int y,struct pos* ptr)
{
    for(int i=0;i<ptr->size;i++)
    {
        if(ptr->x[i]==x && ptr->y[i]==y)
            return 1;
    }
    return 0;
}
void make_move(int board_size,char** board,int turn,char choice[2],int* i,int* j,struct pos* ptr)
{
    
//    int i,j;
    while(1)
    {
        printf("Please enter you move! :");
        scanf("%d",&(*i));
        scanf("%d",&(*j));
        if(*i>0 && *i<=board_size && *j>0 && *j<=board_size)
        {
            if(board[*i-1][*j-1]=='-' && cord_in_list((*i-1),(*j-1),ptr)==1 )
            break;
        }
        else
            printf("Invalid Input. Please Enter Again.\n");        
    }
    board[*i-1][*j-1]=choice[turn];

}

struct pos* get_empty_neigh(int board_size,char** board, int x,int y)
{
    struct pos* d=(struct pos*)malloc(sizeof(struct pos)*board_size*board_size);
    d->size=0;
    d->x=(int*)malloc(sizeof(int)*board_size*board_size);
    d->y=(int*)malloc(sizeof(int)*board_size*board_size);

        // printf("Neighbours of %d %d\n",x,y);
        if( x+1 <= board_size-1 && board[x+1][y]=='-' )
        {
            // printf("%d %d\n",x+1,y);
            d->x[d->size]=x+1;
            d->y[d->size]=y;
            d->size+=1;
        }
        if( x+1 <= board_size-1 && y+1<=board_size-1 && board[x+1][y+1]=='-')
        {
            // printf("%d %d\n",x+1,y+1);
            d->x[d->size]=x+1;
            d->y[d->size]=y+1;
            d->size+=1;
        }
        if( y+1 <= board_size-1 && board[x][y+1]=='-')
        {
            // printf("%d %d\n",x,y+1);
            d->x[d->size]=x;
            d->y[d->size]=y+1;
            d->size+=1;
        }
        if( x-1 >=0 && y+1<=board_size-1 && board[x-1][y+1]=='-')
        {
            // printf("%d %d\n",x-1,y+1);
            d->x[d->size]=x-1;
            d->y[d->size]=y+1;
            d->size+=1;
        }
        if( x-1 >=0 && board[x-1][y]=='-')
        {
            // printf("%d %d\n",x-1,y);
            d->x[d->size]=x-1;
            d->y[d->size]=y;
            d->size+=1;
        }
        if( x-1 >=0 && y-1 >=0 && board[x-1][y-1]=='-')
        {
            // printf("%d %d\n",x-1,y-1);
            d->x[d->size]=x-1;
            d->y[d->size]=y-1;
            d->size+=1;
        }
        if( y-1 >= 0 && board[x][y-1]=='-')
        {
            // printf("%d %d\n",x,y-1);
            d->x[d->size]=x;
            d->y[d->size]=y-1;
            d->size+=1;
        }
        if( x+1 <= board_size-1 && y-1>=0 && board[x+1][y-1]=='-')
        {
            // printf("%d %d\n",x+1,y-1);
            d->x[d->size]=x+1;
            d->y[d->size]=y-1;
            d->size+=1;
        }
    return d;
    //}
} 
int check_inside(struct pos* c,int x, int y)
{
	for(int i=0;i<c->size;i++)
	{
		if(c->x[i]==x && c->y[i]==y)
			return 1;
	}
	return 0;
}

struct pos* get_neigh(int board_size,char** board,char ch)//Obtains an array containing array of all one color pawns
{
    struct pos* d=(struct pos*)malloc(sizeof(struct pos)*board_size*board_size);
    d->size=0;
    d->x=(int*)malloc(sizeof(int)*board_size*board_size);
    d->y=(int*)malloc(sizeof(int)*board_size*board_size);
    
    struct pos* emp;

    struct pos* possible_moves=(struct pos*)malloc(sizeof(struct pos)*board_size*board_size);
    possible_moves->size=0;
    possible_moves->x=(int*)malloc(sizeof(int)*board_size*board_size);
    possible_moves->y=(int*)malloc(sizeof(int)*board_size*board_size);

    for(int i=0;i<board_size;i++)
    {
        for(int j=0;j<board_size;j++)
        {
            if(board[i][j]==ch)
            {
                d->x[d->size]=i;
                d->y[d->size]=j;
                d->size=d->size+1;
            }
        }
    }
    int diffx=0;int diffy=0;
    int x,y;
    char ch1;//To find this character in any row
    if(ch=='B')
        ch1='W';
    else
        ch1='B';   
    //for each pawn
    for(int i=0;i<d->size;i++)
    {
        emp=get_empty_neigh(board_size,board,d->x[i],d->y[i]);
        for(int j=0;j<emp->size;j++)//For each empty neighbour of a particular pawn
        {
            // printf("QGRe\n");
            diffx=d->x[i] - emp->x[j];
            diffy=d->y[i] - emp->y[j];
            x=d->x[i];
            y=d->y[i];
            while(x>=0 && x<=board_size-1 && y>=0 && y<=board_size-1)
            {
                if(board[x][y]==ch1 && check_inside(possible_moves,emp->x[j],emp->y[j])==0 )
                {
                    possible_moves->x[possible_moves->size]=emp->x[j];                    
                    possible_moves->y[possible_moves->size]=emp->y[j];                    
                    possible_moves->size+=1;
			//printf("%d %din get niegh\n",emp->x[j],emp->y[j]);                   
			break;
                }
                x=x+diffx;
                y=y+diffy;
                // printf("%d %d\n",x,y);
            }
        }
    }
    return possible_moves;
}
void flip(int board_size,char** board,char ch,int x, int y)
{
    char ch1;
    if(ch=='B')
    {
        ch1='B';    
        ch='W';
    }
    else if(ch=='W')
    {
        ch1='W';
        ch='B';
    }
    //Chaning 1 indexed to 0 indexing
    x=x-1;
    y=y-1;
    int flag=0;
    int x1=x;
    int y1=y;
    //X axis
    x1+=1;
    flag=0;
    while(x1<=board_size-1)
    {
        if(board[x1][y1]==ch1)
        {
            flag=1;
            break;
        }
        x1+=1;
    }
        //printf("%d %d \n",x,y);

    if(flag==1)
    {
        x1=x;y1=y;
        x1+=1;
        while(x1<=board_size-1)
        {
            if(board[x1][y1]==ch1)
                break;
            if(board[x1][y1]==ch)
            {
                board[x1][y1]=ch1;
                //printf("%c\n",board[x1][y1]);
            }
            x1+=1;
        }
    }
    //printf("in flip\n");

    x1=x;y1=y; flag=0;
    //X=y
    x1+=1;
    y1+=1;
    while(x1<=board_size-1 && y1<=board_size-1)
    {
        if(board[x1][y1]==ch1)
        {
            flag=1;
            break;
        }
        x1+=1;
        y1+=1;
    }    
    if(flag==1)
    {
        x1=x;y1=y;
        x1+=1;y1+=1;
        while(x1<=board_size-1 && y1<=board_size-1)
        {
            if(board[x1][y1]==ch1)
            {
                break;
            }    
            if(board[x1][y1]==ch)
                board[x1][y1]=ch1;
            x1+=1;
            y1+=1;
        }
    }

    x1=x;y1=y;flag=0;
    //y axis
    y1+=1;
    while(y1<=board_size-1)
    {
        if(board[x1][y1]==ch1)
        {
            flag=1;
            break;
        }
        y1+=1;
    }
    if(flag==1)
    {
        x1=x;y1=y;
        y1+=1;
        while(y1<=board_size-1)
        {
            if(board[x1][y1]==ch1)
                break;
            if(board[x1][y1]==ch)
                board[x1][y1]=ch1;
            y1+=1;
        }
    }

    x1=x;y1=y;flag=0;
    //-x and +y
    x1-=1;
    y1+=1;
    while(x1>=0 && y1<=board_size-1)
    {
        if(board[x1][y1]==ch1)
        {
            flag=1;
            break;
        }
        x1-=1;
        y1+=1;
    }
    if(flag==1)
    {
        x1=x;y1=y;
        x1-=1;y1+=1;
        while(x1>=0 && y1<=board_size-1)
        {
            // printf("%d %d\n",)
            if(board[x1][y1]==ch1)
                break;
            if(board[x1][y1]==ch)
                board[x1][y1]=ch1;
            y1+=1;
            x1-=1;
        }
    }
    x1=x;y1=y;flag=0;
    //-x axis
    x1-=1;
    while(x1>=0)
    {
        if(board[x1][y1]==ch1)
        {
            flag=1;
            break;
        }
        x1-=1;
    }
    if(flag==1)
    {
        x1=x;y1=y;
        x1-=1;
        while(x1>=0)
        {
            if(board[x1][y1]==ch1)
                break;
            if(board[x1][y1]==ch)
            {
                board[x1][y1]=ch1;
                // printf("%c move made %c to check\n",ch1,ch);
            }
            x1-=1;
        }
    }

    x1=x;y1=y;flag=0;
    //-x -y
    x1-=1;
    y1-=1;
    while(x1>=0 && y1>=0)
    {
        if(board[x1][y1]==ch1)
        {
            flag=1;
            break;
        }
        x1-=1;
        y1-=1;
    }
    if(flag==1)
    {
        x1=x;y1=y;
        x1-=1;
        y1-=1;
        while(x1>=0 && y1>=0)
        {
            if(board[x1][y1]==ch1)
                break;
            if(board[x1][y1]==ch)
                board[x1][y1]=ch1;
            x1-=1;
            y1-=1;
        }
    }

    x1=x;y1=y;flag=0;
    //-y
    y1-=1;
    while(y1>=0)
    {
        if(board[x1][y1]==ch1)
        {
            flag=1;
            break;
        }
        y1-=1;
    }
    if(flag==1)
    {
        x1=x;y1=y;
        y1-=1;
        while(y1>=0)
        {
            //printf("%c %c %d %d\n",board[x1][y1],ch,x1,y1);
            if(board[x1][y1]==ch1)
                break;
            if(board[x1][y1]==ch)
            {
                board[x1][y1]=ch1;
                //printf("Entered %c %c\n",ch,ch1);
            }
            y1-=1;
        }
    }

    x1=x;y1=y;flag=0;
    //+x  -y
    x1+=1;
    y1-=1;
    while(x1<=board_size-1 && y>=0)
    {
        if(board[x1][y1]==ch1)
        {
            flag=1;
            break;
        }
        x1+=1;
        y1-=1;
    }
    if(flag==1)
    {
        x1=x;y1=y;
        x1+=1;
        y1-=1;
        while(x1<=board_size-1 && y>=0)
        {

            if(board[x1][y1]==ch1)
                break;
            if(board[x1][y1]==ch)
                board[x1][y1]=ch1;
            x1+=1;
            y1-=1;
        }
    }
}

void count_pawns(int board_size,char ** board,int* w,int* b)
{
    for(int i=0;i<board_size;i++)
    {
        for(int j=0;j<board_size;j++)
        {
            if(board[i][j]=='W')
                *w+=1;
            else if(board[i][j]=='B')
                *b+=1;
        }
    }
}

void play_game(int board_size,char** board,char choice[2])//choice refers to the color Player 1 chose
{
    print_board(board_size,board);
    int turn = 1;// 1 means user, 0 is comp
    int flag=0;//1 means game over 
    int i,j;
    int countW=0;
    int countB=0;
    struct pos* ptr;
    struct pos* p1;
    struct pos* p2;
    while(is_empty(board_size,board)!=0)
    {
    	p1=get_neigh(SIZE,board,'W');
        p2=get_neigh(SIZE,board,'B');
        if(turn == 0)
    	{
    		if(p1->size==0)
            {
                    count_pawns(board_size,board,&countW,&countB);
                    if(countW > countB )
                        printf("%c wins!\n",'W');
                    else if(countB > countW)
                        printf("%c wins!\n",'B');
                    else
                        printf("Draw!!!\n");
                    flag=1;
                    break;
            }
            printf("\n\nAI made it's Move\n\n");
            // if(p1->)
            board = MCTS(1,board,2.0);
    		print_board(board_size,board);

        	turn+=1;
        	turn=turn%2;
    		continue;
    	}
        if(choice[turn]=='W')
        {
            ptr=get_neigh(board_size,board,'B');
        }
        else
        {
            ptr=get_neigh(board_size,board,'W');
        }
        // system("clear");
        printf("List of all Allowed moves for %c\n",choice[turn]);
        for(int a=0;a<ptr->size;a++)
        {
            //Possible Moves
            printf("%d %d\n",ptr->x[a]+1,ptr->y[a]+1);
        }        
        //Game Over if no possible moves are left for any player
        if(ptr->size==0)
            break;
        printf("Player %d Turn! %c\n",turn,choice[turn]);
        
        make_move(board_size,board,turn,choice,&i,&j,ptr);
        
        flip(board_size,board,choice[turn],i,j);//ch->bit inserted
        
        print_board(board_size,board);

        turn+=1;
        turn=turn%2 ;
        // if(choice[turn]=='B')
        // else        
            // flip(board_size,board,'B',i,j);//ch->bit inserted
    }
    printf("Game Over!!\n");
    if(flag!=1)
    {
        count_pawns(board_size,board,&countW,&countB);
        if(countW > countB )
            printf("%c wins!\n",'W');
        else if(countB > countW)
            printf("%d wins!\n",'B');
        else
            printf("Draw!!!\n");
    }
}


// num = (rand() % (upper – lower + 1)) + lower for generating random number
int main()
{
    int board_size,turn=1;
    char choice[2];

    // printf("Player 1 Black or White? : ");
    // scanf("%c",&(choice[0]));
    // fflush(stdin);
    while(1)
    {
        
        if(choice[0]=='W' || choice[0]=='B')
            break;
        //printf("\nPlayer 1 Black or White? : ");
        //scanf("%c",&(choice[0]));     
        choice[0] = 'B';           
        // fflush(stdin);
    }
    if(choice[0]=='W')
        choice[1]='B';
    else
        choice[1]='W';
    

    //printf("Enter the dimension of the board: ");
    //scanf("%d",&board_size);
    
    board_size = SIZE;
        
    while(board_size%2!=0 || board_size < 1 || board_size >8)
    {
        printf("Please enter valid input. Board can be only of even dimensions upto 8\n");
        printf("Enter the dimension of the board: ");
        scanf("%d",&board_size);
    }

    char** board=initialize_board(board_size);
    play_game(board_size,board,choice);
}
