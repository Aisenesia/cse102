#include <stdio.h>
#include "util.h"

void fraction_print(int numerator, int denominator) {
    /*if(numerator%denominator==0){
        printf("%d",(numerator/denominator));
    }*/
    /*else if(numerator==0){//dont forger to add else if upwards piece of code is uncommented
        printf("0");
    }*/
    //else{
    printf("%d//%d", numerator, denominator);
    //}
}  /* end fraction_print */

void fraction_add(int n1, int d1, int n2, int d2, int * n3, int * d3) {
    *n3 = n1*d2 + n2*d1;
    *d3 = d1*d2;
    fraction_simplify(n3, d3);
} /* end fraction_add */

void fraction_sub(int n1, int d1, int n2, int d2, int * n3, int * d3) {
    *n3= n1*d2 - n2*d1;
    *d3= d1*d2;
    fraction_simplify(n3, d3);
} /* end fraction_sub */

void fraction_mul(int n1, int d1, int n2, int d2, int * n3, int * d3) {
    *n3=n1*n2;
    *d3=d1*d2;
    fraction_simplify(n3, d3);
} /* end fraction_mul */

void fraction_div(int n1, int d1, int n2, int d2, int * n3, int * d3) {
    *n3=n1*d2;
    *d3=n2*d1;
    fraction_simplify(n3, d3);
} /* end fraction_div */

void fraction_simplify(int * n, int * d) {
int i;
if(*n==0){
    return;
    //If numerator is 0 than division will give error, so we just skip it
}
if(*n > *d){
    i=*d;
}
else {
    i=*n;
}
//we start a loop with i being the lesser of numerator and denominator and it counts backward, decrementing i one by one till it finds gcd
if(i<0){
    i=(-i);//taking absolute value for simplifying when negative
}
while(*n%i != 0 || *d%i != 0){
    i--;
}
*n=*n/i;
*d=*d/i;
}
