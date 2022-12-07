#include <stdio.h>
#include <dlfcn.h>

#ifdef SYSTEM
    #define PRINT_OS printf("Operation system: %s\n", SYSTEM)
#else
    #define PRINT_OS
#endif

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


const int N = 2;
const char* names[] = {"./libdyn1.so", "./libdyn2.so"};

int main()
{
    int n = 0;
    void* handle;
    float(*SinIntegral)(float,float,float); int(*Square)(float,float);
    CHECK_ERROR(handle = dlopen(names[n], RTLD_LAZY), "dlopen error");
    CHECK_ERROR(SinIntegral = dlsym(handle, "SinIntegral"), "dlsym error (SinIntegral)");
    CHECK_ERROR(Square = dlsym(handle, "Square"), "dlsym error (Square)");

    while(1)
    {
        int t;
        scanf("%d", &t);
        if (t == 0)
        {
            n = (n + 1) % N;
            if (dlclose(handle) != 0)
            {
                perror("dlclose error");
                return -1;
            };
            CHECK_ERROR(handle = dlopen(names[n], RTLD_LAZY), "dlopen error");
            CHECK_ERROR(SinIntegral = dlsym(handle, "SinIntegral"), "dlsym error (E)");
            CHECK_ERROR(Square = dlsym(handle, "Square"), "dlsym error (GCD)");
        }
        if (t == 1)
        {
            float A,B;
            scanf("%f %f", &A, &B);
            PRINT_OS;
            printf("Square: %d\n", (*Square)(A,B));
        }
        if (t == 2)
        {
            float A,B,e;
            scanf("%f %f %f", &A, &B, &e);
            PRINT_OS;
            printf("SinIntegral: %.10f\n", (*SinIntegral)(A,B,e));
        }
        if (t == -1)
            break;
    }
}