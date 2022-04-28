#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
int main(){

        int x =10;
        int pid;
        //print the address of x in the parent
        printf("value of X in the parent is  %p\n", &x);
        if((pid= fork())==0){
                printf("child: %d\n",x);
                printf("value of X in child before attempting to write is  %p\n", &x);
                x = 5;
                 printf("value of X in child after attempting to write is  %p\n", &x);
         printf("child: %d\n",x);
        exit(0);
        }
        wait(NULL);
         printf("value of X in the parent is  %p\n", &x);
         printf("%d\n",x);
return 0;
}
