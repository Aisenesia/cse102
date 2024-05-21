#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 10

typedef struct{
    int row;
    int col;
}point;

typedef struct {
    point* body;
    int length;
}snake;

typedef struct{
    char type;
    int val;
}block;


block*** init_board();
void draw_board(block*** board, snake ouroboros);
void play(block*** board);
void move(snake* ouroboros, point* prevHead, int* moveCount);
int check_status(block*** board, snake ouroboros);
void update(block*** board, snake* ouroboros, int moveCount, point* prevHead);

int main(){
    srand(time(NULL));
    block*** board = init_board();
    //draw_board(board,ouroboros);
    play(board);
    for(int i=0;i<SIZE;i++){
        for(int j=0;j<SIZE;j++){
            free(board[i][j]);
        }
        free(board[i]);
    }
    free(board);
}

block*** init_board() {
    block*** board = malloc(SIZE * sizeof(block**));
    for (int i = 0; i < SIZE; i++) {
        board[i] = malloc(SIZE * sizeof(block*));
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = calloc(1, sizeof(block));
            board[i][j][0].type = 'e';
            board[i][j][0].val = 0;
        }
    }
    int baitRow, baitCol, obsRow, obsCol;
    //generating bait
    do {
        baitRow = rand() % SIZE;
        baitCol = rand() % SIZE;
    } while ((baitRow == 0 && baitCol == 0));
    board[baitRow][baitCol][0].type = 'b';
    //generating obstacle
    do {
        obsRow = rand() % SIZE;
        obsCol = rand() % SIZE;
    } while ((obsRow == 0 && obsCol == 0) || (obsRow==baitRow && obsCol==baitCol));
    board[obsRow][obsCol][0].type = 'o';
    board[obsRow][obsCol][0].val = 1;

    return board;
}

void draw_board(block*** board, snake ouroboros){
    system("clear");
    //printf("  0 1 2 3 4 5 6 7 8 9 \n");
    printf("-----------------------\n");
    int flag=0;
    for(int i=0;i<SIZE;i++){
        //printf("%d ",i); 
        printf("| "); 
        for(int j=0;j<SIZE;j++){
            for(int k=0;k<ouroboros.length;k++){
                if(i==ouroboros.body[k].row && j==ouroboros.body[k].col){
                    if(k==0){
                        printf("O ");
                    }
                    else{
                        printf("X ");
                    }
                    flag=1;
                }
            }
            if(flag==0){
                switch (board[i][j][0].type){
                case 'e':
                    printf("  ");
                    flag=1;
                    break;
                case 'b':
                    printf("· ");  
                    break;
                case 'o':
                    printf("%d ", board[i][j][0].val);
                    break;
                }
            }
            flag=0;
            
        }printf("|\n");
        

    }printf("-----------------------\n");
}

void play(block*** board){
    snake ouroboros;
    point prev;
    //ouroboros.length=2;
    //ouroboros.body=(point*)malloc(2*sizeof(point));
    char mov;
    int count = 0;
    ouroboros.length=1;
    ouroboros.body=(point*)malloc((ouroboros.length) * sizeof(point));
    //printf("%ld",sizeof(ouroboros.body));
    ouroboros.body[0].row=0;
    ouroboros.body[0].col=0;
    //ask for movement, check validity, move, check status
    while(1){
        draw_board(board, ouroboros);
        move(&ouroboros, &prev, &count);
        if(check_status(board, ouroboros)){
            break;
        }
        update(board, &ouroboros, count, &prev);
        count++;
    }
    free(ouroboros.body);
}

int check_status(block*** board, snake ouroboros){
    //3 things to check: 1 - snake bit itself, 2 - did snake hit the wall, 3 - did snake hit an obstacle that is bigger than snakes lenght?
    int row = ouroboros.body[0].row;
    int col = ouroboros.body[0].col;
    for(int k=1;k<ouroboros.length-1;k++){
        if(row==ouroboros.body[k].row && col==ouroboros.body[k].col){
            //printf("gate1\n");
            return 1;
        }
    }
    if(row==SIZE || col==SIZE || row==-1 || col==-1){
        //printf("gate2\n");
        return 1;
    }
    if(board[row][col][0].type=='o'){
        
        if(ouroboros.length<board[row][col][0].val){
            //printf("gate3\n");
            return 1;
        }
    }
    return 0;
}

