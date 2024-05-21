#include <stdio.h>
void part1(void);
void part2(void);
void part3(void);
int digitCounter(double a);
double modulus(double a, double b);
void scanner(int* o1,int* o2);
int pwon(int a);
double roun(double a,int m);
int main(){
    part1();
    part2();
    part3();
}
void part1(void){
    int userInput;
    printf("***************************\nPlease enter a year: ");
    scanf("%d",&userInput);
    if(userInput%4||userInput%400&&!userInput%100){//4 100 400 cases has been considered
        printf("%d is not a leap year\n",userInput);
    }
    else{
        printf("%d is a leap year\n",userInput);
    }
}
void part2(void){
    char mode,operation;
    int m,n;
    int operand1,operand2;
    double result=0;
    printf("*********************\n");
    printf("Enter format of output (S or I): ");
    scanf(" %c",&mode);
    if(mode=='S'){
        do{
        printf("Enter m and n values: ");
        scanf("%d %d",&m,&n);
        if(n>m){
            printf("Value for n cannot be greater than m, RETRY!\n");
        }
        }while(n>m);
    }
    else if(mode!='I'){
        printf("S and I are only operable output types!\n");
        return;
    }
    printf("Enter the operation(+,-,/,*,%%,!,^): ");
    scanf(" %c",&operation);    
    switch (operation){
        case '!': 
            printf("Enter the operand: ");
            scanf("%d",&operand1);
            result=1;
            printf("%d!= ",operand1);
            while(operand1>0){
                result*=operand1;   
                operand1--;
            }            
        break;
        case '^': 
            result=1;
            scanner(&operand1,&operand2);
            while(operand2){
                result*=operand1;
                operand2--;
            }
        break;
        case '+':
            scanner(&operand1,&operand2);
            result=operand1+operand2;
            printf("%d%c%d= ",operand1,operation,operand2);
        break;
        case '-':
            scanner(&operand1,&operand2);
            result=operand1-operand2;
            printf("%d%c%d= ",operand1,operation,operand2);
        break;
        case '*':
            scanner(&operand1,&operand2);
            result=operand1*operand2;
            printf("%d%c%d= ",operand1,operation,operand2);
        break;
        case '/':
            scanner(&operand1,&operand2);
            result=(double)operand1/(double)operand2;
            printf("%d%c%d= ",operand1,operation,operand2);
        break;
        case '%':
            scanner(&operand1,&operand2);
            result=operand1%operand2;
            printf("%d%c%d= ",operand1,operation,operand2);
        break;
        default:
            printf("Invalid Operator!\n");
            return;
    }
    if(mode=='S'){
        int e=0;
        result=roun(result,m);
        if(result&&result>pwon(m-n)){//digit shifting and counting for e
            while(digitCounter(result)!=(m-n)){
                result/=10;
                e++;
            }
        }
        else if (result&&result<pwon(m-n)){
            while(pwon(m)>result&&digitCounter(result)!=(m-n)){
                result*=10;
                e--;
            }
        }
        //I aligned them left to right so in contrast to example given to us (0000.1234e8) it prints 1234.0000e4 (for m=8 n=4) which is the same thing essentially
        //I think this makes more sense because leftmost digit is more significant thus i did as such
        printf("%0*.*lfe%d\n",m+1,n,result,e);
    }
    else{
        if(modulus(result,1.0)==0){
            printf("%d\n",(int)result);
        }
        else{
        printf("%lf\n",result);
        }
    }
}
void part3(void){
    double sum;
    int e1,e2,e3,a1,a2;
    printf("***************************\nEnter 3 exam grades of student: ");
    scanf(" %d %d %d",&e1,&e2,&e3);
    if(e1>100||e1<0||e2>100||e2<0||e3>100||e3<0){printf("Invalid Input!\n");return;}
    printf("Enter 2 assignment grades of student: ");
    scanf(" %d %d",&a1,&a2);
    if(a1>100||a1<0||a2>100||a2<0){printf("Invalid Input!\n");return;}
    sum=e1+e2+e3+a1+a2;
    printf("Final grade: %.1lf ,",(sum/5));
    if((sum/5)>60){
        printf("Passed!");
    }
    else{
        printf("Failed!");
    }
    printf("\n**********************************************\n");
}
int digitCounter(double a){
    int dig=0;
    if(a>-1&&a<1){//for decimals between 1 and -1 i used decimal to array transformation
        char d_to_str[16];
        sprintf(d_to_str,"%.8lf",a);
        int i;
        for(i=10;i>0;i--){
            dig++;
            if(d_to_str[i-1]!='0'){
                break;
            }    
        }
        return 9-dig;
    }
    else{
        while((int)a){
            a=a/10;
            dig++;
        }
    }
    return dig;  
}
//modulus for decimals
double modulus(double a, double b) {  
    return a-(int)(a/b)*b;
}
//scans for values
void scanner(int* o1,int* o2){
    printf("Enter the first operand: ");
    scanf("%d",o1);
    printf("Enter the second operand: ");
    scanf("%d",o2);
}
//basically 10^a
int pwon(int a){
    int x=1;
    for (int i=0;i<a;i++){
        x*=10;
    }
    return x;
}
//rounds the decimal this is done to avoid errors caused by repeating digits (ex: 11.111..)
double roun(double a,int m){
    int i=0;
    if(modulus(a,10.0)==0){
        return a;
    }
    else{
        for ( i=0;i<m;i++){
            a*=10;
        }
        a=(int)a;
        for(i=0;i<m;i++){
            a/=10;
        }
        return a;
    }   
}
