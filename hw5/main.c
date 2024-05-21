#include <stdio.h>
#include <math.h>

//personally, if function declerations werent that specific i would've preferred using self-defined color codes 1 to 10 similar to hex colors 0 to 255 approach
//would've been simpler, more efficient, no need for arrayAssigner and stuff
typedef enum {
    RED,
    GREEN,
    BLUE,
    YELLOW,
    ORANGE
} Color;

void part1();
char caseEqualizer(char a);//dont think its actually necessary, we couldve implemented part1 in such a way that this isnt needed
void part2();
Color mix_colors(Color c1, Color c2);
void colorMixer(Color c1, Color c2, Color (*mix_func)(Color,Color));
void arrayAssigner(Color c,float arr[]);
void part3();
void displayTable(int table[][3]);
int checkWin(int table[][3]);

int main(){
    part1();
    part2();
    part3();
}

void part1(){
    printf("--------------------------PART-1--------------------------\n");
    FILE *fp;
    char fileName[128], readChar;
    int alphabetCount[26]={0};
    printf("Enter the file name: ");
    scanf("%s",fileName);
    fp = fopen(fileName,"r");
    if(!fp){
        printf("Error reading file!\n");
        return;
    }
    printf("Letter Frequency:\n");
    while(fscanf(fp,"%c",&readChar)!=EOF){
        readChar=caseEqualizer(readChar);
            alphabetCount[readChar-'a']++;//indexing using ascii
    }
    fclose(fp);
    for(int i=0;i<26;i++){
        printf("%c: %d\n",(char)(i+'A'),alphabetCount[i]);
    }
}

char caseEqualizer(char a){
    if(a>='A'&&a<='Z'){
        return a-'A'+'a';
    }
    return a;
}

void part2(){
    printf("--------------------------PART-2--------------------------\n");
    char colorc[3]; 
    Color color[2];
    int i=0;
    while(i<2){//works twice
        printf("Enter color %d (r,g,b,y,o): ",i+1);
        scanf(" %c",&colorc[i]);//scanning into char and assigning appropriatly based on char
        switch (colorc[i]){
        case 'r':
            color[i]=RED;
            break;
        case 'g':
            color[i]=GREEN;
            break;
        case 'b':
            color[i]=BLUE;
            break;
        case 'y':
            color[i]=YELLOW;
            break;
        case 'o':
            color[i]=ORANGE;
            break;
        default:
            printf("Error! invalid color.\n");
            i--;//repromting, reindexing
            break;
        }
        i++;
    }
    colorMixer(color[0], color[1], mix_colors);
}

void colorMixer(Color c1, Color c2, Color (*mix_func)(Color,Color)){//im aware that there are same statements as in mix_colors but we were instucted not to send array in mix_colors function
    float c1a[3],c2a[3],result[3];
    arrayAssigner(c1,c1a);
    arrayAssigner(c2,c2a);
    result[0]=(c1a[0]+c2a[0])/2;//im confused but i think this function should do something thats why i just didnt call mix_func and implemented it as necesseraly to print both colors and color floats
    result[1]=(c1a[1]+c2a[1])/2;
    result[2]=(c1a[2]+c2a[2])/2;
    Color resultingColor=mix_func(c1,c2);
    printf("Mixed Color: ");
    switch(resultingColor){//print color name
        case RED:
            printf("RED");
            break;
        case GREEN:
            printf("GREEN");
            break;
        case BLUE:
            printf("BLUE");
            break;
        case YELLOW:
            printf("YELLOW");
            break;
        case ORANGE:
            printf("ORANGE");
            break;
    }
    printf(" [%1.1f, %1.1f, %1.1f]\n",result[0],result[1],result[2]);//print color code
}

