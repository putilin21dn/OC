#include <iostream>
#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int main(){
    int error = 0;
    int fd[2]; 
    int fd1[2];
    int fd2[2];

    pipe(fd); // pipe between child process
    pipe(fd1); // pipe between child1 process and parent process
    pipe(fd2); // pipe between child2 process and parent process

    
    int pid1 = fork();
    
    if(pid1==-1){
        printf("FORK ERROR!\n");
        return -1;
    }

    if(pid1==0){
            // CHILD
            
            //printf("%d %d %d %d %d %d\n", fd[0],fd[1], fd1[0],fd1[1], fd2[0],fd2[1]);
            
            error = dup2(fd1[0],1);
            if(error==-1){
                printf("Child: dup error\n");
                return -1;
            }
            std :: string str10 = std :: to_string(fd1[0]);
            std :: string str11 = std :: to_string(fd1[1]);
            std :: string str20 = std :: to_string(fd2[0]);
            std :: string str21 = std :: to_string(fd2[1]);
            std :: string str0 = std :: to_string(fd[0]);
            std :: string str1 = std :: to_string(fd[1]);

            execl("child.out", str10.c_str(),str11.c_str(),str20.c_str(),str21.c_str(), str0.c_str(), str1.c_str(), NULL);
            
        }

    else if(pid1>0){
        
            // PARENT
            printf("[%d] It's parent. Child id: %d\n", getpid(), pid1);
            char *in = (char*)malloc(sizeof(char)*2);
            in[0] = 0;
            char c;
            
            while((c=getchar())!=EOF){
                
                in[0] += 1;

                in[in[0]] = c;

                in = (char*)realloc(in, (in[0]+2)*sizeof(char));

            }
            in[in[0]] = '\0';        
            error = write(fd1[1], in, (in[0]+2)*sizeof(char));

            if (error==-1){
                printf("Not string! \n");
                return -1;
            }

            char *out = (char*)malloc(sizeof(char));

            error = read(fd2[0], &out[0],sizeof(char));

            if(error==-1){
                printf("WARNING! NOT READ TEXT!\n");
                return -1;
            }
            out = (char*)realloc(out, (out[0]+2)*sizeof(char));
            for(int i=1; i<out[0]+1; ++i){
                error = read(fd2[0],&out[i], sizeof(char));
                if(error==-1){
                    printf("WARNING! Not read liter!\n");
                    return -1;
                }
                else{
                    printf("%c",out[i]);
                }
            }

            printf("\n");
            
            close(fd1[1]);
            close(fd2[0]);     
            
            free(in);
            free(out);

            
        }
        

}
