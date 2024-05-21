#include <stdio.h>
#include <string.h>
#include <math.h>
#define MAX_TEXT 150000
#define MAX_WORD_COUNT 1006
#define MAX_WORD_SIZE 15
#define MAX_VECTOR_SIZE 210
#define IFILE "ignore.txt"
#define INFILE "input.txt"
#define DFILE "dictionary.txt"
#define THRESHOLD 26.1

int read_dict(const char * file_name, char dict[][MAX_WORD_SIZE]);
int read_text(const char * text_file, const char * ignore_file, char words[][MAX_WORD_SIZE]);
double dissimilarity(char * w1, char * w2, char dict[][MAX_WORD_SIZE]);
int histogram(const char words[][MAX_WORD_SIZE], const int occurrences[], const char hist[][3*MAX_WORD_SIZE]);
void append(char* w1, char* w2, char* hist, int mode);
int find_closest(char * word, char dict[][MAX_WORD_SIZE]);

int main(){
    char userInput[MAX_WORD_COUNT][MAX_WORD_SIZE],//was previously userInput, will be asked to assistant if i got that correctly
    c;
    char dict[MAX_WORD_COUNT][MAX_WORD_SIZE];
    char words[MAX_TEXT][MAX_WORD_SIZE];//Hippity hoppity, your RAM is my property
    char hist[MAX_WORD_COUNT][3*MAX_WORD_SIZE];//should actually be 2-word size +20 hist array on pdf wasnt enough to store all that(for most words actually)
    float vectors[MAX_WORD_COUNT][MAX_VECTOR_SIZE];
    int i=0,j=0, scale, occurrences[MAX_WORD_COUNT]={0}, userCount, wordCount, cIndex, k, temp;
    printf("Enter word(s): ");
    while((c=getchar())!='\n'){//parsing user input, dependent on \n since we cant really send \0 or EOF on terminal (i dont know how anyways)
        if(c!=' '){
            userInput[i][j]=c;
            j++;
        }
        else{i++;j=0;}
    }
    userCount=i;
    read_dict(DFILE,dict);
    wordCount = read_text("input.txt",IFILE,words);
    //here we will process data we parsed, userInput and words will be compared occurrences will be counted
    for(i=0;i<=userCount;i++){
        for(j=0;j<wordCount;j++){
            if(strcmp(words[j],userInput[i])==0){
                occurrences[i]++;
            }
        }
    }occurrences[i]=-1;
    if(userCount==0){//one word is inputted
        if(occurrences[0]==0){
            printf("\"%s\" doesn't appear in “%s”",userInput[0],INFILE);
            cIndex = find_closest(userInput[0],dict);
            if(cIndex==-2){
                printf(".\n");
                return 0;
            }
            if(cIndex==-1){
                return 0;
            }
            for(i=0;i<wordCount;i++){
                if(strcmp(dict[cIndex],words[i])==0){
                    occurrences[0]++;
                }
            }
            if(occurrences[0]>0){//if closest word has no occurrences we dont print it.
                printf(" but \"%s\" appears %d times",dict[cIndex],occurrences[0]);
            }
        }
        else{
            printf("\"%s\" appears %d times",userInput[0],occurrences[0]);
        }
        printf(".\n");
    }
    else{
        int indexes[25],ind=0;
        int biggest, tempOcc=0;
        for(i=0;occurrences[i]!=-1;i++){
            if(occurrences[i]>biggest){
                biggest=occurrences[i];
            }
            if(occurrences[i]==0){
                indexes[ind] = find_closest(userInput[i],dict);
                //we will count occurences for this closest item
                for(k=0;k<wordCount;k++){
                    if(strcmp(dict[indexes[ind]],words[k])==0){
                        occurrences[i]--;//a trick here i can store occurences and state of the word with this, if negative word is closest word.
                    }
                }//updating biggest if need be
                if(-occurrences[i]>biggest){
                    biggest=-occurrences[i];//changing scale if need be (closest words count is bigger than all other)
                }
                ind++;
            }
        }scale=biggest/21+1;
        if(scale!=1){
            printf("Scale: %d\n",scale);
        }
        ind=0;
        for(i=0;i<=userCount;i++){
            for(k=0;k<3*MAX_WORD_SIZE;k++){
                hist[i][k]=' ';
            }
            if(occurrences[i]>0){
                for(j=0;j<occurrences[i]/scale;j++){
                    hist[i][j+3*MAX_WORD_SIZE-21]='*';
                }
                append(userInput[i]," ",hist[i],1);
            }
            else{
                occurrences[i]=-occurrences[i];//turning back into positive for array stuff
                if(indexes[ind]==-2){
                   occurrences[i]=0;//threshold related
                }
                
                for(k=0;k<occurrences[i]/scale;k++){
                    hist[i][k+3*MAX_WORD_SIZE-21]='+';//placing values to array appropriatly
                }
                append(userInput[i],dict[indexes[ind]],hist[i],2);//also placing values to array, storing word in hist too (const :(   )
                ind++;
            }
        }hist[i][0]='-';//setting sentinel for hist
        histogram(userInput,occurrences,hist);//calling histogram for formatted print
    }   
}
//finds distance didnt use inverse of distance because threshold became way too precise to handle
double dissimilarity(char * w1, char * w2, char dict[][MAX_WORD_SIZE]){
    int w1_index=0, w2_index=0, itemCount, vSize;
    int i, j, flag=0;
    char c;
    double vectors[2][MAX_VECTOR_SIZE], diss=0;
    //first objective - finding indexes of w1 and w2
    i=0;
    while(dict[i][0]!='-'&&flag<2){//flag keeps it from searching further when it found what it searchs for.
        if(strcmp(w1,dict[i])==0){
            w1_index=i;
            flag++;
        }
        if(strcmp(w2,dict[i])==0){
            w2_index=i;
            flag++;
        }
        i++;
    }
    if(dict[w1_index][0]=='-'||dict[w2_index][0]=='-'){return -1;}//not in dict.
    if(w1_index==w2_index){return 0;}//they indicate the same element
    //we found indexes now we scan the vectors.
    FILE *fp;
    fp = fopen(DFILE,"r");
    if(!fp){return -1;}
    while(fgetc(fp)!='=');fscanf(fp,"%d",&itemCount);
    while(fgetc(fp)!='=');fscanf(fp,"%d",&vSize);
    fgetc(fp);//eat \n
    flag=0;
    for(i=0;i<itemCount;i++){
        if(i==w1_index||i==w2_index){
            fscanf(fp,"%*[^ ]");//skip word
            for(j=0;j<vSize;j++){
                fscanf(fp," %lf",&vectors[flag][j]);//scan vector
            }fgetc(fp);//eat \n
            flag++;
        }
        else{
            while(fgetc(fp)!='\n');
        }
    }
    for(i=0;i<vSize;i++){
        diss+=(vectors[0][i]-vectors[1][i])*(vectors[0][i]-vectors[1][i]);
    }
    fclose(fp);
    return diss;
}
//finds closest. Very very slow, would be a lot faster if implemented inside dissimilarity but it was forbidden
int find_closest(char * word, char dict[][MAX_WORD_SIZE]){
    int i,index;
    double lesser, temp;
    lesser = dissimilarity(word, dict[0], dict);
    for(i=1;dict[i][0]!='-';i++){
       temp =  dissimilarity(word, dict[i], dict);
       if(temp!=0 && temp<lesser){
            lesser=temp;
            index=i;
       }
    }
    if(lesser>THRESHOLD){
        return -2;
    }
    return index;
}
//reads file - couldve parsed floats too but hey, prototypes.
int read_dict(const char * file_name, char dict[][MAX_WORD_SIZE]){
    FILE *fp;
    fp=fopen(file_name,"r");
    if(!fp){return -1;}
    int itemCount,vSize,i,j=0,vControl=0;
    char c;
    while(fgetc(fp)!='=');//reading count
    fscanf(fp,"%d",&itemCount);
    while(fgetc(fp)!='=');
    fscanf(fp,"%d",&vSize);
    for(i=0;i<itemCount;i++){
        fscanf(fp,"%s",dict[i]);
        while(fgetc(fp)!='\n');
    }fscanf(fp,"%s",dict[i]);
    while(fgetc(fp)!=EOF);
    fclose(fp);
    dict[i][0]='-', dict[i][1]='\0';
    return itemCount;
}
//Not very useful, reason: const is being sent so its supposed to be useless
int histogram(const char words[][MAX_WORD_SIZE], const int occurrences[], const char hist[][3*MAX_WORD_SIZE]){
    int i, j, temp=0;
    for(i=0;hist[i][0]!='-';i++){
        if(occurrences[i]>temp){
            temp=occurrences[i];
        }
        if(occurrences[i]==0){
            printf("\"%s\"",words[i]);
            for(j=0;words[i][j]!='\0';j++);//strlen, basically
            for(j;j<3*MAX_WORD_SIZE-23;j++){
                printf(" ");
            }printf("NO MATCHES\n");
        }
        else{
            printf("%s\n",hist[i]);
        }
    }
    return temp/21+1;
}
//Stores text in an array
int read_text(const char * text_file, const char * ignore_file, char words[][MAX_WORD_SIZE]){
    FILE* text;
    FILE* ignore;
    text=fopen(text_file,"r");
    ignore=fopen(ignore_file,"r");
    char wIgnore[50][MAX_WORD_SIZE],c;
    int i=0,j=0,ig_size;
    while(fscanf(ignore,"%s",wIgnore[i])!=EOF){
        //printf("%s\n",wIgnore[i]);
        i++;
    }ig_size=i;
    i=0;
    while((c=fgetc(text))!=EOF){
        if((c>='a'&&c<='z')||(c>='A'&&c<='Z')){//isalpha
            words[i][j]=c;
            //printf("%c",c);
            j++;
        }
        if(c==' '){
            words[i][j]='\0';
            for(int k=0;k<ig_size;k++){
                if(strcmp(words[i],wIgnore[k])==0){
                    i--;
                    break;
                }
            }
            j=0;
            //printf("%s ",words[i]);
            i++;
        }
    }
    words[i][0]='-';
    return i;
}
//append function i wrote, basically turns hist array into ready-to-print format
void append(char* w1, char* w2, char* hist, int mode){
    int i,j;
    j=0;
    if(mode==2){
    hist[0]='\"';
        for(i=1;w1[i-1]!='\0';i++){
            hist[i]=w1[j];
            j++;
        }hist[i]='-';hist[i+1]='>';i+=2;j=0;
        for(j=0;w2[j]!='\0';j++){
            hist[i]=w2[j];
            i++;
        }hist[i]='\"';
    }
    else{
        hist[0]='\"';
        for(i=1;w1[i-1]!='\0';i++){
            hist[i]=w1[i-1];
        }
        hist[i]='\"';
    }
    hist[3*MAX_WORD_SIZE-1]='\0';   
}