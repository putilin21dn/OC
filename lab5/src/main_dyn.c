#include <stdio.h>
#include <dlfcn.h>

#define CHECK_ERROR(expr, message) \
    do \
    { \
        void* res = (expr); \
        if (res == NULL) \
        { \
            perror(message); \
            return -1; \
        } \
    } while (0)

#define check(VALUE, OKVAL, MSG) \
    if (VALUE != OKVAL) { \
        printf("%s", MSG); \
        return 1; \
    } \


const int N = 2;
const char* names[] = {"./libdyn1.so", "./libdyn2.so"};

int main()
{
    int n = 0;
    void* handle;
    float(*SinIntegral)(float,float,float); float(*Square)(float,float);
    handle = dlopen(names[n], RTLD_LAZY);
    CHECK_ERROR(handle = dlopen(names[n], RTLD_LAZY), "dlopen error");
    SinIntegral = dlsym(handle, "_Z11SinIntegralfff");
    CHECK_ERROR(SinIntegral = dlsym(handle, "_Z11SinIntegralfff"), "dlsym error (SinIntegral)");
    CHECK_ERROR(Square = dlsym(handle, "_Z6Squareff"), "dlsym error (Square)");

    while(1)
    {
        int t;
        scanf("%d", &t);
        if (t == 0)
        {
            n = (n + 1) % N;
            dlclose(handle);
            CHECK_ERROR(handle = dlopen(names[n], RTLD_LAZY), "dlopen error");
            CHECK_ERROR(SinIntegral = dlsym(handle, "_Z11SinIntegralfff"), "dlsym error (SinIntegral)");
            CHECK_ERROR(Square = dlsym(handle, "_Z6Squareff"), "dlsym error (Square)");
            printf("Swap library!\n");

        }
        if (t == 1)
        {
            float A,B,e;
            check(scanf("%f %f %f", &A, &B, &e), 3, "Error reading floats!\n");
            printf("SinIntegral: %.10f\n", (*SinIntegral)(A,B,e));
            
        }
        if (t == 2)
        {
            float A,B;
            check(scanf("%f %f", &A, &B), 2, "Error reading floats!\n");
            printf("Square: %f\n", (*Square)(A,B));   
        }
        if (t == -1)
            printf("End.\n");
            return 0;
    }
}