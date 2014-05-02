#include<stdio.h>

int main()
{
   int a,b;
   a = 25;
   b = 30;
   printf("%d,%d\n",a,b);
   if(a > b){
      printf("a > b\n");  
   } else if( a == b) {
      printf("a == b\n");
   } else {
      printf("a < b\n");
   }
   
   if(a < b){
      a = a + b;
      printf("a = a + b \n");
   }

   printf("%d,%d\n",a,b);
   
   return 0;
}
