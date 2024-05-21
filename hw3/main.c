#include <stdio.h>
#include <stdlib.h>
#include <time.h>//for srand (proper rng)

#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'



void draw_room(int character_j, int character_i, int door_j, int door_i, int size);
void main_game();
int status_control(int character_j, int character_i, int door_j, int door_i);
void menu();

int main(){
    srand(time(NULL));
    menu();
}

void menu(){
    int menuSelection;
    system("clear");
    printf("Welcome to the 2D puzzle game!\n\n1. New Game\n2. Help\n3. Exit\n\nEnter your selection: ");
    scanf("%d",&menuSelection);
    getchar();
    switch(menuSelection){
        case 1:
            main_game();
            break;
        case 2:
            system("clear");
            printf("\n------------HELP------------\n* Game creates rooms a character and a door.\n* Game will create a square with each side is size of your choice.\n* ");
            printf("Size must be between 5 and 10\n* You are indicated with \"C\" and the door is indicated with \"D\"\n* Your purpose is to move to the door using \"W, A, S, D\" keys");
            printf("\n* You cannot skip above 4 walls of the square\n\n\n");
            printf("ENTER to Return ");
            while(getchar()!='\n'){}
            return menu();
            break;
        case 3:
            printf("\nGoodbye!\n\n");
    }
    
}
void draw_room(int character_j, int character_i, int door_j, int door_i, int size){//draws room according to parameters: character pos, door pos and size
    int i,j;
    printf("\n");
    for(i=0;i<size;i++){//i printed dashes with additional for loops to avoid checking everytime
        printf("--");
    }
    printf("-\n");
    for(i=1;i<size+1;i++){
        for(j=1;j<size+1;j++){
            printf("|");
            if(j==door_j&&i==door_i){
                printf("D");
            }
            else if(j==character_j&&i==character_i){
                printf("C");
            }
            else{
                printf(" ");
            }
        }
        printf("|\n");
    }
    for(i=0;i<size;i++){
        printf("--");
    }
    printf("-\n");

}

int status_control(int character_j, int character_i, int door_j, int door_i){
    if(character_j==door_j&&character_i==door_i){
        return 1;
    }
    else{
        return 0;
    }
}

void main_game(){
    char move;
    int size;
    int door_i;//y axis (top to bottom)
    int door_j; //x axis
    int character_i;
    int character_j;
    int move_count=0;
    int valid_move;
    int flag=0;
    system("clear");
    do{
        printf("Enter size: ");
        scanf("%d",&size);
        getchar();//done to clear buffer so when user enters for ex a letter instead of numbers it doesnt go mad 
        if(size<5||size>10){
            printf("Invalid Size!\n");
        }
    }while(size<5||size>10);
    do{
        door_i = rand()%(size-1)+1;//y axis 
        door_j = rand()%(size-1)+1 ; //x axis
        character_i = rand()%(size-1)+1;
        character_j = rand()%(size-1)+1;
    }while(character_i==door_i&&character_j==door_j);
    
    while(status_control(character_j, character_i, door_j, door_i)==0){
        system("clear");
        flag=0;
        draw_room(character_j,character_i,door_j,door_i,size);
        printf("\nEnter your move: ");
        scanf(" %c",&move);
        while(getchar()!='\n'){
            flag=1;
        }
        if(flag){
            printf("Error, multiple characters at once\n\nENTER to Return ");
            getchar();
        }
        else{
            valid_move=1;
            switch (move)
            {
            case UP:
                if(character_i==1){
                    valid_move=0;
                    printf("\nInvalid move!\n\nENTER to Return ");
                    getchar();
                    break;
                }
                character_i--;
                break;
            case DOWN:
                if(character_i==size){
                    valid_move=0;
                    printf("\nInvalid move!\n\nENTER to Return ");
                    getchar();
                    break;
                }
                character_i++;
                break;
            case LEFT:
                if(character_j==1){
                    valid_move=0;
                    printf("\nInvalid move!\n\nENTER to Return ");
                    getchar();
                    break;
                }
                character_j--;
                break;
            case RIGHT:
                if(character_j==size){
                    valid_move=0;
                    printf("\nInvalid move!\n\nENTER to Return ");
                    getchar();
                    break;
                }
                character_j++;
                break;
            default:
                printf("Invalid input\n\nENTER to Return ");
                getchar();
                valid_move=0;
                break;
            }
            
            if(valid_move){
                move_count++;
            }
        }
    }
    printf("Game is now over!\nYou have completed the game in %d moves\n\nENTER to Return ",move_count);
    getchar();
    return menu();
}



/*
          O
            -----------------
            | | | |D| | | | |
            | | | | | | | | |
            | | | | | | | | |
            | | | | | | | | |
            | |C| | | | | | |
            | | | | | | | | |
            | | | | | | | | |
            | | | | | | | | |  
            -----------------
O is the origin point
*/