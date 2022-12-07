#include "../include/realisation.h"
#include <stdio.h>

#define check(VALUE, OKVAL, MSG) \
    if (VALUE != OKVAL) { \
        printf("%s", MSG); \
        return 1; \
    } \

int main(int argc, const char** argv) {
    int q;
    while (scanf("%d", &q) > 0) {
        if (q == 1) {
            float A,B,e;
            check(scanf("%f%f%f", &A, &B, &e), 3, "Error reading floats!\n");
            printf("SinIntegral(%f, %f, %f) = %f\n", A, B, e, SinIntegral(A,B,e));
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