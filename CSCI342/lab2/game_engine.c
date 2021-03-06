/**
 *
 *              Author: Anton M. Kopti
 *              Bridgewater College CS Operating System Homework*
 */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>


//declaring some global variable 
int pid[100];
int latestPlayer = 0;

//some important functions

/*
 **              A function that prints out all child processes and prompts a question 
 **              to the user asking for a pid input
 **              this will be used in multiple functions                         */

void printChildren(){
        int i;
        for(i=2;i<latestPlayer;i++){
                if(pid[i]!= 0){
                        printf("\n\t\t *child %d with pid: %d\n**", i, pid[i]);
                }
        }

                printf("\t Please pick a PID\n");
}



        /**A function that forks a child process with the executable ./player123**/
void spawnPlayer(){
        int position = latestPlayer;
        latestPlayer++;
        if((pid[position]=fork())==0){
                //create a child program and execute the exec syscall                execv("./player123", NULL);
        }
}

/**
 *              Function below will scan from the keyboard the pid chosen by the user and
 *              then sends a SIGUR1 signal to that child process     
 */
void injurePlayer(){
        printChildren();
        int chosenPID;
        scanf("%d", &chosenPID);
        kill(chosenPID, SIGUSR1);
}

/*
 *              The function below will scan the id inputted from the user and then send
 *              a SIGUSR2 signal to that child process
 */

void healPlayer(){
        printChildren();
        int chosenPID;
        scanf("%d", &chosenPID);
        kill(chosenPID, SIGUSR2);
}

/*
 *              the function below will list all child processes of the user by sending
 *              each child process a SIGINT signal
 */

void listPlayers(){
        int i;
        for(i=0; i<latestPlayer;i++){
                printf("%d", pid[i]);
                if(pid[i] !=0 ){
                        kill(pid[i], SIGINT);
                        sleep(1);
                }


        }
}

/*
 *              this function will list the options after every user input
 */

void printOptions(){
        printf("click one of the following options \n");
        printf(" 1.   Spawn Player \n 2.   Injure Player \n 3.   Heal Player \n 4.   List Players \n 5.   Quit \n");
}



//main driver

int main(){

        printOptions();
        //the while statementi
        int usrInp;
        while(1){
                scanf("%d", &usrInp);
                if(usrInp == 1){
                        //this is for spwaning a player
                        spawnPlayer();
                        int position = latestPlayer;
                        printf("\n\t*** New Player Spawned with pid of: %d***\n\n", pid[--position]);
                        printOptions();
                }else if(usrInp ==2){
                        if(latestPlayer==0){
                                printf("\n\n \tNo existing players, please spawn a player to injure\n\n");
                                printOptions();
                        }else{
                        //thi is for injuring a specific player      
                                injurePlayer();
                                sleep(1);
                                printOptions();
                        }
                }else if(usrInp ==3){
                        if(latestPlayer==0){
                                printf("\n\n \t No existing player to heal, please spawn a player\n\n\n");
                                printOptions();
                        }else{
                        //this is for healing a specific player      
                                healPlayer();
                                sleep(1);
                                printOptions();
                        }
                }else if(usrInp ==4){
                        //this is for listing all  players
                        listPlayers();
                        printOptions();
                }else if (usrInp ==5){
                        //this is for quitting the game
                        int i, status, retpid;
                        for(i=0;i<latestPlayer;i++){
                                kill(pid[i], SIGKILL);
                                printf("Child: %d has been killed\n", pid[i]);
                        }
                        i=0;
                        while((retpid=waitpid(pid[i++], &status, 0)) >0){
                                if(WIFEXITED(status)){
                                        printf("child %d terminated normally with exit status %d\n", retpid, WEXITSTATUS(status));        
                                }else{
                                        printf("child %d terminated abnormally\n", retpid);}}
                        break;
                }else{
                        //if the user inputted in a number that's not between 1-5
                        printf("not an option please choose an option between 1-5");
                }
        }
        //after breaking from the while loop
        printf("GAME OVER");
        return 0;
}
