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


#define PRINT_ID printf("ID : %s\n", C_ID)

#define PRINT_VERSION printf("VERSION : %s\n", C_VERSION)

#define PRINT_DATE printf("DATE : %s\n", DATE)



int main()
{

    PRINT_ID;
    PRINT_VERSION;
    PRINT_DATE;

    int n = 0;
    void* handle;
    float(*SinIntegral)(float,float,float); float(*Square)(float,float);
    handle = dlopen(names[n], RTLD_LAZY);
    CHECK_ERROR(handle = dlopen(names[n], RTLD_LAZY), "dlopen error");
    SinIntegral = dlsym(handle, "_Z11SinIntegralfff");
    CHECK_ERROR(SinIntegral = dlsym(handle, "_Z11SinIntegralfff"), "dlsym error (SinIntegral)");
    CHECK_ERROR(Square = dlsym(handle, "_Z6Squareff"), "dlsym error (Square)");

    int t;

    while(1)
    {
        
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
            printf("SinIntegral(%f, %f, %f) = %.10f\n", A, B, e, (*SinIntegral)(A,B,e));
        }

        if (t == 2)
        {
            float A,B;
            check(scanf("%f %f", &A, &B), 2, "Error reading floats!\n");
            printf("Area is: %f\n", (*Square)(A, B));   
        }
        if (t == -1){
            printf("End.\n");
            return 0;}
    }
}