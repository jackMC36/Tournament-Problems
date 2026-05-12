#include "Functions.h"
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include<stdio.h>
#include <time.h>
#include <errno.h>
#include <sys/stat.h>
#include<ilcplex/cplex.h>



int read_instance_tournament(char* file_name,dataSet* dsptr)
{

	FILE* fin = fopen(file_name, "r");
	if (fin == NULL) return -1;

	int n;
	if (fscanf(fin, "%d\n", &n) != 1) return 1;

	dsptr->n = n;
	dsptr->vertices = calloc(n, sizeof(int));
	dsptr->arcs = malloc(n * sizeof(int*));
	for (int i = 0; i < n; i++)
		dsptr->arcs[i] = calloc(n, sizeof(int));

	char line[65536];
	for (int i = 0; i < n; i++) {
		if (fgets(line, sizeof(line), fin) == NULL) return 1;
		char* ptr = line;
		for (int j = 0; j < n; j++) {
			dsptr->arcs[i][j] = (int)strtol(ptr, &ptr, 10);
			if (*ptr == ',') ptr++;
		}
	}

	return 0;
}

hyperDataSet* create_3_uniform_hypergraph(int n, char* file_name)
{
	hyperDataSet* dsptr = malloc(sizeof(hyperDataSet));
	if(dsptr == NULL) return NULL;

	srand(time(NULL));
	float p;

	dsptr-> n = n;
	dsptr->vertices = calloc(n,sizeof(int));
	int max_edges = n * (n - 1) * (n - 2);
	dsptr->edges = malloc(max_edges * sizeof(hyperEdge));
	if (dsptr->vertices == NULL || dsptr->edges == NULL) {
		free(dsptr->vertices);
		free(dsptr->edges);
		free(dsptr);
		return NULL;
	}
	int edge_index = 0;
	hyperEdge e;

	for(int u = 0; u<n; u++){
		for(int v = 0; v<n; v++){
			for(int w = 0; w<n; w++){

				if((u==v)||(v==w)||(u==w)){
					continue;
				}

				e.u = u;
				e.v = v;
				e.k = w;

				if(is_in(e,dsptr->edges,edge_index)){
					continue;
				}

				p = (double)rand()/((double)RAND_MAX/1.0);

				if(p>1.0/(double)(n*n)){
					continue;
				}
				
				
				dsptr->edges[edge_index] = e;
				edge_index++;
			}
		}
	}

	dsptr->m = edge_index;

	FILE* fptr = fopen(file_name, "w");
	if (fptr == NULL) return NULL;

	fprintf(fptr, "%d,%d\n", n,dsptr->m);
	for(int i = 0; i<dsptr->m; i++){
		hyperEdge e = dsptr->edges[i];
		fprintf(fptr, "%d,%d,%d\n", e.u,e.v,e.k);
	}

	fclose(fptr);

	return dsptr;
}

