#include <stdio.h>

void differentiate(float, float, float*, float*);
void integrate(float, float, float, float*, float*, float*);

int main()
{
    float a, b, c;
    float xstart, xend, dx;
    
    
    scanf("%f %f %f", &a, &b, &c);

    scanf("%f %f %f", &xstart, &xend, &dx);
	
    return 0;
}

void differentiate(float a, float b, float *m, float *intercept)
{
    *m = 2 * a;
    *intercept = b;
}

void integrate(float a, float b, float c, float *ao, float *bo, float *co)
{
    *ao = a / 3.0;
    *bo = b / 2.0;
    *co = c;
}
