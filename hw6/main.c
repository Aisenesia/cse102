#include <stdio.h>
#include <string.h>
int myStrcmp(const char *str, const char *rule);
void part1();
void part2();
void part3();

void foo_min_max(int arr[], int n);

int main(){
    part1();
    part2();
    part3();
}

void part1(){//yan yana sayısı ile toplam karakter sayısı karşılaştırılacak 
    char s[100];
    printf("Enter a string: ");
    scanf("%s", s);
    int n=strlen(s);
    int count=0;
    for (int i = 0; i < n; i++) {
        int j=i;
        while (j<n&&s[j]==s[i]){
            j++;
        }
        count++;
        i=j-1;
    }
    printf("%d\n",count);
}

void part2(){
    char string[]="As a noble person, I want to solve this novel problem on my own";
    char rule[]="o*n";
    char *token;
    token=strtok(string," ");
    while (token != NULL){
        if (!myStrcmp(token, rule)) {
            printf("%s ", token);
        }
        token=strtok(NULL, " ");
    }
    printf("\n");
}
void part3(){
    int n;
    printf("Enter number of elements: ");
    scanf("%d", &n);
    int arr[50];
    for(int i=0;i<n;i++){
        printf("Enter %d. element: ", i+1);
        scanf("%d",&arr[i]);
    }
    foo_min_max(arr,n);
}
int myStrcmp(const char *str, const char *rule) {
    int i, j;
    int len_str=strlen(str);
    int len_rule=strlen(rule);
    int match=1;
    for (i=0,j=0;i<len_str&&j<len_rule;i++,j++) {
        if (rule[j]=='*'){
            j++;
            while(i<len_str&&str[i]!=rule[j]) {
                i++;
            }
            if (i==len_str){
                match=0;
                break;
            }
        }
        else if(rule[j]== '?'){
            
        } 
        else if (str[i] != rule[j]){
            match = 0;
            break;
        }
    }
    
    if (j!=len_rule||i!=len_str){
        match=0;
    }
    return match;
}
void foo_min_max(int arr[], int n){
    int i=0;
    int min=arr[i], max=arr[i];
    while(i<n){
        if(arr[i] < min){
            min=arr[i];
        }
        if(arr[i]>max){
            max=arr[i];
        }
        i++;
    }
    printf("Minimum: %d\n", min);
    printf("Maximum: %d\n", max);
}