dataSet* transform_3_uniform_hypergraph_to_tournament(hyperDataSet* hdsptr, char* file_name)
{
	if (hdsptr == NULL || hdsptr->edges == NULL) return NULL;

	int old_n = hdsptr->n;
	int m = hdsptr->m;
	hyperEdge* edges = hdsptr->edges;
	if (old_n <= 0 || m <= 0) return NULL;

	int T1_n = 3 * m;

	dataSet* T1_dsptr = malloc(sizeof(dataSet));
	T1_dsptr->n = T1_n;
	T1_dsptr->vertices = malloc(T1_n * sizeof(int));
	T1_dsptr->arcs = malloc(T1_n * sizeof(int*));
	int** T1_arcs = T1_dsptr->arcs;
	int* T1_vertices = T1_dsptr->vertices;
	for (int i = 0; i < T1_n; i++) {
		T1_arcs[i] = calloc(T1_n, sizeof(int));
	}

	for (int e = 0; e < m; e++) {
		int base = 3 * e;

		T1_vertices[base] = edges[e].u;
		T1_vertices[base + 1] = edges[e].v;
		T1_vertices[base + 2] = edges[e].k;

		T1_arcs[base][base + 1] = 1;
		T1_arcs[base + 1][base] = -1;
		T1_arcs[base + 1][base + 2] = 1;
		T1_arcs[base + 2][base + 1] = -1;
		T1_arcs[base + 2][base] = 1;
		T1_arcs[base][base + 2] = -1;

		for (int next_e = e + 1; next_e < m; next_e++) {
			int next_base = 3 * next_e;
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
					T1_arcs[base + j][next_base + k] = 1;
					T1_arcs[next_base + k][base + j] = -1;
				}
			}
		}
	}

	dataSet* T2_dsptr = malloc(sizeof(dataSet));
	T2_dsptr->n = 3;
	T2_dsptr->vertices = malloc(3 * sizeof(int));
	T2_dsptr->arcs = malloc(3 * sizeof(int*));
	int** T2_arcs = T2_dsptr->arcs;
	for (int i = 0; i < 3; i++) {
		T2_arcs[i] = calloc(3, sizeof(int));
		T2_dsptr->vertices[i] = i;
	}

	T2_arcs[0][1] = 1;
	T2_arcs[1][0] = -1;
	T2_arcs[1][2] = 1;
	T2_arcs[2][1] = -1;
	T2_arcs[2][0] = 1;
	T2_arcs[0][2] = -1;

	dataSet* T3_dsptr = malloc(sizeof(dataSet));
	T3_dsptr->n = old_n;
	T3_dsptr->vertices = malloc(old_n * sizeof(int));
	T3_dsptr->arcs = malloc(old_n * sizeof(int*));
	int** T3_arcs = T3_dsptr->arcs;
	for (int i = 0; i < old_n; i++) {
		T3_arcs[i] = calloc(old_n, sizeof(int));
		T3_dsptr->vertices[i] = i;
	}

	for (int i = 0; i < old_n; i++) {
		for (int j = i + 1; j < old_n; j++) {
			T3_arcs[i][j] = 1;
			T3_arcs[j][i] = -1;
		}
	}

	dataSet* T_dsptr = malloc(sizeof(dataSet));
	T_dsptr->n = T1_dsptr->n + T2_dsptr->n + T3_dsptr->n;
	int new_n = T_dsptr->n;
	T_dsptr->vertices = malloc(new_n * sizeof(int));
	T_dsptr->arcs = malloc(new_n * sizeof(int*));
	for (int i = 0; i < new_n; i++) {
		T_dsptr->vertices[i] = i;
		T_dsptr->arcs[i] = calloc(new_n, sizeof(int));
	}

	int t1_offset = 0;
	int t2_offset = T1_dsptr->n;
	int t3_offset = T1_dsptr->n + T2_dsptr->n;

	for (int i = 0; i < T1_dsptr->n; i++) {
		for (int j = 0; j < T1_dsptr->n; j++) {
			T_dsptr->arcs[t1_offset + i][t1_offset + j] = T1_arcs[i][j];
		}
	}

	for (int i = 0; i < T2_dsptr->n; i++) {
		for (int j = 0; j < T2_dsptr->n; j++) {
			T_dsptr->arcs[t2_offset + i][t2_offset + j] = T2_arcs[i][j];
		}
	}

	for (int i = 0; i < T3_dsptr->n; i++) {
		for (int j = 0; j < T3_dsptr->n; j++) {
			T_dsptr->arcs[t3_offset + i][t3_offset + j] = T3_arcs[i][j];
		}
	}

	for (int i = 0; i < T1_dsptr->n; i++) {
		for (int j = 0; j < T2_dsptr->n; j++) {
			T_dsptr->arcs[t1_offset + i][t2_offset + j] = 1;
			T_dsptr->arcs[t2_offset + j][t1_offset + i] = -1;
		}
	}

	for (int i = 0; i < T2_dsptr->n; i++) {
		for (int j = 0; j < T3_dsptr->n; j++) {
			T_dsptr->arcs[t2_offset + i][t3_offset + j] = 1;
			T_dsptr->arcs[t3_offset + j][t2_offset + i] = -1;
		}
	}

	for (int i = 0; i < T1_dsptr->n; i++) {
		for (int j = 0; j < T3_dsptr->n; j++) {
			int dst = t3_offset + j;
			if (T1_vertices[i] == T3_dsptr->vertices[j]) {
				T_dsptr->arcs[t1_offset + i][dst] = -1;
				T_dsptr->arcs[dst][t1_offset + i] = 1;
			} else {
				T_dsptr->arcs[t1_offset + i][dst] = 1;
				T_dsptr->arcs[dst][t1_offset + i] = -1;
			}
		}
	}

	clean_dataSet(T1_dsptr);
	clean_dataSet(T2_dsptr);
	clean_dataSet(T3_dsptr);

	FILE* fptr = fopen(file_name, "w");
	if (fptr == NULL) return NULL;

	fprintf(fptr, "%d\n", T_dsptr->n);
	for (int i = 0; i < T_dsptr->n; i++) {
		for (int j = 0; j < T_dsptr->n; j++) {
			fprintf(fptr, "%d", T_dsptr->arcs[i][j]);
			if (j != T_dsptr->n - 1) {
				fprintf(fptr, ",");
			}
		}
		fprintf(fptr, "\n");
	}
	fclose(fptr);

	return T_dsptr;
}

