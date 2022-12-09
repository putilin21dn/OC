#include "../include/realisation.h"
#include <stdio.h>
#include <time.h>


#define check(VALUE, OKVAL, MSG) \
    if (VALUE != OKVAL) { \
        printf("%s", MSG); \
        return 1; \
    } \


#define PRINT_ID printf("ID : %s\n", C_ID)

#define PRINT_VERSION printf("VERSION : %s\n", C_VERSION)

#define PRINT_DATE printf("DATE : %s\n", DATE)




int main() {
    int q;
    PRINT_ID;
    PRINT_VERSION;
    PRINT_DATE;


    while (scanf("%d", &q) > 0) {
        if (q == 1) {
            float A,B,e;
            check(scanf("%f%f%f", &A, &B, &e), 3, "Error reading floats!\n");
            printf("SinIntegral(%f, %f, %f) = %.10f\n", A, B, e, SinIntegral(A,B,e));
        } else if (q == 2) {
            float A, B;
            check(scanf("%f %f", &A, &B), 2, "Error reading floats!\n");
            printf("Area is: %f\n", Square(A, B));
        } else {
            printf("End.\n");
            return 0;
        }
    }
}