void move(snake* ouroboros, point* prevHead, int* moveCount){
    char mov;
    int row, col, i;
    /* for inspecting bodies orientation we should compare body[0] and body[1] */
    //printf("len %d",ouroboros->length);
    int flag;
    do{
        flag=0;
        scanf(" %c",&mov);
        switch (mov)
        {
        case 'W':// X   [0,0] head_row - fol_row==  1
        case 'w':// O   [1,0]
            if(ouroboros->length>1 && (ouroboros->body[0].row - ouroboros->body[1].row)== 1){
                *moveCount--;
                return;//ignored
            }
            row = ouroboros->body[0].row-1;
            col = ouroboros->body[0].col;
            break;
        case 'A'://X O [0,1] [0,0] head_col - fol_col== 1
        case 'a':
            if(ouroboros->length>1 && (ouroboros->body[0].col - ouroboros->body[1].col)== 1){
                *moveCount--;
                return;
            }
            row = ouroboros->body[0].row;
            col = ouroboros->body[0].col-1;
            break;
        case 'S':// O   [0,0] head_row - fol_row== -1
        case 's':// X   [1,0]
            if(ouroboros->length>1 && (ouroboros->body[0].row - ouroboros->body[1].row)== -1){
                *moveCount--;
                return;
            }
            row = ouroboros->body[0].row+1;
            col = ouroboros->body[0].col;
            
            break;
        case 'D'://O X [0,0] [0,1] head_col - fol_col== -1
        case 'd':
            if(ouroboros->length>1 && (ouroboros->body[0].col - ouroboros->body[1].col)== -1){
                *moveCount--;
                return;
            }
            row = ouroboros->body[0].row;
            col = ouroboros->body[0].col+1;
            break;
        default:
            printf("Invalid move!\n");  
            flag=1;
            break;
        }
        while(getchar()!='\n');
    }while(flag==1);
    prevHead->row = ouroboros->body[0].row;
    prevHead->col = ouroboros->body[0].col;
    ouroboros->body[0].row=row;
    ouroboros->body[0].col=col;
}

void update(block*** board, snake* ouroboros, int moveCount, point* prevHead){
    int head_row = ouroboros->body[0].row;
    int head_col = ouroboros->body[0].col;
    int baitRow, baitCol, flag = 0, obsRow, obsCol, index = 0;
    int obsCount = 0;
    point temp;
    temp.row= ouroboros->body[ouroboros->length-1].row;
    temp.col= ouroboros->body[ouroboros->length-1].col;
    for(int i=ouroboros->length-1;i>1;i--){
        ouroboros->body[i].row=ouroboros->body[i-1].row;
        ouroboros->body[i].col=ouroboros->body[i-1].col;
    }
    if(ouroboros->length>1){
        ouroboros->body[1].row =prevHead->row;
        ouroboros->body[1].col =prevHead->col;
    }
    point* obs = malloc(3 * sizeof(point));
    for(int i=0;i<SIZE;i++){
        for(int j=0; j<SIZE ;j++){
            if(board[i][j][0].type=='o'){
                obs[obsCount].row=i;
                obs[obsCount].col=j;
                obsCount++;//obstacle count at 0th index
            }
        }
    }
    if(board[head_row][head_col][0].type=='b'){
        ouroboros->length++;
        board[head_row][head_col][0].type='e';
        ouroboros->body = realloc(ouroboros->body, (ouroboros->length) * sizeof(point));
        if(ouroboros->length>2){
            ouroboros->body[ouroboros->length-1].row = temp.row;
            ouroboros->body[ouroboros->length-1].col = temp.col; 
        }
        else{
            ouroboros->body[ouroboros->length-1].row = prevHead->row;
            ouroboros->body[ouroboros->length-1].col = prevHead->col; 
        }
        //create new bait
        do{
            flag=0;
            baitRow=rand()%SIZE;
            baitCol=rand()%SIZE;
            for(int i=0; i<ouroboros->length; i++){
                if(ouroboros->body[i].row==baitRow && ouroboros->body[i].col==baitCol){
                    flag=1;
                }
            }
        }while(flag==1);
        board[baitRow][baitCol]->type='b';
    }
    if(board[head_row][head_col][0].type=='o'){
        //we already checked for breakability in chech_status, now we destroy blocks
            board[head_row][head_col] = realloc(board[head_row][head_col], 1*sizeof(point));
            board[head_row][head_col][0].type = 'e';
            board[head_row][head_col][0].val = 0;
        obsCount--;
    }
    if(moveCount%5==0 && moveCount!=0){//we shall spawn another obstacle now
        if(obsCount<3){
            do{
                flag=0;
                obsRow = rand()%SIZE;
                obsCol = rand()%SIZE;
                for(int i=0; i<ouroboros->length; i++){
                    if(ouroboros->body[i].row==obsRow && ouroboros->body[i].col==obsCol){
                        flag=1;
                    }
                }
            }while(flag==1 || board[obsRow][obsCol][0].type == 'b');
            index = board[obsRow][obsCol][0].val;//obstacle count at that line
            if(index!=0){
                board[obsRow][obsCol] = realloc(board[obsRow][obsCol], (index+1) * sizeof(block));
                board[obsRow][obsCol][index].type = 'o';
                board[obsRow][obsCol][index].val = 1;
            }
            board[obsRow][obsCol][0].val++;
            board[obsRow][obsCol][0].type='o';        
        }
        else{//3 obstacles are there, we have to spawn an obstacle on top of the others
            int selection= rand()%3;//selecting one of the 3 obstacles to put upon randomly
            index = board[obs[selection].row][obs[selection].col][0].val;
            board[obs[selection].row][obs[selection].col][0].val++;
            board[obs[selection].row][obs[selection].col] = realloc(board[obs[selection].row][obs[selection].col], (index+1) * sizeof(block));
            board[obs[selection].row][obs[selection].col][index].type = 'o';
            board[obs[selection].row][obs[selection].col][index].val = 1;

        }
    }
    free(obs);
}


/*
 O
   X X X {0 1, 1 2} -1 -1

 O X X X {1 1, 1 2} 

 O   X X X {1 0, 1 2} 0 -2

   X X X {2 1, 1 2}  1 -1
 O

O
  X X
    X X 
  O
  X X
    X X 

O X X
    X X 

*/