#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int fib(int n){
if(n<=0){
return 0;}
if(n==1){
printf(0);
return 0;}
if(n==2){
printf(0);
printf(1);
return 1;}
int a=0;
int b=1;
int count=2;
while(count<n){
int temp=a+b;
a=b;
b=temp;
printf(temp);
count=count+1;
}
return b;}

int main(int argc,char*argv[]){
fib(10);return 0;
}
