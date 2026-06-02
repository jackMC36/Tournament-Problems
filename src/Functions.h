#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <math.h>
#include <errno.h>
#include <time.h>
#include <stdbool.h>
#include <assert.h>

#include <sys/time.h>
#include <sys/stat.h>

#include<ilcplex/cplex.h>
#include "gurobi_c.h"




//IP Mproblem structure
typedef struct CPLEX_IP_problem
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
} CPLEX_IP_problem;

typedef struct GUROBI_MIP_solution{
        bool is_2X;
        double* sol;
        double objval;

} GUROBI_MIP_solution;

typedef struct dataSet 
{

	//Array of Integers from 0 to n-1 corresponding to all the vertices
	int*vertices;

	//Array of 1 and -1, corresponding to the direction of the arcs
	int**arcs;

	//Integer corresponding to the number of vertices
	int n;

	//CPLEX problem data
	CPLEX_IP_problem master;

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
	CPLEX_IP_problem master;

} hyperDataSet;


int read_instance_tournament(char* file_name,dataSet* dsptr);
void affichage_matrice_tournament(dataSet* dsptr);

hyperDataSet* create_3_uniform_hypergraph_partition_method(int n, char* file_name);
hyperDataSet* create_3_uniform_hypergraph(int n, char* file_name);
dataSet* transform_3_uniform_hypergraph_to_tournament(hyperDataSet* hdsptr, char* file_name);
dataSet* create_random_tournament(int n, char* file_name);
dataSet** create_all_tournaments_of_size_k(int k);
dataSet* create_greedy_2X_tournament(int n, char* file_name);
dataSet* create_2X_tournament(int n, char* file_name);
dataSet* create_2X_light_tournament_with_random(int n, char* file_name);
dataSet* create_2X_light_tournament_with_LP(int n, char* file_name);
dataSet** partition_new_2X_heavy_tournament(int n, char* file_name_1, char* file_name_2);
dataSet** partition_heavy_tournament(dataSet* initial, char* file_name_1, char* file_name_2);
dataSet* create_subgraph_with_removed_vertice(dataSet* initial, int v, char* file_name);
dataSet* create_P7();
dataSet* create_P7m();
dataSet* create_U5();
dataSet* create_T5();
dataSet* create_H2();
dataSet* create_W5();
dataSet* create_Delta_122();
dataSet* transform_2X_light_tournament_with_LP(int n, char* file_name, dataSet* dsptr);

int* greedy_coloring_tournament(dataSet* dsptr);

int solve_tournament_FVS_CPLEX(dataSet* dsptr);
int solve_tournament_2_coloring_CPLEX(dataSet* dsptr);
int solve_3_uniform_hypergraph_2_coloring_CPLEX(hyperDataSet* dsptr);
GUROBI_MIP_solution* solve_tournament_H2_GUROBI(dataSet* dsptr);
GUROBI_MIP_solution* solve_tournament_FVS_GUROBI_IP(dataSet* dsptr);
GUROBI_MIP_solution* solve_tournament_FVS_GUROBI_LP(dataSet* dsptr);
GUROBI_MIP_solution* solve_tournament_FVS_with_discrept_GUROBI(dataSet* dsptr);
GUROBI_MIP_solution* solve_tournament_2_coloring_GUROBI(dataSet* dsptr);

bool is_directed_triangle(dataSet* dsptr, int i, int j, int k);
bool is_heavy_arc(dataSet* dsptr, int u, int v);
bool is_H2(dataSet* dsptr, int u, int v, int a, int b, int c);
bool is_TU_matrix(int** matrix, int rows, int cols);
bool is_light_tournament(dataSet* dsptr);
bool is_P7m_tournament(dataSet* dsptr);
bool is_in(hyperEdge e1, hyperEdge* edges, int n);
bool are_equal(hyperEdge e1, hyperEdge e2);
bool is_integral_solution_FVS(dataSet* dsptr, GUROBI_MIP_solution* s);
bool is_isomorphic(dataSet* dsptr1, dataSet* dsptr2);
bool contains_subtournament(dataSet* host, dataSet* pattern);

void clean_dataSet(dataSet* dsptr);
void clean_hyperDataSet(hyperDataSet* dsptr);