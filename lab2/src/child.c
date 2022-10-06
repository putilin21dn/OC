#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"


int main(int argc, char *argv[])
{

    // for(int i=0; i<7; ++i){
    //     printf("%d %s\n",i,argv[i]);
    // }
    int error = 0;
    printf("[%d] It's child\n", getpid());
    int fd1[2];
    int fd2[2];
    int fd[2];
    //printf("%s %s",argv[0],argv[1]);
    fd1[0] = atoi(argv[0]);
    fd1[1] = atoi(argv[1]);
    fd2[0] = atoi(argv[2]);
    fd2[1] = atoi(argv[3]);
    fd[0] = atoi(argv[4]);
    fd[1] = atoi(argv[5]);

    char  *in, *out;

    int pid2 = fork();
    
    

    if(pid2<0){
        printf("Error! Create fork2!\n");
        return -1;
    }

    if (pid2==0) {

    // CHILD2

    in = (char*)malloc(sizeof(char)*2);
    error = read(fd[0], &in[0], sizeof(char));
    
    if(error==-1){
        printf("WARNING! NOT READ STRING!\n");
        return -1;
    }
    
    in = (char*)realloc(in, (in[0]+2)*sizeof(char));
    for(int i=1; i<in[0]+1; ++i){
        error = read(fd[0],&in[i], sizeof(char));
        if(error==-1){
            printf("WARNING! Not read liter!\n");
            return -1;
        } 
    }
    out = (char*)malloc(sizeof(char)*2);
    out[0] = 0;
    for(int i=1; i<in[0]+1;++i){
        if(in[i]==' '){
            out[0]+=1;
            out[out[0]] = '_';

        }
        else{
            out[0]+=1;
            out[out[0]] = in[i];
        }
        out = (char*)realloc(out, (out[0]+2)*sizeof(char));    
    }

    out[0]++;
    out[out[0]] = '\0';

    error = write(fd2[1], out, (out[0]+2)*sizeof(char));
    if(error==-1){
        printf("WARRING! Problem write!\n");
    }

    free(in);
    free(out);

    error = dup2(fd2[1],0);
    if(error==-1){
        perror("Child: dup error\n");
        return -1;
    }
    
    
    close(fd2[1]);
    close(fd[0]); 
    }
    else if (pid2>0){

        // CHILD1
        //printf("%d %d\n",fd1[0],fd1[1]);
        in = (char*)malloc(sizeof(char)*2);
        error = read(fd1[0], &in[0], sizeof(char));
        //printf("%d\n",in[0]);
        if(error==-1){
            printf("WARNING! NOT READ STRING!\n");
            return -1;
        }

        in = (char*)realloc(in, (in[0]+2)*sizeof(char));
        for(int i=1; i<in[0]+1; ++i){
            error = read(fd1[0],&in[i], sizeof(char));
            if(error==-1){
                printf("WARNING! Not read liter!\n");
                return -1;
            } 
        }


        out = (char*)malloc(sizeof(char)*2);
            out[0] = 0;
            for(int i=1; i<in[0]+1;++i){
                if(in[i]>='A' && in[i]<='Z'){
                    out[0]+=1;
                    out[out[0]] = in[i] + 32;

                }
                else{
                    out[0]+=1;
                    out[out[0]] = in[i];
                }
                out = (char*)realloc(out, (out[0]+2)*sizeof(char));    
            }

            out[0]++;
            out[out[0]] = '\0';

        write(fd[1],out,sizeof(char)*(out[0]+2)); 
        
        // for(int i=1; i<out[0]+1;++i){
        //     printf("%c",out[i]);
        // }
        
        free(in);
        free(out);

        

        error = dup2(fd[1],0);
        if(error==-1){
            perror("Child: dup error\n");
            return -1;
        }

        error = dup2(fd[0],1);
        if(error==-1){
            perror("Child: dup error\n");
            return -1;
        }
        close(fd1[0]);
        close(fd[1]);
    }
}