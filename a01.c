/*********************************************************************
 * Second Degree Polynomial Calculator
 *
 * Author: Silas Agnew
 * Date: Feb 4, 2019
 * Inputs: Second degree polynomial coefficients (a,b,c), Start x,
 *  End x, and an X-Step size
 * Outputs: Table of the polynomial, its derivative, and the area
 *  underneath the curve from start to current x value.
 *
 * Description:
 *  This application, given the second degree polynomial
 *  coefficients, will output a table of values of the polynomial,
 *  its derivative, and its area underneath the curve.
 *  This also needs a start and stop point and a x-step size.
 ********************************************************************/
#include <stdio.h>

// Prints program usage
void usage(); 

// Returns the value of the derivative at x
float derivative(float, float, float);

// Returns the value of the definite integral from start x to x
float integral(float, float, float, float, float);

// Calculates all values for table
void calculate(float, float, float, float, float, float*, float*, float*);

int main()
{
    float a, b, c;
    float xstart, xend, dx;
    
    usage();

    printf("Enter the polynomial coefficients (a, b, and c)\n");
    scanf("%f %f %f", &a, &b, &c);
    printf("Enter the start, stop, and step interval\n");
    scanf("%f %f %f", &xstart, &xend, &dx);

    // Output
    printf("f(x) = %gx^2 + %gx + %g\n", a, b, c);
    printf("  x      f(x)    f'(x)    A    \n");
    printf("-------------------------------\n");
    
    float i;
    for (i = xstart; i <= xend; i += dx) // Make it print xend as value if ! dx|xf - xi
    {
        float f, df, area;
        calculate(a, b, c, i, xstart, &f, &df, &area);
        printf("%-6.3f  %-6.3f  %-6.3f  %-6.3f\n", i, f, df, area); 
    }
	
    return 0;
}

void usage()
{
    printf("How to use SECOND DEGREE POLYNOMIAL CALCULATOR.\n");
    printf("This application, given the second degree polynomial\n");
    printf("coefficients, will output a table of values of the polynomial,\n");
    printf("its derivative, and its area underneath the curve.\n");
    printf("This also needs a start and stop point and a x-step size.\n");
}

float derivative(float x, float a, float b)
{
    return 2 * a * x + b;
}

float integral(float xstart, float xend, float a, float b, float c)
{
    float start = (a * xstart * xstart * xstart / 3.0) +
        (b * xstart * xstart / 2.0) + (c * xstart);
    float end = (a * xend * xend * xend / 3.0) +
        (b * xend * xend / 2.0) + (c * xend);
    return end - start;
}

void calculate(float a, float b, float c, float x, float xi, float *f, float *df, float *area)
{
    *f = a * x * x + b * x + c;
    *area = integral(xi, x, a, b, c);
    *df = derivative(x, a, b);
}
