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
	int k = 7;
	dataSet** ni_tournaments = create_all_tournaments_of_size_k(k);
	if (ni_tournaments == NULL) {
		fprintf(stderr, "Allocation failed\n");
		return 1;
	}

	int count = 0;
	while (ni_tournaments[count] != NULL) count++;
	printf("Non-isomorphic tournaments on %d vertices: %d\n", k, count);

	GUROBI_MIP_solution** solutions = malloc(count * sizeof(GUROBI_MIP_solution*));
	for(int i=0;i<count;i++){
		solutions[i] = solve_tournament_FVS_GUROBI_LP(ni_tournaments[i]);
	}



	for(int i=0;i<count;i++){
		if(!contains_not_TU_substructure(ni_tournaments[i])){
			printf("Tournament %i: is TU: %d",i, is_TU_matrix())
		}
	}

	return 0;
}

