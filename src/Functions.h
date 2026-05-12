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

	//Array of Integers from 0 to n-1 corresponding to all the vertices
	int*vertices;

	//Array of 1 and -1, corresponding to the direction of the arcs
	int**arcs;

	//Integer corresponding to the number of vertices
	int n;

	//CPLEX problem data
	IP_problem master;

} dataSet;

typedef struct hyperEdge
{
        int u;
        int v;
        int k;

} hyperEdge;

typedef struct  hyperDataSet
{
        //Array of Integers from 0 to n-1 corresponding to all the vertices
        int*vertices;


        hyperEdge* edges;

        //Integer corresponding to the number of vertices
        int n;
        //integer corresponding to the number of edges
        int m;

        //CPLEX problem data
	IP_problem master;

} hyperDataSet;



int read_instance_tournament(char* file_name,dataSet* dsptr);

hyperDataSet* create_3_uniform_hypergraph(int n, char* file_name);
dataSet* transform_3_uniform_hypergraph_to_tournament(hyperDataSet* hdsptr, char* file_name);
dataSet* create_2X_tournament(int n, char* file_name);
dataSet* create_2X_light_tournament(int n, char* file_name);
dataSet** partition_new_2X_heavy_tournament(int n, char* file_name_1, char* file_name_2);
dataSet** partition_2X_heavy_tournament(dataSet* initial, char* file_name_1, char* file_name_2);

bool is_directed_triangle(dataSet* dsptr, int i, int j, int k);
bool is_heavy_arc(dataSet* dsptr, int u, int v);
bool is_light_tournament(dataSet* dsptr);
bool is_in(hyperEdge e1, hyperEdge* edges, int n);
bool are_equal(hyperEdge e1, hyperEdge e2);

int solve_2X_tournament(dataSet* dsptr);
int solve_2X_3_uniform_hypergraph(hyperDataSet* dsptr);

void clean_dataSet(dataSet* dsptr);
void clean_hyperDataSet(hyperDataSet* dsptr);