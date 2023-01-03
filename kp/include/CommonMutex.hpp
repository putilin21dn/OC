#include <errno.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
using namespace std;

struct CommonMutex{
    pthread_mutex_t *ptr;  
    int shm_fd;            
    char *name;            
    int created;      
};


CommonMutex shared_mutex_init(const char *name) {
    CommonMutex mutex = {NULL, 0, NULL, 0};
    errno = 0;
    mutex.shm_fd = shm_open(name, O_RDWR, 0660);
    if (errno == ENOENT) {
        mutex.shm_fd = shm_open(name, O_RDWR | O_CREAT, 0660);
        mutex.created = 1;
    }
    if (mutex.shm_fd == -1) {
        perror("error shm_open \n");

    }
    if (ftruncate(mutex.shm_fd, sizeof(pthread_mutex_t)) != 0) {
        perror("error ftruncate \n");

    }
    void *address = mmap(NULL, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE, MAP_SHARED, mutex.shm_fd, 0);
    if (address == MAP_FAILED) {
        perror("error create mmap \n");
        return mutex;
    }
    pthread_mutex_t *mutex_ptr = (pthread_mutex_t *)address;
    if (mutex.created) {
        pthread_mutexattr_t attr; // Deadlock to common shared data!
        if (pthread_mutexattr_init(&attr)) {
            cout << "An error while pthread_mutexattr_init has been detected!" << '\n';
            return mutex;
        }
        if (pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED)) { // PTHREAD_PROCESS_SHARED - may be operated on by any thread in any process that has access to it
            cout << "An error while pthread_mutexattr_setpshared has been detected!" << '\n';
            return mutex;
        } //pthread_mutexattr_setpsharedshall set the process-shared attribute in an initialized attributes object referenced by attr.
        if (pthread_mutex_init(mutex_ptr, &attr)) {
            cout << "An error while pthread_mutex_init has been detected!" << '\n';
            return mutex;
        }
    }
    mutex.ptr = mutex_ptr;
    mutex.name = (char *)malloc(NAME_MAX + 1);
    strcpy(mutex.name, name);
    return mutex;
}
int shared_mutex_destroy(CommonMutex mutex) {
    if ((errno = pthread_mutex_destroy(mutex.ptr))) {
        cout << "An error while destroying mutex has been detected!" << '\n';
        return -1;
    }
    if (munmap((void *)mutex.ptr, sizeof(pthread_mutex_t))) {
        cout << "An error while munmap has been detected!" << '\n';
        return -1;
    }
    mutex.ptr = NULL;
    if (close(mutex.shm_fd)) {
        cout << "An error while closing has been detected!" << '\n';
        return -1;
    }
    mutex.shm_fd = 0;
    if (shm_unlink(mutex.name)) {
        cout << "An error while shm_unlink has been detected!" << '\n';
        return -1;
    }
    free(mutex.name);
    return 0;
}