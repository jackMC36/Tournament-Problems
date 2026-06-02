#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include "Functions.h"
#include <ilcplex/cplex.h>
#include "gurobi_c.h"

int main(int argc, char **argv)
{
	int k = 5;
	dataSet** tournaments_on_k_vertices = create_all_tournaments_of_size_k(k);
	int edge_count = k * (k - 1) / 2;
	int list_count = 1 << edge_count;

	int non_isomorphic_count = 0;
	bool flag;
	dataSet** non_isomorphic_k_vertices_tournaments = malloc(list_count * sizeof(dataSet*));

	return 0;
}

