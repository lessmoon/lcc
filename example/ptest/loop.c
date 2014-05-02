#include<stdio.h>

int main() 
{
    int i,j,k;
    int sum = 0;
    int factor = 1;
    int sum2 = 0;

    /*for test*/
    for(i = 0 ; i <= 100 ; i++){
    	sum = sum + i;
    }
    printf("for:sum = %d\n",sum);
    
    /*while test*/
    j = 10;
    while(j > 0){
	factor = factor * j;
        j --;
    }
    printf("while:factor = %d\n",factor);

    /*do while test*/
    k = 100;
    do{
   	if(k % 2 == 0){
           sum2 = sum2 + k;
	}
        k --;
    }while(k > 0 );
    printf("do while:sum2 = %d\n",sum2);
    return 0;
}
