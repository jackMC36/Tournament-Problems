#include "Functions.h"
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include<stdio.h>
#include <time.h>
#include<ilcplex/cplex.h>

dataSet* create_instance(int n, char* file_name)
{
	srand(time(NULL));
	FILE* fptr = fopen(file_name, "w");
	
	dataSet* dsptr = malloc(sizeof(dataSet));
	if(dsptr == NULL) return NULL;
	dsptr->n = n;
	dsptr->vertices = calloc(n,sizeof(int));
	dsptr->arcs = malloc(n*sizeof(int*));
	for(int i = 0 ; i < n ; i++)
		dsptr->arcs[i] = calloc(n,sizeof(int));

	//Set A
	for(int i=0;i<n/2;i++){
		for(int j=0;j<n/2;j++){
			if(i==j){
				dsptr->arcs[i][j] = 0;
			}
			if(i<j){
				dsptr->arcs[i][j] = 1;
			}
			if(i>j){
				dsptr->arcs[i][j] = -1;
			}
		}
	}

	//Set B
	for(int i=n/2;i<n;i++){
		for(int j=n/2;j<n;j++){
			if(i==j){
				dsptr->arcs[i][j] = 0;
			}
			if(i<j){
				dsptr->arcs[i][j] = 1;
			}
			if(i>j){
				dsptr->arcs[i][j] = -1;
			}
		}
	}

	//Arcs between Set A and B
	for(int i=0;i<n/2;i++)
	{
		for(int j=n/2;j<n;j++)
		{
			int r = rand()%2;

			if(r==0)
			{
				r-=1;
			}

			dsptr->arcs[i][j] = r;
			dsptr->arcs[j][i] = -r;
		}
				

	}



	fprintf(fptr, "%d\n", n);
	for(int i = 0; i<n; i++){
		for(int j = 0; j<n; j++){
			fprintf(fptr, "%d", dsptr->arcs[i][j]);
			if(j!=n-1){
				fprintf(fptr,",");
			}
		}
		fprintf(fptr,"\n");
	}

	return dsptr;
}

bool triangle(dataSet* dsptr, int i, int j, int k){
	int** arcs = dsptr->arcs;
	if((arcs[i][j]==arcs[j][k])&&(arcs[j][k]==arcs[k][i])&&(arcs[k][i]==arcs[i][j])){
		return true;
	}

	return false;

}

