#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct{
    int board[3][3];
    int empty_row;
    int empty_col;
}board;

// Function prototypes
int init_board(board *g_board);
void print_board(board *g_board);
int is_valid_move(board *g_board, int num, char dir);
void update_board(board *g_board, int num, char dir);
int state_chk(board *g_board);
void save(board *g_board);
void auto_finish(board *g_board, char lastMove, unsigned int counter);
char returnMove(board *g_board, char lastMove);
/*
    Would advise to increase stack size defult maximum stack size should last for ~174k moves which is not enough
*/
int main(){
    board g_board;
    int choice, num_moves = 0;
    int num;
    char dir;
    int ret;
    FILE *pfile;
    FILE *score;
    pfile=fopen("puzzle.txt","w");//re-creating at execution so it doesnt mixed up with previous execs.
    fclose(pfile);

    srand(time(NULL));

    do{
        printf("1. Play game as a user\n2. Finish the game with PC\n3. Show the best score\n4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        do{
            ret=init_board(&g_board);
        }while(ret==-1);
        switch(choice){
            case 1:
                save(&g_board);
                num_moves = 0;
                while(!state_chk(&g_board)){
                    print_board(&g_board);
                    printf("Enter number and direction (e.g., 2-D): ");
                    scanf("%d-%c", &num, &dir);
                    if(is_valid_move(&g_board, num, dir)){
                        update_board(&g_board, num, dir);
                        save(&g_board);
                        num_moves++;
                    } else {
                        printf("Invalid move. Try again.\n");
                    }
                }
                print_board(&g_board);
                printf("Congratulations! You completed the puzzle in %d moves.\n", num_moves);
                printf("Your score: %d\n", 1000 - 10*num_moves);
                score=fopen("scores.txt","a");
                fprintf(score,"%d\n",1000 - 10*num_moves);
                fclose(score);
                break;
            case 2:
                auto_finish(&g_board, -1, 1);
                //printf("PC has solved the puzzle\n");
                break;
            case 3:
                score=fopen("scores.txt","r");
                if(!score){
                    printf("Nothing to show!\n");
                    break;
                }
                int best;
                int tmp;
                fscanf(score,"%d",&best);
                while(fscanf(score,"%d\n",&tmp)!=EOF){
                    if(tmp>best){
                        best=tmp;
                    }
                }
                printf("Best score is: %d\n", best);
                fclose(score);
                break;
            case 4:
                return 0;
            default:
                printf("Invalid input.\n");
                while(getchar()!='\n');
                break;
        }
    }while(choice!=4);
    return 0;
}

