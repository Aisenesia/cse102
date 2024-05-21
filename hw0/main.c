#include <stdio.h>
#include "util.h"

int main() {

    int num1, den1;
    int num2, den2;
    int num3, den3;
    printf("\nNumerator for number 1: ");
    scanf("%d",&num1);
    //do while is used so user does not enter denominator as 0
    do{
        printf("\nDenominator for number 1: ");
        scanf("%d",&den1);
        if(den1==0){
            printf("0 is not acceptable as denominator, Please input another number");
        }
    }
    while(den1==0);
    printf("\nNumerator for number 2: ");
    scanf("%d",&num2);
    //do while is used so user does not enter denominator as 0
    do{
        printf("\nDenominator for number 2: ");
        scanf("%d",&den2);
        if(den2==0){
            printf("\n0 is not acceptable as denominator, Please input another number");
        }
    }
    while(den2==0);

    printf("\nFirst number: ");
    fraction_print(num1, den1);
    printf("\n");

    printf("Second number: ");
    fraction_print(num2, den2);
    printf("\n");

    printf("Addition: ");
    fraction_add(num1, den1, num2, den2, &num3, &den3);
    fraction_print(num3, den3);
    printf("\n");

    printf("Subtraction: ");
    fraction_sub(num1, den1, num2, den2, &num3, &den3);
    fraction_print(num3, den3);
    printf("\n");

    printf("Multiplication: ");
    fraction_mul(num1, den1, num2, den2, &num3, &den3);
    fraction_print(num3, den3);
    printf("\n");
    if(num1==0 || num2==0){
        printf("Division: Undefined\n");
    }
    else{
    printf("Division: ");
    fraction_div(num1, den1, num2, den2, &num3, &den3);
    fraction_print(num3, den3);
    printf("\n");
    }
    return(0);
}