Color mix_colors(Color c1, Color c2) {
    float c1a[3],c2a[3],result[3];
    arrayAssigner(c1,c1a);//assigning floats based on color
    arrayAssigner(c2,c2a);
    result[0]=(c1a[0]+c2a[0])/2;
    result[1]=(c1a[1]+c2a[1])/2;
    result[2]=(c1a[2]+c2a[2])/2;
    double distances[5];
    float comparedColor[3];//a temporary array for holding different color vals
    int i;
    for (i=0;i<5;i++){
        arrayAssigner(i,comparedColor);//actually using enumeration here. Not a have-to, just didnt want to write one by one 
        distances[i]=sqrt(pow(result[0]-comparedColor[0],2)+pow(result[1]-comparedColor[1],2)+pow(result[2]-comparedColor[2],2));//vector distance calculation
    }
    double lesser=distances[0];
    int closestIndex=0;
    for (i=1;i<5;i++){//comparison, assigning first decimal and holding on to it until an even lesser number is encountered
        if (distances[i]<lesser) {
            lesser=distances[i];
            closestIndex=i;
        }
    }
    return closestIndex;
}
//Assigns values to arrays appropriately
void arrayAssigner(Color c, float arr[]){
    switch (c){
        case RED:
            arr[0]=1.0, arr[1]=0.0, arr[2]=0.0;
            break;
        case GREEN:
            arr[0]=0.0, arr[1]=1.0, arr[2]=0.0;
            break;
        case BLUE:
            arr[0]=0.0, arr[1]=0.0, arr[2]=1.0;
            break;
        case YELLOW:
            arr[0]=0.5, arr[1]=0.5, arr[2]=0.0;
            break;
        case ORANGE:
            arr[0]=0.5, arr[1]=0.4, arr[2]=0.2;
            break;
    }
}

void part3(){
    printf("--------------------------PART-3--------------------------\n");
    char userInput, c[3] ="XO"; //3rd is \0
    do{
        int table[3][3]={3,4,5,6,7,8,9,10,11};//trash values so that table elements are not the same (didnt want to add another control mechanism to checkWin, seemed more efficient this way)
        int errorFlag=0;
        int row=-1,col=-1,activePlayer=2,temp;
        while(checkWin(table)){//activePlayer is supposed to be 1 or 2
            activePlayer=activePlayer%2+1;//a special swap for only 1 and 2
            do{
                errorFlag=0;
                printf("Player %d (%c), enter your move (row, col): ", activePlayer, c[activePlayer-1]);
                scanf(" %d %d",&row,&col);
                while(getchar()!='\n'){//to consume characters if inputted
                    errorFlag=1;
                }
                if(errorFlag==1){
                    printf("Error! invalid input type");
                }
                else if(row<0||row>2||col<0||col>2){
                    printf("Error! entered move does not represent any place in table.\n");
                    errorFlag=1;
                }
                else if(table[row][col]==1||table[row][col]==2){
                    printf("Error! that position has already been taken.\n");
                    errorFlag=1;
                }
            }while(errorFlag==1);//checks whether the move entered is viable (is it empty, does it correspond to an actual space on board etc.)
            table[row][col]=activePlayer;
            displayTable(table);
        }
        printf("Player %d wins!\nWant to play again? (y/n) ",activePlayer);
        scanf("%c",&userInput);
    }while(userInput=='y');//USERSELECTION);
}

int checkWin(int table[][3]){//conditions [x][0,1,2] is same [0,1,2][x] is same 
    //for player 1 and player 2 
    int i,j,temp,flag;
    for(i=0;i<3;i++){
        if(table[i][0]==table[i][1]&&table[i][1]==table[i][2]){
            return 0;//0 terminates while loop
        }
    }
    for(j=0;j<3;j++){
        if(table[0][j]==table[1][j]&&table[1][j]==table[2][j]){
            return 0;
        }
    }
    if((table[0][0]==table[1][1]&&table[1][1]==table[2][2])||(table[2][0]==table[1][1]&&table[1][1]==table[0][2])){
        return 0;
    }
    return 1;//1 continues the loop
}
void displayTable(int table[][3]){
    int i,j;
    for(i=0;i<3;i++){
        for(j=0;j<3;j++){
            if(table[i][j]==1){
                printf("X ");
            }
            else if(table[i][j]==2){
                printf("O ");
            }
            else{
                printf("_ ");
            }
        }
        printf("\n");
    }
    printf("\n");
}