int init_board(board *g_board){
    int i, j, temp;
    int nums[9] = {1, 2, 3, 4, 5, 6, 7, 8, 0};
    for(i=8; i>0; i--){//Fisher–Yates shuffle
        j = rand() % (i+1);
        temp = nums[i];
        nums[i] = nums[j];
        nums[j] = temp;
    }
    
    int idx = 0;////we go through the shuffled array with this to find index of 0(space)
    for(i = 0; i < 3; i++){
        for(j = 0; j < 3; j++){
            g_board->board[i][j] = nums[idx];
            if(nums[idx] == 0){
                g_board->empty_row = i;
                g_board->empty_col = j;
            }
            idx++;
        }
    }
    /*
    g_board->board[0][0]=1;
    g_board->board[0][1]=2;
    g_board->board[0][2]=3;
    g_board->board[1][0]=4;
    g_board->board[1][1]=5;
    g_board->board[1][2]=6;
    g_board->board[2][0]=0;
    g_board->board[2][1]=7;
    g_board->board[2][2]=8;
    g_board->empty_row = 2;
    g_board->empty_col = 0;
    */
   int inv_count = 0;//checks solveability if not solveable, return -1
   int *arr = (int*)g_board->board;//linearizing array, easier this way
   for(i=0; i<8; i++){
        for(j=i+1; j<9; j++){
            if(arr[j] && arr[i] && arr[i]>arr[j]){
                inv_count++;
            }
        }
    }
    if(inv_count%2==1){
        //printf("CONTROL_\n");
        return -1;//we need to recreate the board
    }
    return 0;//no problems, continue
}
//prints board
void print_board(board *g_board){
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            if(g_board->board[i][j]==0){
                 printf("_ ");
            }
            else{
                printf("%d ", g_board->board[i][j]);
            }
        }
        printf("\n");
    }printf("\n");
}
//checks validity
int is_valid_move(board *g_board, int num, char dir){
    int select_row = -1, select_col = -1;
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            if(g_board->board[i][j] == num){
                select_row = i;
                select_col = j;
                break;
            }
        }
    }
    int empty_row = g_board->empty_row;
    int empty_col = g_board->empty_col;
    switch(dir){
        case 'U':
        case 'u':
            if((empty_row == select_row - 1) && (empty_col == select_col)){
                return 1;
            }return 0;
        case 'D':
        case 'd':
            if((empty_row == select_row + 1) && (empty_col == select_col)){
                return 1;
            }return 0;
        case 'L':
        case 'l':
            if((empty_row == select_row) && (empty_col == select_col - 1)){
                return 1;
            }return 0;
        case 'R':
        case 'r':
            if((empty_row == select_row) && (empty_col == select_col + 1)){
                    return 1;
                }return 0;
        default:
            while(getchar()!='\n');
            return 0;
    }
}
//after validation update board
void update_board(board *g_board, int num, char dir){
    int select_row, select_col;
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            if(g_board->board[i][j] == num){
                select_row = i;
                select_col = j;
                break;
            }
        }
    }
    //swap
    g_board->board[g_board->empty_row][g_board->empty_col] = num;
    g_board->board[select_row][select_col] = 0;
    g_board->empty_row = select_row;
    g_board->empty_col = select_col;
}
//checks if game is over or not
int state_chk(board *g_board){
    int *arr = (int*)g_board->board;//linearizing board 
    for(int i=1;i<10;i++){
        if(arr[i-1]!=i%9){
            return 0;
        }
    }
    return 1;
}
//print_board, but to file
void save(board *g_board){
    FILE *fp;
    fp=fopen("puzzle.txt","a");
    if(!fp){
        printf("Error, file couldnt be opened!\n");//shouldnt really happen since append
        return;
    }
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            if(g_board->board[i][j]==0){
                fprintf(fp, "_ ");
            }
            else{
                fprintf(fp,"%d ", g_board->board[i][j]);
            }
        }
        fprintf(fp, "\n");
    }
    fprintf(fp, "\n");
    fclose(fp);
}
//returns a move to be done, did it to optimize stack usage. no need for a function otherwise imo.
char returnMove(board *g_board, char lastMove){//char is also a stack optimization, not because its a char but char is basically a 1 byte number
    int empr = g_board->empty_row;
    int empc = g_board->empty_col;
    int pMoves[4] = {0}; // indexed w, a, s, d
    char move;
    if(empr > 0){
        pMoves[0] = 1; // Up
    }
    if(empc > 0){
        pMoves[1] = 1; // Left
    }
    if(empr < 2){
        pMoves[2] = 1; // Down
    }
    if(empc < 2){
        pMoves[3] = 1; // Right
    }
    if(lastMove>=0){
        pMoves[lastMove]=0;//if is for initiation, thinking of calling it with -1 and the rest will be handled by recursion
    }
    
    do{
        move = rand()%4; // Generate a random number from 0 to 3
    }while(pMoves[move] != 1);
    return move;
}
void auto_finish(board *g_board, char lastMove, unsigned int counter){
    char move=returnMove(g_board,lastMove);//explained before, function and char is for stack optimization functionality is to determine a random move to be made
    print_board(g_board);//print is way too slow
    save(g_board);
    switch(move){
        case 0: // Up
            g_board->board[g_board->empty_row][g_board->empty_col] = g_board->board[g_board->empty_row-1][g_board->empty_col];
            g_board->board[g_board->empty_row-1][g_board->empty_col] = 0;
            g_board->empty_row = g_board->empty_row-1;
            break;
        case 1: // Left
            g_board->board[g_board->empty_row][g_board->empty_col] = g_board->board[g_board->empty_row][g_board->empty_col-1];
            g_board->board[g_board->empty_row][g_board->empty_col-1] = 0;
            g_board->empty_col = g_board->empty_col-1;
            break;
        case 2: // Down
            g_board->board[g_board->empty_row][g_board->empty_col] = g_board->board[g_board->empty_row+1][g_board->empty_col];
            g_board->board[g_board->empty_row+1][g_board->empty_col] = 0;
            g_board->empty_row = g_board->empty_row+1;
            break;
        case 3: // Right
            g_board->board[g_board->empty_row][g_board->empty_col] = g_board->board[g_board->empty_row][g_board->empty_col+1];
            g_board->board[g_board->empty_row][g_board->empty_col+1] = 0;
            g_board->empty_col = g_board->empty_col+1;
            break;
    }
    if(state_chk(g_board)==1){
        printf("PC has solved the puzzle in %d moves\n",counter);
        return;
    }
    return auto_finish(g_board, move, counter+1);
}


/*
1 2  
4 6 5
8 7 9

1 2 5
4 6  
8 7 9

e_r 0
e_c 2

e_r 1
e_c 2

we will be doing a random 1-4 where 1=w 2=s 3=a 4=d
*/