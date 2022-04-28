#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h> /* For O_* constants */


#define SIZE_R 100

int main(){
        int fd = shm_open("test", O_RDWR | O_CREAT, 0600);
        if(fd<0){
                perror("shm_open()");
                return EXIT_FAILURE;
        }
        ftruncate(fd, SIZE_R);
        //lseek(fd, SIZE_R, SEEK_SET);
        char* buf = mmap(NULL, SIZE_R, PROT_READ|PROT_WRITE, MAP_SHARED, fd,0);

        if(buf == NULL){
                printf("error calling mmap: %s\n", strerror(errno));
                //exit(1);
        }
        if(write(fd,"",1)==-1){
                printf("error with write");
                exit(1);
        }
        int j =0;
        while(1){
                printf("Please select the integer of your choice:\n1.Read\n 2. Write\n 3.close communication\n");
                int num;
                scanf("%d", &num);
                if(num == 1){
                        //read
                        if(buf!=NULL){
                                int i;
                                printf("*******************************\n");
                                for(i=0;i<SIZE_R;i++){
                                        printf("%c",buf[i]);
                                }
                                printf("\n");
                                printf("*******************************\n");
                        }
                }else if(num == 2){
                        //write
                        j=0;
                        memset(buf, 0, SIZE_R);
                        char str[SIZE_R];
                        printf("send your message\n");
                        //scanf("%[^\n] ", str);
                        scanf("%s", str);
                        //fgets(str, SIZE_R, stdin);
                        strcpy(buf, str);
                }else if(num ==3){
                //close communication
                        if(munmap(buf, SIZE_R) ==-1){
                                printf("error unmapping file: %s\n", strerror(errno));
                                exit(1);
                        }

                        if(close(fd)==-1){
                                printf("error closing file: %s\n", strerror(errno));
                        }
                        if(shm_unlink("test")==-1){
                                printf("error with shm unlinking");  
                        }
                        return 0;
                }else{
                        //not an option
                        printf("this is not an option\n");
                }
        }
}
