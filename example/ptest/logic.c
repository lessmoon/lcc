#include<stdio.h>

int main()
{
     int a = 0;
     int b = 20;
     int c = 30;

     if(a > 0 || b < 50){//true
         printf("1 Pass\n");
     } else {
         printf("1 Wrong\n");
     }

     if(a > 0 || b > 50){//false
         printf("1' Wrong\n");
     } else {
         printf("1' Pass\n");
     }


     if(a <= 0 && b < 50){//true
         printf("2 Pass\n");
     } else {
         printf("2 Wrong\n");
     }

     if(a > 0 && b < 50){//false
         printf("2' Wrong\n");
     } else {
         printf("2' Pass\n");
     }


     if(a >= 0 || b < 50 && c < 10){//true
         printf("3 Pass\n");
     } else {
         printf("3 Wrong\n");
     }
      
     return 0;
}
