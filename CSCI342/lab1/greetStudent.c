#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>



int main(){
        char *names[4]={"./greeting","anton", "jack","jerry"};       
        int pid[2];//this is for the future creation of children     
        int retpid, status, i;
        for(i=0;i<=2;i++){
                if((pid[i]=fork())==0){
                        execlp(names[0], names[i+1]);
                        exit(100+i);
                }else if(pid[i] <0){
                        printf("fork error %s\n", strerror(errno));  
                }
        }
        i=0;
        int counter =0;
        while((retpid = waitpid(pid[i++], &status,0))>0){
                if(WIFEXITED(status)){
                        //printf("child %d terminated normally with exit status=%d\n", retpid, WEXITSTATUS(status));
                        counter++;
                }else{
                        printf("child %d terminated abnormally\n", retpid);
                }

        if(counter==3){
                printf("Parent finished\n");
        }
        //abnormal termination with no children
        if(errno != ECHILD){
                perror("waitpid error");
        }
        exit(0);
}