bool is_in(hyperEdge e1, hyperEdge* edges, int n)
{
	for(int i=0; i<n; i++){
		if(are_equal(e1,edges[i])){
			return true;
		}
	}
	return false;
}

bool are_equal(hyperEdge e1, hyperEdge e2)
{
	if(
		((e1.v==e2.v)&&(e1.u==e2.u)&&(e1.k==e2.k))||
		((e1.v==e2.v)&&(e1.u==e2.k)&&(e1.k==e2.u))||
		((e1.v==e2.u)&&(e1.u==e2.v)&&(e1.k==e2.k))||
		((e1.v==e2.u)&&(e1.u==e2.k)&&(e1.k==e2.v))||
		((e1.v==e2.k)&&(e1.u==e2.v)&&(e1.k==e2.u))||
		((e1.v==e2.k)&&(e1.u==e2.u)&&(e1.k==e2.v))	
	){
		return true;
	}

	return false;

}


dataSet* create_2X_tournament(int n, char* file_name)
{
	srand(time(NULL));
	FILE* fptr = fopen(file_name, "w");
	if (fptr == NULL) return NULL;
	
	dataSet* dsptr = malloc(sizeof(dataSet));
	if(dsptr == NULL) return NULL;
	dsptr->n = n;
	dsptr->vertices = calloc(n,sizeof(int));
	dsptr->arcs = malloc(n*sizeof(int*));
	for(int i = 0 ; i < n ; i++)
		dsptr->arcs[i] = calloc(n,sizeof(int));

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
	fclose(fptr);

	return dsptr;
}


