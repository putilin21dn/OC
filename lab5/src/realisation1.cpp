#include "realisation.h"
#include <cmath>

// float f(float a){
//     return sinf(a);
// }

float SinIntegral(float A, float B, float e){
    float n = (B-A)/e;

    float ans = 0.0;
    for(int i=1; i<=n; ++i){
        ans += e*sin(A + e*(i-0.5));
    }
    return ans;
}

float Square(float A, float B) {
    return A * B;
}