int solve_2X_tournament(dataSet* dsptr)
{
	int rval = 0;

	IP_problem* ip_prob_ptr = &(dsptr->master);
	ip_prob_ptr->env = NULL;
	ip_prob_ptr->lp = NULL;
	ip_prob_ptr->env = CPXopenCPLEX (&rval);
	if(rval) fprintf(stderr,"ERROR WHILE CALLING CPXopenCPLEX\n");
	if ( ip_prob_ptr->env == NULL ) 
	{
		char  errmsg[1024];
		fprintf (stderr, "Could not open CPLEX environment.\n");
		CPXgeterrorstring (ip_prob_ptr->env, rval, errmsg);
		fprintf (stderr, "%s", errmsg);
		exit(0);	
	}

	//We create the MIP problem
	ip_prob_ptr->lp = CPXcreateprob (ip_prob_ptr->env, &rval, "TP2");
	if(rval) fprintf(stderr,"ERROR WHILE CALLING CPXcreateprob\n");

	rval = CPXsetintparam (ip_prob_ptr->env, CPX_PARAM_DATACHECK, CPX_ON); 
	rval = CPXsetintparam (ip_prob_ptr->env, CPX_PARAM_SCRIND, CPX_ON);

	int n = dsptr->n;
	int nv = n;

	//Memory
	ip_prob_ptr->nv = nv;
    ip_prob_ptr->x = (double*)malloc(sizeof(double)*nv);
    ip_prob_ptr->cost = (double*)malloc(sizeof(double)*nv);
    ip_prob_ptr->c_type = (char*)malloc(sizeof(char)*nv);
    ip_prob_ptr->up_bound = (double*)malloc(sizeof(double)*nv);
    ip_prob_ptr->low_bound = (double*)malloc(sizeof(double)*nv);
	ip_prob_ptr->var_name = (char**)malloc(sizeof(char*)*nv);


	//variables xi definition
	for(int j = 0 ; j < n ; j++)
	{
		ip_prob_ptr->x[j] = 0;
		ip_prob_ptr->cost[j] = 1;
		ip_prob_ptr->c_type[j] = 'I';
		ip_prob_ptr->up_bound[j] = 1000;
		ip_prob_ptr->low_bound[j] = 0;
		ip_prob_ptr->var_name[j] = (char*)malloc(sizeof(char)*1024);
	        snprintf(       ip_prob_ptr->var_name[j],
        	                1024,
                	        "x_%d",
                        	j);
	}


	rval = CPXnewcols( ip_prob_ptr->env, ip_prob_ptr->lp, 
			nv, 
			ip_prob_ptr->cost, 
			ip_prob_ptr->low_bound,
			ip_prob_ptr->up_bound,
			ip_prob_ptr->c_type,
			ip_prob_ptr->var_name);
	if(rval)
		fprintf(stderr,"CPXnewcols returned errcode %d\n",rval);



	//Constraints part
    ip_prob_ptr->rhs = (double*)malloc(sizeof(double));
    ip_prob_ptr->sense = (char*)malloc(sizeof(char));
    ip_prob_ptr->rmatbeg = (int*)malloc(sizeof(int));
	ip_prob_ptr->nz = n;


	ip_prob_ptr->rmatind = (int*)malloc(sizeof(int)*nv);
    ip_prob_ptr->rmatval = (double*)malloc(sizeof(double)*nv);
	ip_prob_ptr->const_name = (char**)malloc(sizeof(char*));
	ip_prob_ptr->const_name[0] = (char*)malloc(sizeof(char)*1024);

	ip_prob_ptr->rmatbeg[0] = 0;

	for (int i = 0; i < n - 2; i++) {
		for (int j = i + 1; j < n - 1; j++) {
			for (int k = j + 1; k < n; k++) {
				if (!triangle(dsptr, i, j, k)) {
					continue;
				}

				ip_prob_ptr->rhs[0] = 1;
				ip_prob_ptr->sense[0] = 'G';

				snprintf(ip_prob_ptr->const_name[0],
						 1024,
						 "constraint_%d_%d_%d",
						 i,
						 j,
						 k);

				ip_prob_ptr->rmatind[0] = i;
				ip_prob_ptr->rmatval[0] = 1.0;
				ip_prob_ptr->rmatind[1] = j;
				ip_prob_ptr->rmatval[1] = 1.0;
				ip_prob_ptr->rmatind[2] = k;
				ip_prob_ptr->rmatval[2] = 1.0;

				rval = CPXaddrows(ip_prob_ptr->env,
						  ip_prob_ptr->lp,
						  0,//No new column
						  1,//One new row
						  3,//Number of nonzero coefficients
						  ip_prob_ptr->rhs,
						  ip_prob_ptr->sense,
						  ip_prob_ptr->rmatbeg,
						  ip_prob_ptr->rmatind,
						  ip_prob_ptr->rmatval,
						  NULL,//No new column
						  ip_prob_ptr->const_name);
				if (rval)
					fprintf(stderr,"CPXaddrows returned errcode %d\n",rval);
			}
		}
	}

	//We switch to minimization
	rval = CPXchgobjsen( ip_prob_ptr->env, ip_prob_ptr->lp, CPX_MIN );


	//We write the problem for debugging purposes, can be commented afterwards
	rval = CPXwriteprob (ip_prob_ptr->env, ip_prob_ptr->lp, "Tournament.lp", NULL);
	if(rval)
		fprintf(stderr,"CPXwriteprob returned errcode %d\n",rval);

	//We solve the model
	rval = CPXmipopt (ip_prob_ptr->env, ip_prob_ptr->lp);
	if(rval)
		fprintf(stderr,"CPXmipopt returned errcode %d\n",rval);

	rval = CPXsolwrite( ip_prob_ptr->env, ip_prob_ptr->lp, "Tournament.sol" );
	if(rval)
		fprintf(stderr,"CPXsolwrite returned errcode %d\n",rval);

	//We get the objective value
	rval = CPXgetobjval( ip_prob_ptr->env, ip_prob_ptr->lp, &(ip_prob_ptr->objval) );
	if(rval)
		fprintf(stderr,"CPXgetobjval returned errcode %d\n",rval);

	//We get the best solution found 
	rval = CPXgetobjval( ip_prob_ptr->env, ip_prob_ptr->lp, &(ip_prob_ptr->objval) );
	rval = CPXgetx( ip_prob_ptr->env, ip_prob_ptr->lp, ip_prob_ptr->x, 0, nv-1 );
	if(rval)
		fprintf(stderr,"CPXgetx returned errcode %d\n",rval);



	return rval;
}
