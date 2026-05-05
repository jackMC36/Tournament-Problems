#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <math.h>
#include <errno.h>
#include <time.h>
#include <stdbool.h>
#include<ilcplex/cplex.h>
#include <assert.h>




//IP Mproblem structure
typedef struct IP_problem
{
        //Internal structures
        CPXENVptr env;
        CPXLPptr lp;
	//number of variables
	int nv;
	//number of constraints
	int nc;

        //Output solution
        double *x;
        //Costs array
        double *cost;
        //TYpe of the variables (binary or continuous)
        char *c_type;
        //Bounds over the variables
        double *up_bound;
        double *low_bound;
	//Names of the variables
	char** var_name;

        //Right hand section of the constraints
        double *rhs;
        //Sense of the constraints
        char *sense;
        //Left hand section for data constraints
        int *rmatbeg;
        int *rmatind;
        double *rmatval;
	int nz;
	//Names of the constraints 
	char** const_name;
        //Solution status8
        int solstat;
        double objval;
} IP_problem;

typedef struct dataSet 
{
	//Attributes of the instance

	//Array of Integers from 0 to n-1 corresponding to all the vertices (A union B)
	int*vertices;

	//Array of 1 and -1, corresponding to the direction of the arcs
	int**arcs;

	//Integer corresponding to the number of vertices
	int n;

	//CPLEX problem data
	IP_problem master;

} dataSet;

typedef struct arc
{
        int u;
        int v;
} arc;

dataSet* create_2X_tournament(int n, char* file_name);
dataSet* transform_2X_light_tournament(int n, char* file_name);
dataSet* create_2X_light_tournament(int n, char* file_name);
bool is_directed_triangle(dataSet* dsptr, int i, int j, int k);
bool is_heavy_arc(dataSet* dsptr, int u, int v);
int solve_2X_tournament(dataSet* dsptr);

