#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "fcntl.h"
#include "sys/mman.h"
#include "string.h"
#include "errno.h"
#include "semaphore.h"

#define CHECK_ERROR(expr, message) \
    do \
    { \
        int res = (expr); \
        if (res == -1) \
        { \
            perror(message); \
            return -1; \
        } \
    } while (0)

#define UNLINK_ERROR(expr, message) \
    do \
    { \
        int res = (expr); \
        if (res == -1 && errno == EACCES) \
        { \
            perror(message); \
            return -1; \
        } \
    } while (0) 

const int MAX_LENGTH = 10000;
const int SIZE = MAX_LENGTH + sizeof(int);
const int zero = 0;

int main(){


    UNLINK_ERROR(unlink("file1"), "unlink error");  
    int file1 = open("file1", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

    if (file1 == -1 )
    {
        perror("open error");
        return -1;
    }
    
    CHECK_ERROR(lseek(file1, SIZE - 1, SEEK_SET), "lseek error");
    write(file1, &zero, 1);
    sem_t* sem1 = sem_open("!semaphore1", O_CREAT, S_IRUSR | S_IWUSR, 0);
    sem_t* sem2 = sem_open("!semaphore2", O_CREAT, S_IRUSR | S_IWUSR, 0);
    if (sem1 == SEM_FAILED || sem2 == SEM_FAILED )
    {
        perror("sem_open error");
        return -1;
    }
    int id = -1;

    CHECK_ERROR(id = fork(), "fork error");


    // child

     if (id == 0)
    {   
        void* in = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, file1, 0);
        if (in == MAP_FAILED )
        {
            perror("mmap error");
            return -1;
        }
        
        char* str = (char *)calloc(MAX_LENGTH, sizeof(char));
        char* out = (char *)malloc(sizeof(char)*MAX_LENGTH);

        if (str == NULL)
        {
            perror("calloc error");
            return -1;
        }
        
        CHECK_ERROR(sem_wait(sem1), "sem_wait error");
        memcpy(str, in, sizeof(char)*MAX_LENGTH);

        out[0]=0;
        for(int i=1; i<str[0]+1;++i){
            out[0]++;
            //printf("str[%d] = %c", i,str[i]);
            // out = (char*)realloc(out, (out[0]+2)*sizeof(char)); 
            if(str[i]>='A' & str[i]<='Z'){
                out[out[0]] = str[i] - 'A' + 'a';
                //printf("%c",out[out[0]]);
                //printf("%c\n",out[out[0]]);
                // out[out[0]] = '$';
            }
            else{
                out[out[0]] = str[i];
                //printf("%c\n",out[out[0]]);
            }
            if(str[i]==' '){
                out[out[0]] = '_';
                //printf("%c\n",out[out[0]]);
            }
            
           
        }

    out[0]++;
    memcpy(in, out, MAX_LENGTH*sizeof(char));
    CHECK_ERROR(sem_post(sem2), "sem_post error");

    CHECK_ERROR(munmap(in, SIZE), "munmap error");
    free(str);
    free(out);
        
    }

    // parent 
    if (id>0){

        void* out = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, file1, 0);
        if (out == MAP_FAILED )
        {
            perror("mmap error");
            return -1;
        }

        char *in = (char *)malloc(sizeof(char)*MAX_LENGTH);
        in[0] = 0;
        char c;
        while ((c = getchar()) != EOF) {
            in[0] += 1;
            in[in[0]] = c;
        }
        in[in[0]] = '\0'; 
        int value=10;
        
        memcpy(out, in, MAX_LENGTH*sizeof(char));
        CHECK_ERROR(sem_post(sem1), "sem_post error");
        CHECK_ERROR(sem_wait(sem2), "sem_wait error");

        
        char* str = (char *)calloc(MAX_LENGTH, sizeof(char));
        memcpy(str, out, sizeof(char)*MAX_LENGTH);

        for (int i=1; i<str[0]+1;++i){
            printf("%c",str[i]);
        }

        CHECK_ERROR(munmap(out, SIZE), "munmap error");
        sem_close(sem1);
        sem_close(sem2);
        printf("\n");
        free(in);
        free(str);
        
    }

    
    CHECK_ERROR(close(file1), "close error");
    unlink("file1");

}