dataSet** partition_new_2X_heavy_tournament(int n, char* file_name_1, char* file_name_2)
{
	dataSet** partition = malloc(3*sizeof(dataSet*));

	dataSet* initial = create_2X_tournament(n,file_name_1);
	if (initial == NULL) return NULL;

	partition[1] = malloc(sizeof(dataSet));
	if(partition[1] == NULL) return NULL;

	partition[2] = malloc(sizeof(dataSet));
	if(partition[2] == NULL) return NULL;

	int* heavy_vertex = calloc(n, sizeof(int));
	if (heavy_vertex == NULL) return NULL;

	for (int u = 0; u < n; u++) {
		for (int v = 0; v < n; v++) {
			if (u == v) continue;
			if (is_heavy_arc(initial, u, v)) {
				heavy_vertex[u] = 1;
				heavy_vertex[v] = 2;
			}
		}
	}

	int* new_n = calloc(3,sizeof(int));
	int* heavy_vertex_count = calloc(3,sizeof(int));

	int** old_to_new = malloc(3*sizeof(int*));
	int** new_to_old = malloc(3*sizeof(int*));

	for(int p=1; p<=2;p++){
		old_to_new[p] = malloc(n*sizeof(int));
		new_to_old[p] = malloc(n*sizeof(int));

		if (old_to_new[p] == NULL || new_to_old[p] == NULL) return NULL;

		for (int i = 0; i < n; i++) {
			old_to_new[p][i] = -1;
		}

		for(int i=0;i<n;i++){
			if (heavy_vertex[i]==p) {
			heavy_vertex_count[p]++;
			continue;
			}
			old_to_new[p][i] = new_n[p];
			new_to_old[p][new_n[p]] = i;
			new_n[p]++;
		}

		partition[p]->n = new_n[p];
		partition[p]->vertices = calloc(new_n[p],sizeof(int));
		partition[p]->arcs = malloc(new_n[p]*sizeof(int*));
		for(int i = 0 ; i < new_n[p] ; i++)
			partition[p]->arcs[i] = calloc(new_n[p],sizeof(int));

		for(int i = 0; i<new_n[p];i++){
			partition[p]->vertices[i] = i;
		}

		for(int i = 0; i<new_n[p]; i++){
			for(int j = 0; j<new_n[p]; j++){
				partition[p]->arcs[i][j] = initial->arcs[new_to_old[p][i]][new_to_old[p][j]];
			}
		}
		switch (p)
		{
		case 1:
			FILE* fptr1 = fopen(file_name_1, "w");

			if (fptr1 == NULL) {
			free(old_to_new[p]);
			free(new_to_old[p]);
			free(heavy_vertex);
			return NULL;
			}

			fprintf(fptr1, "%d\n", new_n[p]);
			for(int i = 0; i<new_n[p]; i++){
				for(int j = 0; j<new_n[p]; j++){
					fprintf(fptr1, "%d", partition[p]->arcs[i][j]);
					if(j!=new_n[p]-1){
						fprintf(fptr1,",");
					}
				}
				fprintf(fptr1,"\n");
			}
			fclose(fptr1);

			free(old_to_new[p]);
			free(new_to_old[p]);
			break;

		case 2:
			FILE* fptr2 = fopen(file_name_2, "w");

			if (fptr2 == NULL) {
			free(old_to_new[p]);
			free(new_to_old[p]);
			free(heavy_vertex);
			return NULL;
			}

			fprintf(fptr2, "%d\n", new_n[p]);
			for(int i = 0; i<new_n[p]; i++){
				for(int j = 0; j<new_n[p]; j++){
					fprintf(fptr2, "%d", partition[p]->arcs[i][j]);
					if(j!=new_n[p]-1){
						fprintf(fptr2,",");
					}
				}
				fprintf(fptr2,"\n");
			}
			fclose(fptr2);

			free(old_to_new[p]);
			free(new_to_old[p]);
		}
	}
	
	free(heavy_vertex);
		
	return partition;
}

dataSet** partition_2X_heavy_tournament(dataSet* initial, char* file_name_1, char* file_name_2)
{
	dataSet** partition = malloc(3*sizeof(dataSet*));

	if (initial == NULL) return NULL;

	partition[1] = malloc(sizeof(dataSet));
	if(partition[1] == NULL) return NULL;

	partition[2] = malloc(sizeof(dataSet));
	if(partition[2] == NULL) return NULL;

	int n = initial->n;

	int* heavy_vertex = calloc(n, sizeof(int));
	if (heavy_vertex == NULL) return NULL;

	for (int u = 0; u < n; u++) {
		for (int v = 0; v < n; v++) {
			if (u == v) continue;
			if (is_heavy_arc(initial, u, v)) {
				heavy_vertex[u] = 1;
				heavy_vertex[v] = 2;
			}
		}
	}

	int* new_n = calloc(3,sizeof(int));
	int* heavy_vertex_count = calloc(3,sizeof(int));

	int** old_to_new = malloc(3*sizeof(int*));
	int** new_to_old = malloc(3*sizeof(int*));

	for(int p=1; p<=2;p++){
		old_to_new[p] = malloc(n*sizeof(int));
		new_to_old[p] = malloc(n*sizeof(int));

		if (old_to_new[p] == NULL || new_to_old[p] == NULL) return NULL;

		for (int i = 0; i < n; i++) {
			old_to_new[p][i] = -1;
		}

		for(int i=0;i<n;i++){
			if (heavy_vertex[i]==p) {
			heavy_vertex_count[p]++;
			continue;
			}
			old_to_new[p][i] = new_n[p];
			new_to_old[p][new_n[p]] = i;
			new_n[p]++;
		}

		partition[p]->n = new_n[p];
		partition[p]->vertices = calloc(new_n[p],sizeof(int));
		partition[p]->arcs = malloc(new_n[p]*sizeof(int*));
		for(int i = 0 ; i < new_n[p] ; i++)
			partition[p]->arcs[i] = calloc(new_n[p],sizeof(int));

		for(int i = 0; i<new_n[p];i++){
			partition[p]->vertices[i] = i;
		}

		for(int i = 0; i<new_n[p]; i++){
			for(int j = 0; j<new_n[p]; j++){
				partition[p]->arcs[i][j] = initial->arcs[new_to_old[p][i]][new_to_old[p][j]];
			}
		}
		switch (p)
		{
		case 1:
			FILE* fptr1 = fopen(file_name_1, "w");

			if (fptr1 == NULL) {
			free(old_to_new[p]);
			free(new_to_old[p]);
			free(heavy_vertex);
			return NULL;
			}

			fprintf(fptr1, "%d\n", new_n[p]);
			for(int i = 0; i<new_n[p]; i++){
				for(int j = 0; j<new_n[p]; j++){
					fprintf(fptr1, "%d", partition[p]->arcs[i][j]);
					if(j!=new_n[p]-1){
						fprintf(fptr1,",");
					}
				}
				fprintf(fptr1,"\n");
			}
			fclose(fptr1);

			free(old_to_new[p]);
			free(new_to_old[p]);
			break;

		case 2:
			FILE* fptr2 = fopen(file_name_2, "w");

			if (fptr2 == NULL) {
			free(old_to_new[p]);
			free(new_to_old[p]);
			free(heavy_vertex);
			return NULL;
			}

			fprintf(fptr2, "%d\n", new_n[p]);
			for(int i = 0; i<new_n[p]; i++){
				for(int j = 0; j<new_n[p]; j++){
					fprintf(fptr2, "%d", partition[p]->arcs[i][j]);
					if(j!=new_n[p]-1){
						fprintf(fptr2,",");
					}
				}
				fprintf(fptr2,"\n");
			}
			fclose(fptr2);

			free(old_to_new[p]);
			free(new_to_old[p]);
		}
	}
	
	free(heavy_vertex);
		
	return partition;
}

