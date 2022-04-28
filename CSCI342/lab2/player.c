/**
 *  
 *                 Author: Anton M. Kopti
 *                 Bridgewater College CS Operating System Homework
 *     
 *      
 **/
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>


//global variable
int health_level = 5;

//signal handlers

//decreasing health signal handler
void decrease_health(int signum){

        health_level--;
        if(health_level==0){
                printf("Enemy %d Killed\n" , getpid());

                exit(0);
        }else{
                printf("HEALTH: %d\n", health_level);
        }

}
//printing health signal handler
void print_health(int sig){
        printf("player: %d has health of: %d\n",getpid(), health_level);
}
//increasing health signal handler
void increase_health(int sig){
        if(health_level==5){
                printf("player's health is at capacity, user can't gain more than 5 points of health\n");
        }else if(health_level <5){
                health_level++;
                printf("Health increased to %d\n", health_level);    
        }
}



int main(){
        int pid;

        signal(SIGUSR1, decrease_health);
        signal(SIGUSR2, increase_health);
        signal(SIGINT, print_health);
        while(1){
                pause();
        }

        return 0;
}
