#include <stdio.h>
#include <stdlib.h>//random
#include <time.h>//srand and time printing

#define VAT 0.18
#define STUDENT_DIS 0.125
#define ADD_DIS 0.1
#define ROCK 1
#define PAPER 2
#define SCISSORS 3

int displayMenu(); //Parse and display menu return number of items on the menu
double findPrice(int line); //Return price of the item in line entered as parameter
int printItem(FILE *rec,int line); //print item on line entered as parameter in file entered as parameter, return length of the printed line
void order(); //taking order and calling functions as needed. Base of part1 as you may call it
void displayReceipt();//printing receipt
void part2();//rock,paper,scissors
void part2sw(int a); //for shortening code

int main(){
    order();//fully dynamic you can change menu as you wish except when dish names are too long also turkish characters create issues in c
    part2();
}    
void order(){
    FILE *receipt;
    receipt=fopen("receipt.txt","w");
    int menuSelection,menuLen,quantity,lineLen,lenFlag=0;
    double toPay=0,temp;
    char userInput;
    time_t timer;   
    char timeStr[18]; //only for time strftime puts time in a string and said string is entered in this function as parameter so i used strftime as instructed and thus i have to use string here  
    struct tm* tm_info;//also used for time
    time(&timer);
    tm_info = localtime(&timer);
    strftime(timeStr,18,"%d.%m.%Y/%H:%M", tm_info);//taking time to a string
    fprintf(receipt,"220104004073        %s\n",timeStr);//Used spaces instead of \t because \t causes problems for different terminals whereas ' ' is a reliable solution
    fprintf(receipt,"------------------------------------\nProduct                 Price (TL)\n-----------------------------------\n");
    menuLen = displayMenu();
    while(printf("Please choose a product (1-%d): ",menuLen)&&scanf("%d",&menuSelection)&&menuSelection!=0){//printing scanning and checking for 0 in while
           if(menuSelection>menuLen||menuSelection<0){//10 items on the menu but user tried to select 11th item or -5th item which is not possible
            printf("Invalid Choice!\n");
        }
        else{
            do{
                printf("How many servings do you want? ");
                scanf("%d",&quantity);
            }while(quantity<1&&printf("Error, invalid number of servings!\n"));
            lenFlag=0;
            if(quantity>1){
                fprintf(receipt,"%d* ",quantity);//printing quantity in front of item ex. 2* Lahmacun
                lenFlag=3;//if quantity and asterisk is printed lenght of line increases by 3 and we do calculations based on length so we should take that into account
            }
            lineLen = lenFlag+printItem(receipt,menuSelection);//prints item and takes its length at once
            while(lineLen!=24){//printing ' ' according to presented format
                fprintf(receipt," ");
                lineLen++;
            }
            temp=findPrice(menuSelection+1);//+1 for alignment
            toPay+=(double)quantity*temp;//calculating price to be paid
            fprintf(receipt,"%-6.2lf\n",(double)quantity*temp);
        }
    }
    fprintf(receipt,"Total:                  %-6.2lf\n",toPay);
    do{
        printf("Are you a student? (Y/N) ");
        scanf(" %c",&userInput);
        if(userInput=='Y'||userInput=='y'){
            temp=1;
        }
        else if(userInput=='N'||userInput=='n'){
            temp=0;
        }
        else{
            temp=-1;
        }
    }while(temp==-1&&printf("Error, invalid input!\n"));
    if(temp){
        fprintf(receipt,"Student Discount:       -%-6.2lf\n",toPay*STUDENT_DIS);
        toPay-=toPay*STUDENT_DIS;//*0.125
    }
    if(toPay>150){
        toPay-=toPay*ADD_DIS;//*0.1
        fprintf(receipt,"Bonus Discount:         -%-6.2lf\n",toPay*ADD_DIS);
    }
    fprintf(receipt,"-----------------------------------\nPrice:                  %-6.2lf\n",toPay);
    toPay+=toPay*VAT;
    fprintf(receipt,"Price + VAT:            %-6.2lf",toPay);
    fclose(receipt);
    return displayReceipt();//calling this function to print newly created receipt to terminal
}


int displayMenu(){
    FILE *fp;
    fp= fopen("menu.txt","r");
    if(!fp){
        printf("Error reading file\n");
        return 0;
    }
    char c=0;
    int index=0;
    printf("Yemek listesi: \n");
    while(fscanf(fp,"%c",&c)!=EOF){
        if(index&&(c<'.' || c>'9')){//skips printing numbers so it doesnt print prices
            printf("%c",c);
        }
        if(c=='\n'){
            index++;
            printf("%d. ",index);
        }
    }
    printf("\n0. Finish the order\n\n");
    fclose(fp);
    return index;//returning item count for some error messages and stuff
}

