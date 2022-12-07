#include "../include/realisation.h"
#include <math.h>

float f(float a){
    return sinf(a);
}

float SinIntegral(float A, float B, float e){
    float n = (B-A)/e;

    float ans = e* (f(B) - f(A))/2.0;

    for(int i=1; i<=n-1; ++i){
        ans += e*f(A + e*i);
    }
    return ans;
}

float Square(float A, float B) {
    return A/2.f * B;
}