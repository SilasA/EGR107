/*********************************************************************
 * TOLERANCE STACK ANALYSIS APPLICATION
 * Author: Silas Agnew
 * Date: 31 March 2019
 * Description:
 *
 ********************************************************************/

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define PART 1
#define GAP 2

/***********************************************************
 * Contains info from CSV input
 * if type is PART then all fields will have values
 * if type is GAP then only upper_tol and lower_tol will
 * have values
 **********************************************************/
struct dimension_t {
    int type;
    float nominal;
    float direction;
    float upper_tol;
    float lower_tol;
    bool fixed;
};
typedef struct dimension_t dimension_t;

dimension_t _dimensions[15];
int _size = 0;

dimension_t *_gapptr;

dimension_t create_part(int part, float nominal, float direction, float tolerance, bool fixed);
dimension_t create_gap(int gap, float upper_tol, float lower_tol);
void populate_dimensions(FILE *file);
void print_dimensions(bool gaps);

float calc_gap_mean();
float calc_gap_tol();
void calc_new_dim();
void calc_new_tol();

int main(int argc, char **argv)
{
    FILE *file = fopen("input.txt", "r");

    if (file == NULL)
    {
        printf("File not found");
        return 1;
    }

    populate_dimensions(file);
    //print_dimensions(true);
        
    float gapMean = calc_gap_mean();
    float gapTol = calc_gap_tol();
    float maxGap = gapMean + gapTol;
    float minGap = gapMean - gapTol;

    float gapUpperTol = _gapptr->upper_tol;
    float gapLowerTol = _gapptr->lower_tol;

    char *maxEquality;
    char *minEquality;

    // Determine equality
    if (gapUpperTol > maxGap)
        maxEquality = "Greater";
    else if (gapUpperTol < maxGap)
        maxEquality = "Less";
    else
        maxEquality = "Equals";

    if (gapLowerTol > minGap)
        minEquality = "Greater";
    else if (gapLowerTol < minGap)
        minEquality = "Less";
    else
        minEquality = "Equals";

    printf("Actual Gap Mean: %g\"\n", gapMean);
    printf("Actual Gap Tolerance: %g\"\n", gapTol);
    printf("The Maximum Gap (%g\") is (%s) than specified (%g\")\n", maxGap, maxEquality, gapUpperTol);
    printf("The Minimum Gap (%g\") is (%s) than the specified (%g\")\n\n", minGap, minEquality, gapLowerTol);

    printf("Recommended Adjustments to meet GAP %g, %g:\n", gapLowerTol, gapUpperTol);
    calc_new_dim();
    calc_new_tol();
    print_dimensions(false);
    return 0;
}

dimension_t create_part(int part, float nominal, float direction, float tolerance, bool fixed)
{
    dimension_t d;
    d.type = part;
    d.nominal = nominal;
    d.direction = direction;
    d.upper_tol = tolerance;
    d.lower_tol = -tolerance;
    d.fixed = fixed;
    return d;
}

dimension_t create_gap(int gap, float upper_tol, float lower_tol)
{
    dimension_t d;
    d.type = gap;
    d.upper_tol = upper_tol;
    d.lower_tol = lower_tol;
    return d;
}

void populate_dimensions(FILE *file)
{
    char *tokens[5];
    char *line = (char*)malloc(30);

    while (fscanf(file, "%s\n", line) != EOF)
    {
        char *token;
        token = strtok(line, ",");
        for (int i = 0; token != NULL; i++)
        {
            tokens[i] = token;
            token = strtok(NULL, ",");
        } 

        dimension_t d;
        if (strcmp(tokens[0], "PART") == 0)
        {
           d = create_part(PART,
                    strtof(tokens[1], NULL),
                    strtof(tokens[2], NULL),
                    strtof(tokens[3], NULL),
                    strcmp(tokens[4], "F") == 0);
            _dimensions[_size++] = d;
        }
        else
        {
            d = create_gap(GAP, 
                    strtof(tokens[1], NULL),
                    strtof(tokens[2], NULL));
            _dimensions[_size++] = d;
            _gapptr = &(_dimensions[_size - 1]);
        }
    }
    free(line);
}

void print_dimensions(bool gaps)
{
    for (int i = 0; i < _size; i++)
    {
        if (_dimensions[i].type == PART)
        {
            printf("PART--Nom: %6g Direct: %6g Tol: %6g Fixed: %s\n",
                    _dimensions[i].nominal, _dimensions[i].direction, _dimensions[i].upper_tol,
                    _dimensions[i].fixed ? "true" : "false");
        }
        else if (gaps)
        {
            printf("GAP---Min: %6g Max: %6g\n", _dimensions[i].lower_tol,
                    _dimensions[i].upper_tol);
        }
    }
}

float calc_gap_mean()
{
    float sum = 0;
    int count = 0;
    for (int i = 0; i < _size; i++)
    {
        if (_dimensions[i].type == PART)
        {
            sum += _dimensions[i].nominal * _dimensions[i].direction;
            count++;
        }
    }
    return sum;
}

float calc_gap_tol()
{
    float sum = 0;
    for (int i = 0; i < _size; i++)
    {
        if (_dimensions[i].type == PART)
        {
            sum += _dimensions[i].upper_tol;
        }
    }
    return sum;
};

void calc_new_dim()
{
    float scale;
    float sum;
    float fixed;
    float nom_gap = (_gapptr->upper_tol + _gapptr->lower_tol) / 2.0;
    
    for (int i = 0; i < _size; i++)
    {
        if (_dimensions[i].type != GAP)
        {
            if (_dimensions[i].fixed)
            {
                fixed = _dimensions[i].nominal;
                nom_gap = fixed - nom_gap;
            }
            else
                sum += _dimensions[i].nominal;
        }
    }
    
    scale = nom_gap / sum;
    
    for (int i = 0; i < _size; i++)
    {
        if (_dimensions[i].type != GAP)
        {
            if (!_dimensions[i].fixed)
            {
                _dimensions[i].nominal *= scale;
            }
        }
    }
}

void calc_new_tol()
{
    float scale;
    float sum = 0;
    float fixed;
    float nom_gap = (_gapptr->upper_tol + _gapptr->lower_tol) / 2.0;

    for (int i = 0; i < _size; i++)
    {
        if (_dimensions[i].type != GAP)
        {
            if (_dimensions[i].fixed)
            {
                fixed = _dimensions[i].upper_tol;
                nom_gap -= fixed;
            }
            else
                sum += _dimensions[i].upper_tol;
        }
    }

    scale = nom_gap / sum;

    for (int i = 0; i < _size; i++)
    {
        if (_dimensions[i].type != GAP)
        {
            if (!_dimensions[i].fixed)
            {
                _dimensions[i].upper_tol *= scale;
                _dimensions[i].lower_tol = -_dimensions[i].upper_tol;
            }
        }
    }
}

    
