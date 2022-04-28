#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include<time.h>

//Macro defined
#define ARRLEN 3; //will be a constant since the board is 3x3        
#define new_max(x,y) (((x) >= (y)) ? (x) : (y));
#define new_min(x,y) (((x) <= (y)) ? (x) : (y));
struct threadInfo{
        int arr[3][3];
        int curPlayer;
        bool gameOver;
        int curiter; //this will increment everytime a player has played a turn and when it reached 9 it will
        int savedpos[2]; // this will be used to store the last move of the last player
};

//methods
void printBoard(int B[3][3]){
        for(int i=0;i<3;i++){                                        
                                                                     

                for(int j=0;j<3;j++){
                        if(j!=2){                                    
                                                                     

                                printf("%d |", B[i][j]);             
                                                                     

                        }else{                                       
                                                                     

                                printf("%d\n", B[i][j]);             
                                                                     

                        }                                            
                                                                     

                }                                                    
                                                                     

       //printf("__ __ __\n");                                       
                                                                     
                                                                     

        }
        printf("\n");                                                
                                                                     

        printf("****************************************\n");        
                                                                     
                                                                     

}

//This is the evaluation function as discussed




//Method for First Player
void* player1(void* arg){
        printf("************ player 1 ************\n");

        struct threadInfo* info = (struct threadInfo*) arg;
        printBoard(info->arr);
        while(!info->gameOver){
                while(info->curPlayer != 1){
                        sleep(2);
                }
                if(info->curiter == 1){
                        info->arr[0][0] = 1;
                        info->curiter++;
                        info->curPlayer = 2;
                }
                if(info->curiter == 3){
                        info->arr[1][0] = 1;
                        info->curiter++;
                        info->curPlayer = 2;
                }
                if(info->curiter == 5){
                        info->arr[2][0] = 1;
                        info->curiter++;
                        printf("**********************************************Player One Wins!***********************************************\n");
                        return (0);
                }


        }
}

void* player2(void* arg){
        printf("************ player 2 ************\n");

        struct threadInfo* info = (struct threadInfo*) arg;
        while(!info->gameOver){
                while(info->curPlayer != 2){
                        sleep(3);
                }
                if(info->curiter == 2){
                        info->arr[0][1] = 2;
                        info->curiter++;
                        info->curPlayer = 1;
               }
                if(info->curiter == 4){
                        info->arr[0][2] = 2;
                        info->curiter++;
                        info->curPlayer = 1;
                }

        }
}

//methood to check if the player won
bool checkwin(char b[3][3]){
        if(

        (b[0][0] == b[0][1] && b[0][1] == b[0][2])  ||
        (b[1][0] == b[1][1] && b[1][1] == b[1][2])  ||
        (b[2][0] == b[2][1] && b[2][1] == b[2][2])  ||
        (b[0][0] == b[1][0] && b[1][0] == b[2][0])  ||
        (b[0][1] == b[1][1] && b[1][1] == b[2][1])  ||
        (b[0][2] == b[1][2] && b[1][2] == b[2][2])  ||
        (b[0][0] == b[1][1] && b[1][1] == b[2][2])  ||
        (b[0][2] == b[1][1] && b[1][1] == b[2][0])){
                return true;
        }else{
                return false;
        }

}



//main driver
int main(int argc, char** argv){
        struct threadInfo info;
        info.curPlayer = 1;
        info.curiter = 1;
        info.savedpos[0] = 0;
        info.savedpos[1] = 0;
        for(int i=0;i<3;i++){
                for(int j=0; j<3;j++){
                        info.arr[i][j]=0;
                }
        }
        info.gameOver = false;
        printBoard(info.arr);
        int arrtemp[3][18];
        //arrtemp[0] = {0,0,0,1,1,0,0,2,0,2};
        pthread_t id1;
        pthread_t id2;

//player one thread
        int err = pthread_create(&id1,NULL, player1, &info);
        if(err != 0){
                printf("Error creating pthread \n");
                return 1;
        }
        if(pthread_detach(id1) != 0 ){
                printf("Error detaching player1 \n");
                return 1;
        }
//player two thread
        err = pthread_create(&id2, NULL, player2, &info);
              if(err != 0){
                printf("Error creating pthread \n");
                return 1;
        }
        if(pthread_detach(id2) != 0 ){
                printf("Error detaching player2 \n");
                return 1;
        }
        int i =0;
        while(i<4){
                sleep(1);
                printBoard(info.arr);
                i++;
        }
        return 0;
        // do a while loop that loops until the game is over
}