bool is_directed_triangle(dataSet* dsptr, int i, int j, int k){
	int** arcs = dsptr->arcs;
	if((arcs[i][j]==arcs[j][k])&&(arcs[j][k]==arcs[k][i])&&(arcs[k][i]==arcs[i][j])){
		return true;
	}

	return false;

}

bool directed_triangles_count(dataSet* dsptr){
	int n = dsptr->n;
	int cpt = 0;
	for(int i=0;i<n;i++){
		for(int j=0; j<n; j++){
			for(int k=0; k<n;k++){
				cpt += is_directed_triangle(dsptr,i,j,k);
			}
		}
	}
	return cpt/3;
}

bool is_light_tournament(dataSet* dsptr){
	int n = dsptr->n;
	for(int u=0; u<n;u++){
		for(int v=0; v<n; v++){
			if(is_heavy_arc(dsptr,u,v)){
				return false;
			}
		}
	}
	
	return true;
}

bool is_heavy_arc(dataSet* dsptr, int u, int v){
	int n = dsptr->n;
	if (u == v) return false;
	for (int a = 0; a < n - 2; a++) {
		if (a == u || a == v) continue;
		for (int b = a + 1; b < n - 1; b++) {
			if (b == u || b == v) continue;
			for (int c = b + 1; c < n; c++) {
				if (c == u || c == v) continue;
				if (!is_directed_triangle(dsptr, a, b, c)) {
					continue;
				}
			
				if(is_directed_triangle(dsptr,u,v,a)&&is_directed_triangle(dsptr,u,v,b)&&is_directed_triangle(dsptr,u,v,c)){
					return true;
				}

			}

		}
	}
	return false;
}

dataSet* create_2X_light_tournament(int n, char* file_name){
	dataSet* dsptr = create_2X_tournament(n,file_name);
	while(!(is_light_tournament(dsptr))){
		free(dsptr);
		dsptr = create_2X_tournament(n,file_name);
	}

	return dsptr;
}


