#include <stdio.h>

void part1(void);
void part2(void);
void part3(void);
void part4(void);
int digitCounter(int a);

int main(){
    part1();
    part2();
    part3();
    part4();
}

void part1(){
    int n1,n2;
    int temp;
    printf("//-------part1---------//\n");
    printf("First Number : ");
    scanf("%d",&n1);
    printf("Second Number: ");
    scanf("%d",&n2);
    //euclidean algorithm
    while(n2){
        temp=n2;
        n2=n1%n2;
        n1=temp;
    }
    printf("Result: %d\n",n1);
    }
void part2(){
    int n1,n2;
    printf("//-------part2---------//\n");
    printf("First Number : ");
    scanf("%d",&n1);
    printf("Second Number: ");
    scanf("%d",&n2);
    printf("Result:\n");
    printf("\t%7d\n",n1);
    printf("\t%7d\n",n2);
    printf("\t+      \n");
    printf("\t-------\n");
    printf("\t%7d\n",(n1+n2)); 
}
void part3(){
    int n1,n2;
    printf("//-------part3---------//\n");
    printf("First Number : ");
    scanf("%d",&n1);
    printf("Second Number: ");
    scanf("%d",&n2);
    //counts digits and fills them in an array
    int digits = 0,temp = n2;
    int numArray[5];
    while(temp){
        numArray[digits]=temp%10;
        temp=temp/10;
        digits++;
    }
    temp=digits;//will be used for a different purpose again to not declare another variable
    printf("Result:\n");
    printf("\t%10d\n",n1);
    printf("\t%10d\n",n2);
    printf("\t*         \n");
    printf("\t----------\n");
    int spc=10;//spaces 10 of "-" so base space count is 10
    while(digits){
        printf("\t");
        for(int i=0;i<spc-digitCounter(numArray[temp-digits]*n1);i++){
            printf(" ");
            
        }
        spc--;
        printf("%d\n",numArray[temp-digits]*n1);
        digits--;
    }
    /*we print according to digit count of number to be printed we did the multiplication with array we created and counted digit count 
    printed " " depending on digit count so its aligned accordingly*/
    printf("\t+         \n");
    printf("\t----------\n");
    printf("\t%10d\n",(n1*n2));
}
void part4(){
    int n;
    printf("//-------part4---------//\n");
    printf("Enter a number in the range 1-10\n");
    scanf("%d",&n);
    if(n<=5 && n>0){
        printf("The integer you entered is less than or equal to 5\n");
    }
    else if(n>5 && n<=10){ 
        printf("The integer you entered is greater than 5\n");
    }
    else{
        printf("Invalid input\n");
    }
}

int digitCounter(int a){
    int dig=1;
    while(a>=10){
        a=a/10;
        dig++;
    }
    return dig;  
}