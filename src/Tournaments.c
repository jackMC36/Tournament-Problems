#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include "Functions.h"
#include<ilcplex/cplex.h>
#include "gurobi_c.h"

int main(int argc, char **argv)
{

	dataSet* data = create_2X_tournament(90,"data.csv");
	GUROBI_MIP_solution* error = solve_tournament_2_coloring_GUROBI(data);
	
	if (error == NULL) {
		fprintf(stderr, "Tournament solve failed.\n");
		return 1;
	}

	if(is_light_tournament(data)){
		printf("Data tournament is light.");
	}
	else{
		printf("The tournament is heavy.");
		dataSet** partition = partition_heavy_tournament(data,"partition1.csv","partition2.csv");
		if (partition == NULL || partition[1] == NULL || partition[2] == NULL) {
			fprintf(stderr, "Partitioning failed.\n");
			return 1;
		}

		if(is_light_tournament(partition[1])){
			printf("P1 is light.");
		}
		else{
			printf("P1 is heavy.");
		}

		if(is_light_tournament(partition[2])){
			printf("P2 is light.");
		}
		else{
			printf("P2 is heavy.");
		}
	}

	

	return 0;
}