int solve_2X_tournament(dataSet* dsptr)
{
	if (dsptr == NULL) {
		fprintf(stderr, "solve_2X_tournament: NULL dataset\n");
		return 1;
	}

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
		ip_prob_ptr->c_type[j] = 'C';
		ip_prob_ptr->up_bound[j] = 1;
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
				if (!is_directed_triangle(dsptr, i, j, k)) {
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

	rval = CPXwriteprob (ip_prob_ptr->env, ip_prob_ptr->lp, "cplex_data/Tournament.lp", NULL);
	if(rval)
		fprintf(stderr,"CPXwriteprob returned errcode %d\n",rval);

	//We solve the model
	rval = CPXmipopt (ip_prob_ptr->env, ip_prob_ptr->lp);
	if(rval)
		fprintf(stderr,"CPXmipopt returned errcode %d\n",rval);

	rval = CPXsolwrite( ip_prob_ptr->env, ip_prob_ptr->lp, "cplex_data/Tournament.sol" );
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

int solve_2X_3_uniform_hypergraph(hyperDataSet* dsptr)
{
	if (dsptr == NULL) {
		fprintf(stderr, "solve_2X_3_uniform_hypergraph: NULL dataset\n");
		return 1;
	}

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

	ip_prob_ptr->lp = CPXcreateprob (ip_prob_ptr->env, &rval, "TP2");
	if(rval) fprintf(stderr,"ERROR WHILE CALLING CPXcreateprob\n");

	rval = CPXsetintparam (ip_prob_ptr->env, CPX_PARAM_DATACHECK, CPX_ON); 
	rval = CPXsetintparam (ip_prob_ptr->env, CPX_PARAM_SCRIND, CPX_ON);

	int n = dsptr->n;
	int m = dsptr->m;
	hyperEdge* edges = dsptr->edges;
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
		ip_prob_ptr->up_bound[j] = 1;
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

	//We fill what we can 
	ip_prob_ptr->rmatbeg[0] = 0;

	// xu + xv + xk >= 1
	for(int e=0; e<m; e++){

		ip_prob_ptr->rhs[0] = 1;
		ip_prob_ptr->sense[0] = 'G';

		snprintf(ip_prob_ptr->const_name[0],
				1024,
				"constraint_%d_%d_%d_lb",
				edges[e].u,
				edges[e].v,
				edges[e].k);

		ip_prob_ptr->rmatind[0] = edges[e].u;
		ip_prob_ptr->rmatval[0] = 1.0;
		ip_prob_ptr->rmatind[1] = edges[e].v;
		ip_prob_ptr->rmatval[1] = 1.0;
		ip_prob_ptr->rmatind[2] = edges[e].k;
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
	
	// xu + xv + xk <= 2
	for(int e=0; e<m; e++){

		ip_prob_ptr->rhs[0] = 2;
		ip_prob_ptr->sense[0] = 'L';

		snprintf(ip_prob_ptr->const_name[0],
				1024,
				"constraint_%d_%d_%d_ub",
				edges[e].u,
				edges[e].v,
				edges[e].k);

		ip_prob_ptr->rmatind[0] = edges[e].u;
		ip_prob_ptr->rmatval[0] = 1.0;
		ip_prob_ptr->rmatind[1] = edges[e].v;
		ip_prob_ptr->rmatval[1] = 1.0;
		ip_prob_ptr->rmatind[2] = edges[e].k;
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
	
	//We switch to minimization
	rval = CPXchgobjsen( ip_prob_ptr->env, ip_prob_ptr->lp, CPX_MIN );


	rval = CPXwriteprob (ip_prob_ptr->env, ip_prob_ptr->lp, "cplex_data/Hypergraph.lp", NULL);
	if(rval)
		fprintf(stderr,"CPXwriteprob returned errcode %d\n",rval);

	//We solve the model
	rval = CPXmipopt (ip_prob_ptr->env, ip_prob_ptr->lp);
	if(rval)
		fprintf(stderr,"CPXmipopt returned errcode %d\n",rval);

	rval = CPXsolwrite( ip_prob_ptr->env, ip_prob_ptr->lp, "cplex_data/Hypergraph.sol" );
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

void clean_dataSet(dataSet* dsptr){
	for(int i=0;i<dsptr->n;i++){
		free(dsptr->arcs[i]);
	}
	free(dsptr->arcs);
	free(dsptr->vertices);
	free(dsptr);
}

void clean_hyperDataSet(hyperDataSet* hdsptr){
	free(hdsptr->edges);
	free(hdsptr->vertices);
	free(hdsptr);
}