double findPrice(int line){
    FILE *fp;
    fp= fopen("menu.txt","r");
    double price;
    int flag=0;
    char c=1;//initialization
    int i=1;//1 instead of zero for skipping "Product Price (TL)" line
    while(i<line&&(fscanf(fp,"%c",&c)!=EOF)){//skipping lines till the line we want to find price of
        if(c=='\n'){
            i++;
        }
    }
    while(flag<2&&(fscanf(fp,"%c",&c)!=EOF)){
        if(c==' '){
            flag++;//for skipping 2 worded dishes, aligning pointer to price
        }
        else{
            flag=0;
        }
    }
    fscanf(fp," %lf",&price);
    fclose(fp);
    return price;
}

int printItem(FILE *rec,int line){//prints item indexed as {line} from menu
    FILE *fp;
    fp= fopen("menu.txt","r");
    if(!fp){
        printf("Error reading file\n");
        return 0;
    }
    int i=0;
    char c;
    int flag=0;
    int lineLen=0;
    while((fscanf(fp,"%c",&c)!=EOF)){
        if(!(line-i)&&((c<'.' || c>'9')&&c!='\n'&&flag<2)){//skips printing numbers so it doesnt print prices
            if(c==' '){
                flag++;//for dishes that include 2-3 words ex. Etli Pide
            }
            else{flag=0;}
            lineLen++;
            fprintf(rec,"%c",c);
        }
        if(c=='\n'){
            i++;
        }
    }
    fclose(fp);
    return lineLen;//we already counted parsed list so why not return lineLen for future uses?
}

void displayReceipt(){//prints receipt taken from receipt.txt file created by order(); function
    FILE *receipt;
    receipt = fopen("receipt.txt","r");
    if(!receipt){
        printf("Error reading file\n");
        return;
    }
    char c;
    printf("\n");
    while(fscanf(receipt,"%c",&c)!=EOF){
        printf("%c",c);
    }
    fclose(receipt);
    printf("\n\n");
}

void part2(){//Rock paper scissors game
    srand(time(0));
    int userNum,machineNum;
    char userInput='y';//initialization
    while((userInput!='N'&&userInput!='n')){
        if(userInput=='Y'||userInput=='y'){
            machineNum=(rand()%3)+1;
            printf("Please make a choice!\n1: Rock, 2: Paper, 3: Scissors\n");
            scanf(" %d",&userNum);
            if(userNum<1||userNum>3){
                printf("Invalid input!");
            }
            else{
                printf("You chose ");
                part2sw(userNum);
                printf("I chose ");
                part2sw(machineNum);
                if(userNum==machineNum){
                    printf("It's a tie!");
                }
                else if ((userNum==ROCK&&machineNum==SCISSORS)||(userNum==SCISSORS&&machineNum==PAPER)||(userNum==PAPER&&machineNum==ROCK)){
                    printf("You won!");
                }
                else{
                    printf("I won!");
                }
            } 
        }
        else{
                printf("Invalid choice!");
            }
        printf("\nDo you want to play again? (Y/N):");
        scanf(" %c",&userInput);
    }
    printf("Goodbye!\n");
    return;
}

void part2sw(int a){//for shortening code
    switch (a){
    case ROCK:
        printf("Rock. ");
        break;
    case PAPER:
        printf("Paper. ");
        break;
    case SCISSORS:
        printf("Scissors. ");
        break;
    }
}





/*

 while((userInput!='N'&&userInput!='n')){
        if(userInput=='Y'||userInput=='y'){
            machineNum=(rand()%3)+1;
            printf("Please make a choice!\n1: Rock, 2: Paper, 3: Scissors\n");
            scanf(" %d",&userNum);
            if(userNum<1||userNum>3){
                printf("Invalid input!");
            }
            else{
                printf("You chose ");
                part2sw(userNum);
                printf("I chose ");
                part2sw(machineNum);
                if(userNum==machineNum){
                    printf("It's a tie!");
                }
                else if ((userNum==ROCK&&machineNum==SCISSORS)||(userNum==SCISSORS&&machineNum==PAPER)||(userNum==PAPER&&machineNum==ROCK)){
                    printf("You won!");
                }
                else{
                    printf("I won!");
                }
            } 
        }
        else{
                printf("Invalid choice!");
            }
        printf("\nDo you want to play again? (Y/N):");
        scanf(" %c",&userInput);
    }

*/