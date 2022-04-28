#include <stdio.h>

int main(int argc, char *argv[]) {
        if(argc ==2){
                printf("Hello %s\n",argv[1]);
        }else if(argc >2){
                printf("too many arguments");
        }else{
                printf("too little arguments");
        }
}
