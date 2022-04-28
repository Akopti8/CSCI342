/***************
: * 
 * gladiator.c 
 *
 * Eric McGregor 
 ****************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sem_helper.h"
#include <time.h>  
#include <stdio.h>
#include <errno.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h> 
#include "sem_helper.h"
#include <stdbool.h>

/***
 *
 *
 *      Beginning of helper file
 *
 *
 *
 * **/



void print_error(const char* str, int code)
{
        printf("%s: %s\n",str, strerror(code));
        exit(-1);
}

int getSemaphoreID(int size, int tokid)
{
        char *filepath = "/tmp";
        key_t key;
        int semid;

        if ((key = ftok(filepath, tokid)) == -1) {
                print_error("Can not create token", errno);
        }

        if ((semid = semget(key, size, 0666 | IPC_CREAT)) == -1) {   
                print_error("Can not create semaphore", errno);      
        }

        return semid;
}

void freeSemaphores(int semid)
{
        union semun {
                int val;
                struct semid_ds* buf;
                ushort* array;
        } arg;

        arg.val = 0;

        if ((semctl(semid, 0, IPC_RMID, arg)) == -1) {
                print_error("Error freeing semaphors", errno);       
        }
}

void setAllSemaphores(int semid, int size, int value)
{
        unsigned short arr[size];
        int i;
        for (i = 0; i< size;i++){
                arr[i] = value;
        }

        union semun {
                int val;
                struct semid_ds* buf;
                ushort* array;
        } arg;

        arg.array = arr;

        if ((semctl(semid, 0, SETALL, arg)) == -1) {
                print_error("Error setting all semaphore to 0", errno);
        }
}

void getAllSemaphores(int semid, int size, unsigned short* arr)      
{
        union semun {
                int val;
                struct semid_ds* buf;
                ushort* array;
        } arg;

        arg.array = arr;

        if ((semctl(semid, 0, GETALL, arg)) == -1) {
                print_error("Error getting all semaphores 0", errno);        }
}

void setSemaphore(int semid, int index, int value) {

        union semun {
                int val;
                struct semid_ds* buf;
                ushort* array;
        } arg;

        arg.val = value;

        if ((semctl(semid, index, SETVAL, arg)) == -1) {
                print_error("Error setting semaphore to 0", errno);  
        }
}

int getSemaphore(int semid, int index) {

        int semValue;
        if ((semValue = semctl(semid, index, GETVAL)) == -1) {       
                print_error("Error getting semaphore value", errno); 
        }

        return semValue;
}

void incrementSemaphore(int semid, int index) {
        struct sembuf op[1];
        int retval;

        op[0].sem_num = index;
        op[0].sem_op = 1;
        op[0].sem_flg = 0;

        if ((retval = semop(semid, op, 1)) == -1) {
                printf("Error incrementing semaphore", errno);       
        }
}

void decrementSemaphore(int semid, int index) {
        struct sembuf op[1];
        int retval;

        op[0].sem_num = index;
        op[0].sem_op = -1;
        op[0].sem_flg = 0;

        if ((retval = semop(semid, op, 1)) == -1) {
                printf("Error decrementing semaphore", errno);       
        }
}
int findEnemy(int id, int len, short *board){
        int index,i;
        for(i=0;i<= len; i++){
                if((int)board[i]==id){
                        return i;
                }
        }
}
int main(int argc, char* argv[])
{
        int semid;
        int gladiator_id;
        int arena_len;

        if (argc != 4) {
                printf("usage: %s semid gladiator_id arena_len\n", argv[0]);
                return 0;
        }

        semid = atoi(argv[1]);
        gladiator_id = atoi(argv[2]);
        arena_len = atoi(argv[3]);

        /* Battle */
        srand (time(NULL));
        int i, index;
        short board[arena_len*arena_len];
        getAllSemaphores(semid, arena_len*arena_len, board);
        for(i=0;i<= arena_len*arena_len; i++){
                if(board[i]==gladiator_id){
                        index=i;
                }
        }
        int index2=index-1;
        int temp;
        int cnt=0;
        int len = arena_len * arena_len;
        int opponent;
        if(gladiator_id == 1){
             opponent = 2;
         }else{
            opponent = 1;
        }
        while(1){
                //sleep(1);
                //gracious(board, gladiator_id);
                cnt++;
                int j=0;
                bool temp = true;
                if(cnt>=64){
                        //printf("***************************************************\n");
                        //for loosing
                        for(i=0;i< len; i++){
                                if(board[i]==gladiator_id){
                                        j=1;
                                }
                                printf("\n");
                        }
                        if(temp!=false){
                                printf("************************************************************\n");
                                printf("%d WONNNNNN\n", gladiator_id);
                                printf("***********************************************************\n");
                                kill(getpid(),SIGKILL);
                                return 0;
                        }
                }
                if(index < len && cnt%2 ==0){
                        setSemaphore(semid,index, gladiator_id);     
                        index++;
                }else if(index2>=0 && cnt%2 ==1){

                        setSemaphore(semid,index2, gladiator_id);    
                        index2--;

                }else if(index>len && index2<0){
                        //we will be checking where there are gaps from trying to fill the board
                        //and then restart our index to that value   
                        //I will do that with find enemy function    
                        int tempp;
                        for(i=0;i<= len; i++){
                                if((int)board[i]==opponent){
                                        tempp = i;
                                }
                        }
                        setSemaphore(semid,tempp, gladiator_id);     

                }
        }

        //setSemaphore(semid,rand()% (arena_len*arena_len) , gladiator_id);

        return 0;
}
