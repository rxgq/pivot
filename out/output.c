#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int fib(int n){
int a=0;
int b=0;
while(b<n){
int temp=a+b;
a=b;
b=temp;
printf(temp);
}
return a;}

int main(int argc,char*argv[]){
fib(10);return 0;
}
