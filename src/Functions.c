#include "Functions.h"


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


void affichage_matrice_tournament(dataSet* dsptr)
{
	printf("\nTournament T on %d vertices",dsptr->n);
	printf("\nMatrice d'adjacence:\n");
	for(int i=0;i<dsptr->n;i++){
		printf("|");
		for(int j=0;j<dsptr->n;j++){
			printf("%d", dsptr->arcs[i][j]);
			if(j<dsptr->n-1){
				printf(",");
			}
		}
		printf("|\n");
	}

}

int** transform_Tournament_to_matrix(dataSet* dsptr)
{
	int m = directed_triangles_count(dsptr);
	int n = dsptr->n;

	int** matrix = malloc(m*sizeof(int*));
	for(int i=0;i<m;i++){
		matrix = calloc(n,sizeof(int));
	}

	int count = 0;

	for(int i=0;i<n-2;i++){
		for(int j=i+1;j<n-1;j++){
			for(int k=j+1;k<n;k++){
				if(is_directed_triangle(dsptr,i,j,k)){
					matrix[count][i] = 1;
					matrix[count][j] = 1;
					matrix[count][k] = 1;
					count +=1;
				}
			}
		}
	}

	return matrix;
}

void affichage_matrice_carre(int** M, int n)
{
	printf("\nTournament T on %d vertices",n);
	printf("\nMatrice d'adjacence:\n");
	for(int i=0;i<n;i++){
		printf("|");
		for(int j=0;j<n;j++){
			printf("%d", M[i][j]);
			if(j<n-1){
				printf(",");
			}
		}
		printf("|\n");
	}

}


int* int_vers_Binaire(int code){
    int* binaire = malloc(8 * sizeof(int));
    for (int i = 7; i >= 0; i--) {
        binaire[i] = code % 2;
        code = code / 2;
    }
    return binaire;
}

hyperDataSet* create_3_uniform_hypergraph_partition_method(int n, char* file_name)
{
	hyperDataSet* dsptr = malloc(sizeof(hyperDataSet));
	if(dsptr == NULL) return NULL;

	static int seeded_partition = 0;
	if (!seeded_partition) {
		srand((unsigned int)time(NULL));
		seeded_partition = 1;
	}
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

	for(int u = 0; u<2*n/3; u++){
		for(int v = 0; v<2*n/3; v++){
			for(int w = 0; w<2*n/3; w++){

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

				if(p>1.0/(double)(n)){
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

hyperDataSet* create_3_uniform_hypergraph(int n, char* file_name)
{
	hyperDataSet* dsptr = malloc(sizeof(hyperDataSet));
	if(dsptr == NULL) return NULL;

	static int seeded_hypergraph = 0;
	if (!seeded_hypergraph) {
		srand((unsigned int)time(NULL));
		seeded_hypergraph = 1;
	}
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

static void free_matrix_3x3(int** matrix)
{
	if (matrix == NULL) {
		return;
	}

	for (int i = 0; i < 3; i++) {
		free(matrix[i]);
	}
	free(matrix);
}

static bool isomorphic_3x3_matrices(int** lhs, int** rhs)
{
	static const int permutations[6][3] = {
		{0, 1, 2},
		{0, 2, 1},
		{1, 0, 2},
		{1, 2, 0},
		{2, 0, 1},
		{2, 1, 0}
	};

	if (lhs == NULL || rhs == NULL) {
		return false;
	}

	for (int rp = 0; rp < 6; rp++) {
		for (int cp = 0; cp < 6; cp++) {
			bool same = true;
			for (int i = 0; i < 3 && same; i++) {
				for (int j = 0; j < 3; j++) {
					if (lhs[i][j] != rhs[permutations[rp][i]][permutations[cp][j]]) {
						same = false;
						break;
					}
				}
			}
			if (same) {
				return true;
			}
		}
	}

	return false;
}

int*** generate_all_matrices_on_3X3(){
	int*** list = calloc(512, sizeof(int**));
	if (list == NULL) {
		return NULL;
	}

	int count = 0;

	for (int mask = 0; mask < 512; mask++) {
		int** matrix = malloc(3 * sizeof(int*));
		if (matrix == NULL) {
			for (int prev = 0; prev < count; prev++) {
				free_matrix_3x3(list[prev]);
			}
			free(list);
			return NULL;
		}

		for (int row = 0; row < 3; row++) {
			matrix[row] = calloc(3, sizeof(int));
			if (matrix[row] == NULL) {
				for (int r = 0; r < row; r++) {
					free(matrix[r]);
				}
				free(matrix);
				for (int prev = 0; prev < count; prev++) {
					free_matrix_3x3(list[prev]);
				}
				free(list);
				return NULL;
			}
		}

		for (int bit = 0; bit < 9; bit++) {
			int value = (mask >> bit) & 1;
			matrix[bit / 3][bit % 3] = value;
		}

		bool duplicate = false;
		for (int prev = 0; prev < count; prev++) {
			if (isomorphic_3x3_matrices(matrix, list[prev])) {
				duplicate = true;
				break;
			}
		}

		if (duplicate) {
			free_matrix_3x3(matrix);
			continue;
		}

		list[count++] = matrix;
	}

	list[count] = NULL;

	return list;
}

static void free_tournament(dataSet* t) {
	if (t == NULL) return;
	for (int i = 0; i < t->n; i++) free(t->arcs[i]);
	free(t->arcs);
	free(t->vertices);
	free(t);
}

static dataSet* alloc_tournament(int n) {
	dataSet* t = malloc(sizeof(dataSet));
	if (t == NULL) return NULL;
	t->n = n;
	t->vertices = calloc(n, sizeof(int));
	t->arcs = malloc(n * sizeof(int*));
	if (t->vertices == NULL || t->arcs == NULL) {
		free(t->vertices); free(t->arcs); free(t);
		return NULL;
	}
	for (int i = 0; i < n; i++) {
		t->vertices[i] = i;
		t->arcs[i] = calloc(n, sizeof(int));
		if (t->arcs[i] == NULL) {
			for (int r = 0; r < i; r++) free(t->arcs[r]);
			free(t->arcs); free(t->vertices); free(t);
			return NULL;
		}
	}
	return t;
}

dataSet** create_all_tournaments_of_size_k(int k) {
	if (k <= 0) {
		dataSet** empty = calloc(1, sizeof(dataSet*));
		return empty;
	}

	dataSet** cur = calloc(2, sizeof(dataSet*));
	if (cur == NULL) return NULL;
	cur[0] = alloc_tournament(1);
	if (cur[0] == NULL) { free(cur); return NULL; }
	int cur_count = 1;

	for (int sz = 2; sz <= k; sz++) {
		int n = sz - 1;                        
		int max_cand = cur_count * (1 << n);   
		dataSet** nxt = calloc(max_cand + 1, sizeof(dataSet*));
		if (nxt == NULL) {
			for (int t = 0; t < cur_count; t++) free_tournament(cur[t]);
			free(cur);
			return NULL;
		}
		int nxt_count = 0;

		for (int t = 0; t < cur_count; t++) {
			dataSet* base = cur[t];

			for (int mask = 0; mask < (1 << n); mask++) {
				dataSet* ext = alloc_tournament(sz);
				if (ext == NULL) goto oom;

				for (int i = 0; i < n; i++)
					for (int j = 0; j < n; j++)
						ext->arcs[i][j] = base->arcs[i][j];

				for (int i = 0; i < n; i++) {
					if ((mask >> i) & 1) {
						ext->arcs[n][i] = 1;
						ext->arcs[i][n] = -1;
					} else {
						ext->arcs[i][n] = 1;
						ext->arcs[n][i] = -1;
					}
				}

				bool dup = false;
				for (int j = 0; j < nxt_count; j++) {
					if (is_isomorphic(ext, nxt[j])) { dup = true; break; }
				}
				if (!dup) {
					nxt[nxt_count++] = ext;
				} else {
					free_tournament(ext);
				}
			}
		}

		for (int t = 0; t < cur_count; t++) free_tournament(cur[t]);
		free(cur);
		nxt[nxt_count] = NULL;
		cur = nxt;
		cur_count = nxt_count;
		printf("Non-isomorphic tournaments on %d vertices: %d\n", sz, cur_count);
		continue;

oom:
		for (int j = 0; j < nxt_count; j++) free_tournament(nxt[j]);
		free(nxt);
		for (int t2 = 0; t2 < cur_count; t2++) free_tournament(cur[t2]);
		free(cur);
		return NULL;
	}

	return cur;
}

dataSet* create_random_tournament(int n, char* file_name)
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

	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			int r = rand()%2;

			if(r==0)
			{
				r-=1;
			}
			if(i!=j){
			dsptr->arcs[i][j] = r;
			dsptr->arcs[j][i] = -r;
			}
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

dataSet* create_greedy_2X_tournament(int n, char* file_name)
{
	dataSet* data = create_random_tournament(n, file_name);
	if (data == NULL) {
		return NULL;
	}

	GUROBI_MIP_solution* sol_ptr = solve_tournament_2_coloring_GUROBI(data);
	if (sol_ptr == NULL) {
		return data;
	}

	if(sol_ptr->is_2X){
		return data;
	}

	int* greedy_coloring = NULL;
	while(!(sol_ptr->is_2X))
	{
		greedy_coloring = greedy_coloring_tournament(data);
		if (greedy_coloring == NULL) {
			return data;
		}

		int removed = 0;
		for(int i=0; i<data->n; i++){
			if(greedy_coloring[i]>1){
				data = create_subgraph_with_removed_vertice(data,i,file_name);
				removed = 1;
				break;
			}
		}
		free(greedy_coloring);

		if (!removed || data == NULL || data->n <= 0) {
			break;
		}

		sol_ptr = solve_tournament_2_coloring_GUROBI(data);
		if (sol_ptr == NULL) {
			break;
		}
	}
	
	return data;
}

dataSet* create_subgraph_with_removed_vertice(dataSet* initial, int v, char* file_name){

	if (initial == NULL || v < 0 || v >= initial->n || initial->n <= 1) {
		return NULL;
	}

	dataSet* new_data = malloc(sizeof(dataSet));
	if (new_data == NULL) {
		return NULL;
	}

	FILE* fptr = fopen(file_name, "w");
	if (fptr == NULL) {
		free(new_data);
		return NULL;
	}

	new_data->n = initial->n - 1;
	int n = new_data->n;
	new_data->vertices = malloc(n * sizeof(int));
	new_data->arcs = malloc(n*sizeof(int*));
	if (new_data->vertices == NULL || new_data->arcs == NULL) {
		fclose(fptr);
		free(new_data->vertices);
		free(new_data->arcs);
		free(new_data);
		return NULL;
	}

	for(int i=0;i<n;i++){
		new_data->arcs[i] = malloc(n*sizeof(int));
		if (new_data->arcs[i] == NULL) {
			for (int r = 0; r < i; r++) {
				free(new_data->arcs[r]);
			}
			fclose(fptr);
			free(new_data->vertices);
			free(new_data->arcs);
			free(new_data);
			return NULL;
		}
	}

	for(int i=0;i<n;i++){
		int old_i = (i < v) ? i : i + 1;
		new_data->vertices[i] = initial->vertices[old_i];
		for(int j=0; j<n; j++){
			int old_j = (j < v) ? j : j + 1;
			new_data->arcs[i][j] = initial->arcs[old_i][old_j];
		}
	}

	fprintf(fptr, "%d\n", n);
	for(int i = 0; i<n; i++){
		for(int j = 0; j<n; j++){
			fprintf(fptr, "%d", new_data->arcs[i][j]);
			if(j!=n-1){
				fprintf(fptr,",");
			}
		}
		fprintf(fptr,"\n");
	}
	fclose(fptr);


	clean_dataSet(initial);
	return new_data;

}

dataSet* Delta(dataSet* A, dataSet* B, dataSet* C)
{
	dataSet* dsptr = malloc(sizeof(dataSet));

	int n_A = A->n;
	int n_B = B->n;
	int n_C = C->n;

	dsptr->n = n_A+n_B+n_C;
	int n = dsptr->n;

	dsptr->arcs = malloc(n*sizeof(int*));
	for(int i=0;i<n;i++){
		dsptr->arcs[i] = malloc(n*sizeof(int));
	}

	for(int i=0;i<n_A;i++){
		for(int j=0;j<n_A;j++){
			dsptr->arcs[i][j] = A->arcs[i][j];
		}
	}

	for(int i=n_A;i<n_A+n_B;i++){
		for(int j=n_A;j<n_A+n_B;j++){
			dsptr->arcs[i][j] = B->arcs[i-n_A][j-n_A];
		}
	}

	for(int i=n_A+n_B;i<n;i++){
		for(int j=n_A+n_B;j<n;j++){
			dsptr->arcs[i][j] = C->arcs[i-n_A-n_B][j-n_A-n_B];
		}
	}

	for(int i=0;i<n_A;i++){
		for(int j=n_A;j<n_A+n_B;j++){
			dsptr->arcs[i][j] = 1;
			dsptr->arcs[j][i] = -1;
		}
	}

	for(int i=0;i<n_A;i++){
		for(int j=n_A+n_B;j<n;j++){
			dsptr->arcs[i][j] = -1;
			dsptr->arcs[j][i] = 1;
		}
	}

	for(int i=n_A;i<n_A+n_B;i++){
		for(int j=n_A+n_B;j<n;j++){
			dsptr->arcs[i][j] = 1;
			dsptr->arcs[j][i] = -1;
		}
	}
	return dsptr;
}

dataSet* create_vertex_tournament()
{
	dataSet* vertex = malloc(sizeof(dataSet));
	vertex->n = 1;

	vertex->arcs = malloc(sizeof(int*));
	vertex->arcs[0] = malloc(sizeof(int));

	vertex->arcs[0][0] = 0;
	return vertex;
}

dataSet* create_k_chromatic_tournament(int k)
{
	dataSet* vertex = malloc(sizeof(dataSet));
	vertex->n = 1;

	vertex->arcs = malloc(sizeof(int*));
	vertex->arcs[0] = malloc(sizeof(int));

	vertex->arcs[0][0] = 0;

	dataSet* dsptr = vertex;

	if(k==1){
		return vertex;
	}
	while(k!=1){
		dsptr = Delta(dsptr,dsptr,vertex);
		k-=1;
	}
	return dsptr;
}


static dataSet* create_subgraph_with_removed_vertices(dataSet* initial, int* removed, int removed_count, char* file_name)
{
	if (initial == NULL || removed == NULL || removed_count <= 0 || removed_count >= initial->n) {
		return NULL;
	}

	bool* removed_flag = calloc(initial->n, sizeof(bool));
	if (removed_flag == NULL) {
		return NULL;
	}

	for (int i = 0; i < removed_count; i++) {
		if (removed[i] < 0 || removed[i] >= initial->n) {
			free(removed_flag);
			return NULL;
		}
		removed_flag[removed[i]] = true;
	}

	dataSet* new_data = malloc(sizeof(dataSet));
	if (new_data == NULL) {
		free(removed_flag);
		return NULL;
	}

	FILE* fptr = fopen(file_name, "w");
	if (fptr == NULL) {
		free(removed_flag);
		free(new_data);
		return NULL;
	}

	new_data->n = initial->n - removed_count;
	int n = new_data->n;
	new_data->vertices = malloc(n * sizeof(int));
	new_data->arcs = malloc(n * sizeof(int*));
	if (new_data->vertices == NULL || new_data->arcs == NULL) {
		fclose(fptr);
		free(removed_flag);
		free(new_data->vertices);
		free(new_data->arcs);
		free(new_data);
		return NULL;
	}

	for (int i = 0; i < n; i++) {
		new_data->arcs[i] = malloc(n * sizeof(int));
		if (new_data->arcs[i] == NULL) {
			for (int r = 0; r < i; r++) {
				free(new_data->arcs[r]);
			}
			fclose(fptr);
			free(removed_flag);
			free(new_data->vertices);
			free(new_data->arcs);
			free(new_data);
			return NULL;
		}
	}

	int new_i = 0;
	for (int old_i = 0; old_i < initial->n; old_i++) {
		if (removed_flag[old_i]) {
			continue;
		}

		new_data->vertices[new_i] = initial->vertices[old_i];
		int new_j = 0;
		for (int old_j = 0; old_j < initial->n; old_j++) {
			if (removed_flag[old_j]) {
				continue;
			}
			new_data->arcs[new_i][new_j] = initial->arcs[old_i][old_j];
			new_j++;
		}
		new_i++;
	}

	fprintf(fptr, "%d\n", n);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			fprintf(fptr, "%d", new_data->arcs[i][j]);
			if (j != n - 1) {
				fprintf(fptr, ",");
			}
		}
		fprintf(fptr, "\n");
	}
	fclose(fptr);

	free(removed_flag);
	clean_dataSet(initial);
	return new_data;
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

dataSet** partition_heavy_tournament(dataSet* initial, char* file_name_1, char* file_name_2)
{
	dataSet** partition = malloc(3*sizeof(dataSet*));

	if (initial == NULL) return NULL;

	int n = initial->n;

	partition[1] = malloc(sizeof(dataSet));
	if(partition[1] == NULL) return NULL;

	partition[2] = malloc(sizeof(dataSet));
	if(partition[2] == NULL) return NULL;

	int* heavy_vertex = calloc(n, sizeof(int));
	if (heavy_vertex == NULL) return NULL;

	char** heavy_pair = malloc(n * sizeof(char*));
	if (heavy_pair == NULL) {
		free(heavy_vertex);
		return NULL;
	}
	for (int i = 0; i < n; i++) {
		heavy_pair[i] = calloc(n, sizeof(char));
	}

	for (int u = 0; u < n; u++) {
		for (int v = u + 1; v < n; v++) {
			if (is_heavy_arc(initial, u, v) || is_heavy_arc(initial, v, u)) {
				heavy_pair[u][v] = 1;
				heavy_pair[v][u] = 1;
			}
		}
	}

	int* queue = malloc(n * sizeof(int));

	int* has_heavy_neighbor = calloc(n, sizeof(int));
	if (has_heavy_neighbor == NULL) {
		for (int i = 0; i < n; i++) free(heavy_pair[i]);
		free(heavy_pair);
		free(queue);
		free(heavy_vertex);
		return NULL;
	}

	for (int start = 0; start < n; start++) {
		if (heavy_vertex[start] != 0) continue;

		int found_heavy = 0;
		for (int w = 0; w < n; w++) {
			if (start == w) continue;
			if (heavy_pair[start][w]) {
				found_heavy = 1;
				break;
			}
		}
		if (!found_heavy) continue;

		heavy_vertex[start] = 1;
		int head = 0;
		int tail = 0;
		queue[tail++] = start;

		while (head < tail) {
			int u = queue[head++];
			for (int v = 0; v < n; v++) {
				if (u == v) continue;

				if (!heavy_pair[u][v]) continue;

				has_heavy_neighbor[u] = 1;
				has_heavy_neighbor[v] = 1;

				int required_color = (heavy_vertex[u] == 1) ? 2 : 1;
				if (heavy_vertex[v] == 0) {
					heavy_vertex[v] = required_color;
					queue[tail++] = v;
				} else if (heavy_vertex[v] != required_color) {
					fprintf(stderr, "partition_heavy_tournament: heavy-arc graph is not bipartite; cannot split heavy vertices into 2 opposite sides.\n");
					for (int i = 0; i < n; i++) free(heavy_pair[i]);
					free(heavy_pair);
					free(has_heavy_neighbor);
					free(queue);
					free(heavy_vertex);
					return NULL;
				}
			}
		}
	}

	for (int i = 0; i < n; i++) {
		if (!has_heavy_neighbor[i]) {
			heavy_vertex[i] = 0;
		}
	}

	free(queue);
	free(has_heavy_neighbor);
	for (int i = 0; i < n; i++) free(heavy_pair[i]);
	free(heavy_pair);

	int* new_n = calloc(3,sizeof(int));
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
			int exclude_val = (p == 1) ? 2 : 1;
			if (heavy_vertex[i] == exclude_val) {
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
	if (arcs[i][j] == arcs[j][k] && arcs[j][k] == arcs[k][i] && arcs[i][j]!=0 ) {
		return true;
	}
	return false;

}

int directed_triangles_count(dataSet* dsptr){
	int n = dsptr->n;
	int cpt = 0;
	for(int i=0;i<n;i++){
		for(int j=0; j<n; j++){
			for(int k=0; k<n;k++){
				cpt += is_directed_triangle(dsptr,i,j,k);
			}
		}
	}
	return cpt/6;
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

dataSet* create_U5()
{
	int n = 5;
	dataSet* dsptr = malloc(sizeof(dataSet));
	if (!dsptr) return NULL;
	dsptr->n = n;
	dsptr->vertices = malloc(n * sizeof(int));
	if (!dsptr->vertices) {
		free(dsptr);
		return NULL;
	}

	for(int i=0; i<n;i++){
		dsptr->vertices[i] = i;
	}

	int** arcs = malloc(n*sizeof(int*));
	if (!arcs) {
		free(dsptr->vertices);
		free(dsptr);
		return NULL;
	}
	for(int i=0;i<n;i++){
		arcs[i] = calloc(n,sizeof(int));
		if (!arcs[i]) {
			for (int r = 0; r < i; r++) {
				free(arcs[r]);
			}
			free(arcs);
			free(dsptr->vertices);
			free(dsptr);
			return NULL;
		}
	}

	for(int i=1; i<n-1;i++){
		arcs[i][i+1] = 1;
		arcs[i+1][i] = -1;
	}

	arcs[1][0] = 1;
	arcs[0][1] = -1;

	arcs[4][0] = 1;
	arcs[0][4] = -1;

	arcs[0][2] = 1;
	arcs[2][0] = -1;

	arcs[2][4] = 1;
	arcs[4][2] = -1;

	arcs[4][1] = 1;
	arcs[1][4] = -1;

	arcs[1][3] = 1;
	arcs[3][1] = -1;

	arcs[3][0] = 1;
	arcs[0][3] = -1;

	dsptr->arcs = arcs;

	return dsptr;
}

dataSet* create_T5()
{
	int n = 5;
	dataSet* dsptr = malloc(sizeof(dataSet));
	if (!dsptr) return NULL;
	dsptr->n = n;
	dsptr->vertices = malloc(n * sizeof(int));
	if (!dsptr->vertices) {
		free(dsptr);
		return NULL;
	}

	for(int i=0; i<n;i++){
		dsptr->vertices[i] = i;
	}

	int** arcs = malloc(n*sizeof(int*));
	if (!arcs) {
		free(dsptr->vertices);
		free(dsptr);
		return NULL;
	}
	for(int i=0;i<n;i++){
		arcs[i] = calloc(n,sizeof(int));
		if (!arcs[i]) {
			for (int r = 0; r < i; r++) {
				free(arcs[r]);
			}
			free(arcs);
			free(dsptr->vertices);
			free(dsptr);
			return NULL;
		}
	}

	for(int i=0; i<n-1;i++){
		arcs[i][i+1] = 1;
		arcs[i+1][i] = -1;
	}
	arcs[4][0] = 1;
	arcs[0][4] = -1;

	arcs[0][2] = 1;
	arcs[2][0] = -1;

	arcs[2][4] = 1;
	arcs[4][2] = -1;

	arcs[4][1] = 1;
	arcs[1][4] = -1;

	arcs[1][3] = 1;
	arcs[3][1] = -1;

	arcs[3][0] = 1;
	arcs[0][3] = -1;

	dsptr->arcs = arcs;

	return dsptr;
}

dataSet* create_T7()
{
	int n = 7;
	dataSet* dsptr = malloc(sizeof(dataSet));
	if (!dsptr) return NULL;

	dsptr->n = n;
	dsptr->vertices = malloc(n * sizeof(int));
	if (!dsptr->vertices) {
		free(dsptr);
		return NULL;
	}

	for (int i = 0; i < n; i++) {
		dsptr->vertices[i] = i;
	}

	int** arcs = malloc(n * sizeof(int*));
	if (!arcs) {
		free(dsptr->vertices);
		free(dsptr);
		return NULL;
	}

	for (int i = 0; i < n; i++) {
		arcs[i] = calloc(n, sizeof(int));
		if (!arcs[i]) {
			for (int r = 0; r < i; r++) {
				free(arcs[r]);
			}
			free(arcs);
			free(dsptr->vertices);
			free(dsptr);
			return NULL;
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (i == j) {
				arcs[i][j] = 0;
				continue;
			}

			int d = (j - i + n) % n;
			if (d == 1 || d == 2 || d == 3) {
				arcs[i][j] = 1;
			} else {
				arcs[i][j] = -1;
			}
		}
	}

	dsptr->arcs = arcs;
	return dsptr;
}

dataSet* create_H2()
{
	int n = 5;
	dataSet* dsptr = malloc(sizeof(dataSet));
	if (!dsptr) return NULL;
	dsptr->n = n;
	dsptr->vertices = malloc(n * sizeof(int));
	if (!dsptr->vertices) {
		free(dsptr);
		return NULL;
	}

	for(int i=0; i<n;i++){
		dsptr->vertices[i] = i;
	}

	int** arcs = malloc(n*sizeof(int*));
	if (!arcs) {
		free(dsptr->vertices);
		free(dsptr);
		return NULL;
	}
	for(int i=0;i<n;i++){
		arcs[i] = calloc(n,sizeof(int));
		if (!arcs[i]) {
			for (int r = 0; r < i; r++) {
				free(arcs[r]);
			}
			free(arcs);
			free(dsptr->vertices);
			free(dsptr);
			return NULL;
		}
	}

	for(int i=0; i<n-1;i++){
		arcs[i][i+1] = 1;
		arcs[i+1][i] = -1;
	}
	arcs[4][0] = 1;
	arcs[0][4] = -1;

	arcs[0][2] = 1;
	arcs[2][0] = -1;

	arcs[2][4] = -1;
	arcs[4][2] = 1;

	arcs[4][1] = -1;
	arcs[1][4] = 1;

	arcs[1][3] = -1;
	arcs[3][1] = 1;

	arcs[3][0] = 1;
	arcs[0][3] = -1;

	dsptr->arcs = arcs;

	return dsptr;
}

dataSet* create_W5()
{
	int n = 5;
	dataSet* dsptr = malloc(sizeof(dataSet));
	if (!dsptr) return NULL;
	dsptr->n = n;
	dsptr->vertices = malloc(n * sizeof(int));
	if (!dsptr->vertices) {
		free(dsptr);
		return NULL;
	}

	for(int i=0; i<n;i++){
		dsptr->vertices[i] = i;
	}

	int** arcs = malloc(n*sizeof(int*));
	if (!arcs) {
		free(dsptr->vertices);
		free(dsptr);
		return NULL;
	}
	for(int i=0;i<n;i++){
		arcs[i] = calloc(n,sizeof(int));
		if (!arcs[i]) {
			for (int r = 0; r < i; r++) {
				free(arcs[r]);
			}
			free(arcs);
			free(dsptr->vertices);
			free(dsptr);
			return NULL;
		}
	}

	for(int i=0; i<n-1;i++){
		arcs[i][i+1] = -1;
		arcs[i+1][i] = 1;
	}
	arcs[4][0] = -1;
	arcs[0][4] = 1;

	arcs[0][2] = 1;
	arcs[2][0] = -1;

	arcs[2][4] = -1;
	arcs[4][2] = 1;

	arcs[4][1] = 1;
	arcs[1][4] = -1;

	arcs[1][3] = -1;
	arcs[3][1] = 1;

	arcs[3][0] = 1;
	arcs[0][3] = -1;

	dsptr->arcs = arcs;

	return dsptr;
}

dataSet* create_Delta_122()
{
	int n = 5;
	dataSet* dsptr = malloc(sizeof(dataSet));
	if (!dsptr) return NULL;
	dsptr->n = n;
	dsptr->vertices = malloc(n * sizeof(int));
	if (!dsptr->vertices) {
		free(dsptr);
		return NULL;
	}

	for(int i=0; i<n;i++){
		dsptr->vertices[i] = i;
	}

	int** arcs = malloc(n*sizeof(int*));
	if (!arcs) {
		free(dsptr->vertices);
		free(dsptr);
		return NULL;
	}
	for(int i=0;i<n;i++){
		arcs[i] = calloc(n,sizeof(int));
		if (!arcs[i]) {
			for (int r = 0; r < i; r++) {
				free(arcs[r]);
			}
			free(arcs);
			free(dsptr->vertices);
			free(dsptr);
			return NULL;
		}
	}

	for(int i=0; i<n-1;i++){
		arcs[i][i+1] = 1;
		arcs[i+1][i] = -1;
	}

	arcs[4][0] = 1;
	arcs[0][4] = -1;

	arcs[0][2] = -1;
	arcs[2][0] = 1;

	arcs[2][4] = 1;
	arcs[4][2] = -1;

	arcs[4][1] = 1;
	arcs[1][4] = -1;

	arcs[1][3] = 1;
	arcs[3][1] = -1;

	arcs[3][0] = 1;
	arcs[0][3] = -1;

	dsptr->arcs = arcs;

	return dsptr;
}

dataSet* create_P7()
{
	int n = 7;
	dataSet* dsptr = malloc(sizeof(dataSet));
	if (!dsptr) return NULL;

	dsptr->n = n;
	dsptr->vertices = malloc(n * sizeof(int));
	if (!dsptr->vertices) {
		free(dsptr);
		return NULL;
	}

	for (int i = 0; i < n; i++) {
		dsptr->vertices[i] = i;
	}

	int** arcs = malloc(n * sizeof(int*));
	if (!arcs) {
		free(dsptr->vertices);
		free(dsptr);
		return NULL;
	}

	for (int i = 0; i < n; i++) {
		arcs[i] = calloc(n, sizeof(int));
		if (!arcs[i]) {
			for (int r = 0; r < i; r++) {
				free(arcs[r]);
			}
			free(arcs);
			free(dsptr->vertices);
			free(dsptr);
			return NULL;
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (i == j) {
				arcs[i][j] = 0;
				continue;
			}

			int d = (j - i + n) % n;
			if (d == 1 || d == 2 || d == 4) {
				arcs[i][j] = 1;
			} else {
				arcs[i][j] = -1;
			}
		}
	}

	dsptr->arcs = arcs;
	return dsptr;
}

dataSet* create_P7m()
{
    int n = 6;
    dataSet* dsptr = malloc(sizeof(dataSet));
    if (!dsptr) return NULL;

    dsptr->n = n;
    dsptr->vertices = malloc(n * sizeof(int));
    dsptr->arcs = malloc(n * sizeof(int*));

    if (!dsptr->vertices || !dsptr->arcs) {
        free(dsptr->vertices);
        free(dsptr->arcs);
        free(dsptr);
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        dsptr->vertices[i] = i;
        dsptr->arcs[i] = malloc(n * sizeof(int));
        
        if (!dsptr->arcs[i]) {
            for (int r = 0; r < i; r++) free(dsptr->arcs[r]);
            free(dsptr->vertices);
            free(dsptr->arcs);
            free(dsptr);
            return NULL;
        }

        for (int j = 0; j < n; j++) {
            if (i == j) {
                dsptr->arcs[i][j] = 0;
            } else {
                int d = (j - i + 7) % 7;
                if (d == 1 || d == 2 || d == 4) {
                    dsptr->arcs[i][j] = 1;
                } else {
                    dsptr->arcs[i][j] = -1;
                }
            }
        }
    }

    return dsptr;
}

static int P7m_arc_value(int i, int j)
{
	if (i == j) return 0;
	int d = (j - i + 7) % 7;
	if (d == 1 || d == 2 || d == 4) {
		return 1;
	}
	return -1;
}

static bool P7m_match_backtrack(dataSet* dsptr, int depth, int* map, bool* used)
{
	if (depth == 6) {
		return true;
	}

	for (int v = 0; v < 6; v++) {
		if (used[v]) {
			continue;
		}

		bool ok = true;
		for (int i = 0; i < depth; i++) {
			int expected = P7m_arc_value(i, depth);
			int actual = dsptr->arcs[map[i]][v];
			if (actual != expected) {
				ok = false;
				break;
			}
		}

		if (!ok) {
			continue;
		}

		map[depth] = v;
		used[v] = true;
		if (P7m_match_backtrack(dsptr, depth + 1, map, used)) {
			return true;
		}
		used[v] = false;
	}

	return false;
}

bool is_P7m_tournament(dataSet* dsptr)
{
	if (dsptr == NULL || dsptr->n != 6 || dsptr->arcs == NULL) {
		return false;
	}

	int map[6];
	bool used[6] = {false, false, false, false, false, false};
	return P7m_match_backtrack(dsptr, 0, map, used);
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

bool is_H2(dataSet* dsptr, int u, int v, int a, int b, int c){
	if(is_directed_triangle(dsptr,u,v,a)&&is_directed_triangle(dsptr,u,v,b)&&is_directed_triangle(dsptr,u,v,c)&&is_directed_triangle(dsptr,a,b,c)){
		return true;
	}
	return false;
}

static bool isomorphic_backtrack(dataSet* dsptr1, dataSet* dsptr2, int* map1to2, bool* used2, int* outdeg1, int* outdeg2, int* tri1, int* tri2, int mapped_count)
{
	int n = dsptr1->n;
	if (mapped_count == n) {
		return true;
	}

	int best_u = -1;
	int best_count = n + 1;
	for (int u = 0; u < n; u++) {
		if (map1to2[u] != -1) {
			continue;
		}

		int count = 0;
		for (int v = 0; v < n; v++) {
			if (used2[v]) {
				continue;
			}
			if (outdeg1[u] != outdeg2[v] || tri1[u] != tri2[v]) {
				continue;
			}

			bool ok = true;
			for (int u2 = 0; u2 < n; u2++) {
				if (map1to2[u2] == -1) {
					continue;
				}
				int v2 = map1to2[u2];
				if (dsptr1->arcs[u][u2] != dsptr2->arcs[v][v2]) {
					ok = false;
					break;
				}
				if (dsptr1->arcs[u2][u] != dsptr2->arcs[v2][v]) {
					ok = false;
					break;
				}
			}
			if (ok) {
				count++;
			}
		}

		if (count == 0) {
			return false;
		}
		if (count < best_count) {
			best_count = count;
			best_u = u;
		}
	}

	if (best_u == -1) {
		return false;
	}

	for (int v = 0; v < n; v++) {
		if (used2[v]) {
			continue;
		}
		if (outdeg1[best_u] != outdeg2[v] || tri1[best_u] != tri2[v]) {
			continue;
		}

		bool ok = true;
		for (int u2 = 0; u2 < n; u2++) {
			if (map1to2[u2] == -1) {
				continue;
			}
			int v2 = map1to2[u2];
			if (dsptr1->arcs[best_u][u2] != dsptr2->arcs[v][v2]) {
				ok = false;
				break;
			}
			if (dsptr1->arcs[u2][best_u] != dsptr2->arcs[v2][v]) {
				ok = false;
				break;
			}
		}
		if (!ok) {
			continue;
		}

		map1to2[best_u] = v;
		used2[v] = true;
		if (isomorphic_backtrack(dsptr1, dsptr2, map1to2, used2, outdeg1, outdeg2, tri1, tri2, mapped_count + 1)) {
			return true;
		}
		used2[v] = false;
		map1to2[best_u] = -1;
	}

	return false;
}

static bool contains_subtournament_candidate_ok(dataSet* host, dataSet* pattern, int* p_to_h, int p, int h)
{
	int m = pattern->n;
	for (int p2 = 0; p2 < m; p2++) {
		if (p_to_h[p2] == -1) {
			continue;
		}
		int h2 = p_to_h[p2];
		if (pattern->arcs[p][p2] != host->arcs[h][h2]) {
			return false;
		}
		if (pattern->arcs[p2][p] != host->arcs[h2][h]) {
			return false;
		}
	}
	return true;
}

static bool contains_subtournament_backtrack(dataSet* host, dataSet* pattern, int* p_to_h, bool* used_h, int mapped_count)
{
	int n = host->n;
	int m = pattern->n;

	if (mapped_count == m) {
		return true;
	}

	int best_p = -1;
	int best_count = n + 1;

	for (int p = 0; p < m; p++) {
		if (p_to_h[p] != -1) {
			continue;
		}

		int count = 0;
		for (int h = 0; h < n; h++) {
			if (used_h[h]) {
				continue;
			}
			if (contains_subtournament_candidate_ok(host, pattern, p_to_h, p, h)) {
				count++;
			}
		}

		if (count == 0) {
			return false;
		}
		if (count < best_count) {
			best_count = count;
			best_p = p;
		}
	}

	if (best_p == -1) {
		return false;
	}

	for (int h = 0; h < n; h++) {
		if (used_h[h]) {
			continue;
		}
		if (!contains_subtournament_candidate_ok(host, pattern, p_to_h, best_p, h)) {
			continue;
		}

		p_to_h[best_p] = h;
		used_h[h] = true;

		if (contains_subtournament_backtrack(host, pattern, p_to_h, used_h, mapped_count + 1)) {
			return true;
		}

		used_h[h] = false;
		p_to_h[best_p] = -1;
	}

	return false;
}

bool contains_not_TU_substructure(dataSet* dsptr){
	int n = dsptr->n;
	for(int a=0;a<n;a++){
		for(int b=0; b<n; b++){
			for(int c=0;c<n;c++){
				for(int d=0; d<n;d++){
					for(int e=0;e<n;e++){
						if(is_directed_triangle(dsptr,a,b,d)&&is_directed_triangle(dsptr,b,c,e)&&is_directed_triangle(dsptr,a,c,e)){
							return true;
						}
					}
				}

			}
		}
	}
	return false;
}

bool is_isomorphic(dataSet* dsptr1, dataSet* dsptr2)
{
	if (dsptr1 == NULL || dsptr2 == NULL) {
		return false;
	}
	if (dsptr1->arcs == NULL || dsptr2->arcs == NULL) {
		return false;
	}
	if (dsptr1->n != dsptr2->n) {
		return false;
	}

	int n = dsptr1->n;
	if (n <= 1) {
		return true;
	}

	int* outdeg1 = calloc(n, sizeof(int));
	int* outdeg2 = calloc(n, sizeof(int));
	int* tri1 = calloc(n, sizeof(int));
	int* tri2 = calloc(n, sizeof(int));
	int* map1to2 = malloc(n * sizeof(int));
	bool* used2 = calloc(n, sizeof(bool));

	if (outdeg1 == NULL || outdeg2 == NULL || tri1 == NULL || tri2 == NULL || map1to2 == NULL || used2 == NULL) {
		free(outdeg1);
		free(outdeg2);
		free(tri1);
		free(tri2);
		free(map1to2);
		free(used2);
		return false;
	}

	for (int i = 0; i < n; i++) {
		map1to2[i] = -1;
		for (int j = 0; j < n; j++) {
			if (i == j) {
				continue;
			}
			if (dsptr1->arcs[i][j] == 1) {
				outdeg1[i]++;
			}
			if (dsptr2->arcs[i][j] == 1) {
				outdeg2[i]++;
			}
		}
	}

	for (int i = 0; i < n - 2; i++) {
		for (int j = i + 1; j < n - 1; j++) {
			for (int k = j + 1; k < n; k++) {
				if (is_directed_triangle(dsptr1, i, j, k)) {
					tri1[i]++;
					tri1[j]++;
					tri1[k]++;
				}
				if (is_directed_triangle(dsptr2, i, j, k)) {
					tri2[i]++;
					tri2[j]++;
					tri2[k]++;
				}
			}
		}
	}

	bool result = isomorphic_backtrack(dsptr1, dsptr2, map1to2, used2, outdeg1, outdeg2, tri1, tri2, 0);

	free(outdeg1);
	free(outdeg2);
	free(tri1);
	free(tri2);
	free(map1to2);
	free(used2);

	return result;
}

bool contains_subtournament(dataSet* host, dataSet* pattern)
{
	if (host == NULL || pattern == NULL) {
		return false;
	}
	if (host->arcs == NULL || pattern->arcs == NULL) {
		return false;
	}

	int n = host->n;
	int m = pattern->n;
	if (m < 0 || n < 0) {
		return false;
	}
	if (m == 0) {
		return true;
	}
	if (m > n) {
		return false;
	}

	int* p_to_h = malloc(m * sizeof(int));
	bool* used_h = calloc(n, sizeof(bool));
	if (p_to_h == NULL || used_h == NULL) {
		free(p_to_h);
		free(used_h);
		return false;
	}

	for (int i = 0; i < m; i++) {
		p_to_h[i] = -1;
	}

	bool result = contains_subtournament_backtrack(host, pattern, p_to_h, used_h, 0);

	free(p_to_h);
	free(used_h);
	return result;
}

int calculate_determinant_3x3_matrix(int** matrix){
	int d = 0;
	d+= 1*matrix[0][0]*(matrix[1][1]*matrix[2][2] - matrix[2][1]*matrix[1][2]);
	d+= -1*matrix[1][0]*(matrix[0][1]*matrix[2][2] - matrix[2][1]*matrix[0][2]);
	d+= 1*matrix[2][0]*(matrix[0][1]*matrix[1][2] - matrix[1][1]*matrix[0][2]);
	return d;
}

bool is_TU_matrix(int** matrix, int rows, int cols)
{
	if (matrix == NULL || rows < 0 || cols < 0) {
		return false;
	}

	if (rows < 3 || cols < 3) {
		return true;
	}

	int** sub = malloc(3 * sizeof(int*));
	if (sub == NULL) {
		return false;
	}
	for (int i = 0; i < 3; i++) {
		sub[i] = malloc(3 * sizeof(int));
		if (sub[i] == NULL) {
			for (int r = 0; r < i; r++) {
				free(sub[r]);
			}
			free(sub);
			return false;
		}
	}

	for (int r1 = 0; r1 < rows - 2; r1++) {
		for (int r2 = r1 + 1; r2 < rows - 1; r2++) {
			for (int r3 = r2 + 1; r3 < rows; r3++) {
				for (int c1 = 0; c1 < cols - 2; c1++) {
					for (int c2 = c1 + 1; c2 < cols - 1; c2++) {
						for (int c3 = c2 + 1; c3 < cols; c3++) {
							sub[0][0] = matrix[r1][c1];
							sub[0][1] = matrix[r1][c2];
							sub[0][2] = matrix[r1][c3];

							sub[1][0] = matrix[r2][c1];
							sub[1][1] = matrix[r2][c2];
							sub[1][2] = matrix[r2][c3];

							sub[2][0] = matrix[r3][c1];
							sub[2][1] = matrix[r3][c2];
							sub[2][2] = matrix[r3][c3];

							int d = calculate_determinant_3x3_matrix(sub);
							if (d != -1 && d != 0 && d != 1) {
								for (int r = 0; r < 3; r++) {
									free(sub[r]);
								}
								free(sub);
								return false;
							}
						}
					}
				}
			}
		}
	}

	for (int r = 0; r < 3; r++) {
		free(sub[r]);
	}
	free(sub);
	return true;
}

dataSet* create_2X_light_tournament_with_random(int n, char* file_name){
	dataSet* dsptr = create_2X_tournament(n,file_name);
	while(!(is_light_tournament(dsptr))){
		free(dsptr);
		dsptr = create_2X_tournament(n,file_name);
	}

	return dsptr;
}

dataSet* create_2X_light_tournament_with_LP(int n, char* file_name)
{
	dataSet* dsptr = create_2X_tournament(n,file_name);
	if (dsptr == NULL) {
		return NULL;
	}

	

	while (!is_light_tournament(dsptr) && dsptr->n > 1) {
		GUROBI_MIP_solution* solve = solve_tournament_H2_GUROBI(dsptr);
		if (solve == NULL || solve->sol == NULL) {
			break;
		}

		int old_n = dsptr->n;
		double max_value = solve->sol[0];
		for (int i = 1; i < old_n; i++) {
			if (solve->sol[i] > max_value) {
				max_value = solve->sol[i];
			}
		}

		if (max_value <= 1e-9) {
			break;
		}

		int* removed = malloc(old_n * sizeof(int));
		if (removed == NULL) {
			break;
		}

		int removed_count = 0;
		for (int i = 0; i < old_n; i++) {
			if (fabs(solve->sol[i] - max_value) < 1e-6) {
				removed[removed_count++] = i;
			}
		}

		if (removed_count == 0) {
			free(removed);
			break;
		}

		if (removed_count >= old_n) {
			removed[0] = old_n - 1;
			removed_count = 1;
		}

		dataSet* reduced = create_subgraph_with_removed_vertices(dsptr, removed, removed_count, file_name);
		free(removed);
		if (reduced == NULL) {
			break;
		}
		dsptr = reduced;
	}

	return dsptr;
}

dataSet* transform_2X_light_tournament_with_LP(int n, char* file_name, dataSet* dsptr)
{
	if (dsptr == NULL) {
		return NULL;
	}

	

	while (!is_light_tournament(dsptr) && dsptr->n > 1) {
		GUROBI_MIP_solution* solve = solve_tournament_H2_GUROBI(dsptr);
		if (solve == NULL || solve->sol == NULL) {
			break;
		}

		int old_n = dsptr->n;
		double max_value = solve->sol[0];
		for (int i = 1; i < old_n; i++) {
			if (solve->sol[i] > max_value) {
				max_value = solve->sol[i];
			}
		}

		if (max_value <= 1e-9) {
			break;
		}

		int* removed = malloc(old_n * sizeof(int));
		if (removed == NULL) {
			break;
		}

		int removed_count = 0;
		for (int i = 0; i < old_n; i++) {
			if (fabs(solve->sol[i] - max_value) < 1e-6) {
				removed[removed_count++] = i;
			}
		}

		if (removed_count == 0) {
			free(removed);
			break;
		}

		if (removed_count >= old_n) {
			removed[0] = old_n - 1;
			removed_count = 1;
		}

		dataSet* reduced = create_subgraph_with_removed_vertices(dsptr, removed, removed_count, file_name);
		free(removed);
		if (reduced == NULL) {
			break;
		}
		dsptr = reduced;
	}

	return dsptr;
}

int* greedy_coloring_tournament(dataSet* dsptr)
{
	int n = dsptr->n;
	int* coloring = malloc(n * sizeof(int));

	for (int i = 0; i < n; i++) {
		coloring[i] = -1;
	}

	for (int v = 0; v < n; v++) {
		int color = 0;
		while (1) {
			int feasible = 1;

			for (int u = 0; u < v && feasible; u++) {
				for (int w = u + 1; w < v; w++) {
					if (!is_directed_triangle(dsptr, u, w, v)) {
						continue;
					}
					if (coloring[u] == color && coloring[w] == color) {
						feasible = 0;
						break;
					}
				}
			}

			if (feasible) {
				coloring[v] = color;
				break;
			}
			color++;
		}
	}

	return coloring;
}

int solve_tournament_FVS_CPLEX(dataSet* dsptr)
{
	if (dsptr == NULL) {
		fprintf(stderr, "solve_2X_tournament: NULL dataset\n");
		return 1;
	}

	int rval = 0;

	CPLEX_IP_problem* ip_prob_ptr = &(dsptr->master);
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

	rval = CPXwriteprob (ip_prob_ptr->env, ip_prob_ptr->lp, "cplex_data/Tournament_FVS.lp", NULL);
	if(rval)
		fprintf(stderr,"CPXwriteprob returned errcode %d\n",rval);

	//We solve the model
	rval = CPXmipopt (ip_prob_ptr->env, ip_prob_ptr->lp);
	if(rval)
		fprintf(stderr,"CPXmipopt returned errcode %d\n",rval);

	rval = CPXsolwrite( ip_prob_ptr->env, ip_prob_ptr->lp, "cplex_data/Tournament_FVS.sol" );
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

int solve_tournament_2_coloring_CPLEX(dataSet* dsptr)
{
	if (dsptr == NULL) {
		fprintf(stderr, "solve_2X_tournament: NULL dataset\n");
		return 1;
	}

	int rval = 0;

	CPLEX_IP_problem* ip_prob_ptr = &(dsptr->master);
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
						 "constraint_%d_%d_%d_lb",
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

	for (int i = 0; i < n - 2; i++) {
		for (int j = i + 1; j < n - 1; j++) {
			for (int k = j + 1; k < n; k++) {
				if (!is_directed_triangle(dsptr, i, j, k)) {
					continue;
				}

				ip_prob_ptr->rhs[0] = 2;
				ip_prob_ptr->sense[0] = 'L';

				snprintf(ip_prob_ptr->const_name[0],
						 1024,
						 "constraint_%d_%d_%d_ub",
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

	rval = CPXwriteprob (ip_prob_ptr->env, ip_prob_ptr->lp, "cplex_data/Tournament_2_coloring.lp", NULL);
	if(rval)
		fprintf(stderr,"CPXwriteprob returned errcode %d\n",rval);

	//We solve the model
	rval = CPXmipopt (ip_prob_ptr->env, ip_prob_ptr->lp);
	if(rval)
		fprintf(stderr,"CPXmipopt returned errcode %d\n",rval);

	rval = CPXsolwrite( ip_prob_ptr->env, ip_prob_ptr->lp, "cplex_data/Tournament_2_coloring.sol" );
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

int solve_3_uniform_hypergraph_2_coloring_CPLEX(hyperDataSet* dsptr)
{
	if (dsptr == NULL) {
		fprintf(stderr, "solve_2X_3_uniform_hypergraph: NULL dataset\n");
		return 1;
	}

	int rval = 0;

	CPLEX_IP_problem* ip_prob_ptr = &(dsptr->master);
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


	rval = CPXwriteprob (ip_prob_ptr->env, ip_prob_ptr->lp, "cplex_data/Hypergraph_2_coloring.lp", NULL);
	if(rval)
		fprintf(stderr,"CPXwriteprob returned errcode %d\n",rval);

	//We solve the model
	rval = CPXmipopt (ip_prob_ptr->env, ip_prob_ptr->lp);
	if(rval)
		fprintf(stderr,"CPXmipopt returned errcode %d\n",rval);

	rval = CPXsolwrite( ip_prob_ptr->env, ip_prob_ptr->lp, "cplex_data/Hypergraph_2_coloring.sol" );
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

GUROBI_MIP_solution* solve_tournament_FVS_GUROBI_IP(dataSet* dsptr)
{
	int n = dsptr->n;

	GRBenv *env = NULL;
  	GRBmodel *model = NULL;
	GUROBI_MIP_solution* sol_ptr = malloc(sizeof(GUROBI_MIP_solution));
  	int error = 0;
  	double* sol = malloc(n*sizeof(double));
  	int* ind = malloc(3*sizeof(int));
  	double* val = malloc(3*sizeof(double));
  	double* obj = malloc(n*sizeof(double));
  	char* vtype = malloc(n*sizeof(int));
	char* const_name = (char*)malloc(sizeof(char)*1024);
  	int optimstatus;
  	double objval;

	sol_ptr->objval = objval;
	sol_ptr->sol = sol;
	sol_ptr->is_2X = NULL;



	 /* Create environment */
	error = GRBemptyenv(&env);
	if (error) goto QUIT;

	error = GRBsetstrparam(env, "LogFile", "tournament_FVS_GRB.log");
	if (error) goto QUIT;

	error = GRBstartenv(env);
	if (error) goto QUIT;

	/* Create an empty model */
	error = GRBnewmodel(env, &model, "tournament_FVS_GRB", 0, NULL, NULL, NULL, NULL, NULL);
	if (error) goto QUIT;

	/* Add variables */
	for(int i=0; i<n; i++){
		obj[i] = 1;
		vtype[i] = GRB_BINARY;
	}
	error = GRBaddvars(model, n, 0, NULL, NULL, NULL, obj, NULL, NULL, vtype,
						NULL);
	if (error) goto QUIT;

	/* Change objective sense to minimization */
	error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MINIMIZE);
	if (error) goto QUIT;

	/* Constraint */

	for (int i = 0; i < n - 2; i++) {
		for (int j = i + 1; j < n - 1; j++) {
			for (int k = j + 1; k < n; k++) {
				if (!is_directed_triangle(dsptr, i, j, k)) {
					continue;
				}
				snprintf(const_name,
						 1024,
						 "constraint_%d_%d_%d",
						 i,
						 j,
						 k);

				ind[0] = i; ind[1] = j; ind[2] = k;
				val[0] = 1; val[1] = 1; val[2] = 1;
				error = GRBaddconstr(model, 3, ind, val, GRB_GREATER_EQUAL, 1.0, const_name);
				if (error) goto QUIT;
			}
		}
	}

	/* Optimize model */
	error = GRBoptimize(model);
	if (error) goto QUIT;

	/* Write model to 'mip1.lp' */
	error = GRBwrite(model, "tournament_FVS_GRB.lp");
	if (error) goto QUIT;

	/* Capture solution information */
	error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
	if (error) goto QUIT;

	printf("\nOptimization complete\n");
	if (optimstatus == GRB_OPTIMAL) {
		error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
		if (error) goto QUIT;

		error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, n, sol);
		if (error) goto QUIT;

		printf("Optimal objective: %.4e\n", objval);
		for(int i=0; i<n; i++){
			printf("  x%d=%.6f,", i,  sol[i]);
		}
	} else if (optimstatus == GRB_INF_OR_UNBD || optimstatus == GRB_INFEASIBLE) {
		printf("Model is infeasible or unbounded\n");
	} else {
		printf("Optimization was stopped early\n");
	}

	QUIT:

	/* Error reporting */
	if (error) {
		printf("ERROR: %s\n", GRBgeterrormsg(env));
		exit(1);
	}

	/* Free model */
	GRBfreemodel(model);

	/* Free environment */
	GRBfreeenv(env);

	return sol_ptr;
}

GUROBI_MIP_solution* solve_tournament_FVS_GUROBI_LP(dataSet* dsptr)
{
	int n = dsptr->n;

	GRBenv *env = NULL;
  	GRBmodel *model = NULL;
	GUROBI_MIP_solution* sol_ptr = malloc(sizeof(GUROBI_MIP_solution));
  	int error = 0;
  	double* sol = malloc(n*sizeof(double));
  	int* ind = malloc(3*sizeof(int));
  	double* val = malloc(3*sizeof(double));
  	double* obj = malloc(n*sizeof(double));
  	char* vtype = malloc(n*sizeof(int));
	char* const_name = (char*)malloc(sizeof(char)*1024);
  	int optimstatus;
  	double objval;

	sol_ptr->objval = objval;
	sol_ptr->sol = sol;
	sol_ptr->is_2X = NULL;



	 /* Create environment */
	error = GRBemptyenv(&env);
	if (error) goto QUIT;

	error = GRBsetstrparam(env, "LogFile", "tournament_FVS_GRB.log");
	if (error) goto QUIT;

	error = GRBstartenv(env);
	if (error) goto QUIT;

	/* Create an empty model */
	error = GRBnewmodel(env, &model, "tournament_FVS_GRB", 0, NULL, NULL, NULL, NULL, NULL);
	if (error) goto QUIT;

	/* Add variables */
	for(int i=0; i<n; i++){
		obj[i] = 1;
		vtype[i] = GRB_CONTINUOUS;
	}
	error = GRBaddvars(model, n, 0, NULL, NULL, NULL, obj, NULL, NULL, vtype,
						NULL);
	if (error) goto QUIT;

	/* Change objective sense to minimization */
	error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MINIMIZE);
	if (error) goto QUIT;

	/* Constraint */

	for (int i = 0; i < n - 2; i++) {
		for (int j = i + 1; j < n - 1; j++) {
			for (int k = j + 1; k < n; k++) {
				if (!is_directed_triangle(dsptr, i, j, k)) {
					continue;
				}
				snprintf(const_name,
						 1024,
						 "constraint_%d_%d_%d",
						 i,
						 j,
						 k);

				ind[0] = i; ind[1] = j; ind[2] = k;
				val[0] = 1; val[1] = 1; val[2] = 1;
				error = GRBaddconstr(model, 3, ind, val, GRB_GREATER_EQUAL, 1.0, const_name);
				if (error) goto QUIT;
			}
		}
	}

	/* Optimize model */
	error = GRBoptimize(model);
	if (error) goto QUIT;

	/* Write model to 'mip1.lp' */
	error = GRBwrite(model, "tournament_FVS_GRB.lp");
	if (error) goto QUIT;

	/* Capture solution information */
	error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
	if (error) goto QUIT;

	printf("\nOptimization complete\n");
	if (optimstatus == GRB_OPTIMAL) {
		error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
		if (error) goto QUIT;

		error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, n, sol);
		if (error) goto QUIT;

		printf("Optimal objective: %.4e\n", objval);
		for(int i=0; i<n; i++){
			printf("  x%d=%.6f,", i,  sol[i]);
		}
	} else if (optimstatus == GRB_INF_OR_UNBD || optimstatus == GRB_INFEASIBLE) {
		printf("Model is infeasible or unbounded\n");
	} else {
		printf("Optimization was stopped early\n");
	}

	QUIT:

	/* Error reporting */
	if (error) {
		printf("ERROR: %s\n", GRBgeterrormsg(env));
		exit(1);
	}

	/* Free model */
	GRBfreemodel(model);

	/* Free environment */
	GRBfreeenv(env);

	return sol_ptr;
}

GUROBI_MIP_solution* solve_tournament_FVS_with_discrept_GUROBI(dataSet* dsptr)
{
	int n = dsptr->n;

	GRBenv *env = NULL;
  	GRBmodel *model = NULL;
	GUROBI_MIP_solution* sol_ptr = malloc(sizeof(GUROBI_MIP_solution));
  	int error = 0;
  	double* sol = malloc(n*sizeof(double));
  	int* ind = malloc(3*sizeof(int));
  	double* val = malloc(3*sizeof(double));
  	double* obj = malloc(n*sizeof(double));
  	char* vtype = malloc(n*sizeof(int));
	char* const_name = (char*)malloc(sizeof(char)*1024);
  	int optimstatus;
  	double objval;

	sol_ptr->objval = objval;
	sol_ptr->sol = sol;
	sol_ptr->is_2X = NULL;



	 /* Create environment */
	error = GRBemptyenv(&env);
	if (error) goto QUIT;

	error = GRBsetstrparam(env, "LogFile", "tournament_FVS_GRB.log");
	if (error) goto QUIT;

	error = GRBstartenv(env);
	if (error) goto QUIT;

	/* Create an empty model */
	error = GRBnewmodel(env, &model, "tournament_FVS_GRB", 0, NULL, NULL, NULL, NULL, NULL);
	if (error) goto QUIT;

	/* Add variables */
	for(int i=0; i<n; i++){
		double s = (double) (rand()%2);

		if(s==0.0)
		{
			s-=1.0;
		}

		double epsilon = 1.0/ (double) rand();
		

		
		obj[i] = 1.0 + s*epsilon;
		vtype[i] = GRB_CONTINUOUS;
	}
	error = GRBaddvars(model, n, 0, NULL, NULL, NULL, obj, NULL, NULL, vtype,
						NULL);
	if (error) goto QUIT;

	/* Change objective sense to minimization */
	error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MINIMIZE);
	if (error) goto QUIT;

	/* Constraint */

	for (int i = 0; i < n - 2; i++) {
		for (int j = i + 1; j < n - 1; j++) {
			for (int k = j + 1; k < n; k++) {
				if (!is_directed_triangle(dsptr, i, j, k)) {
					continue;
				}
				snprintf(const_name,
						 1024,
						 "constraint_%d_%d_%d",
						 i,
						 j,
						 k);

				ind[0] = i; ind[1] = j; ind[2] = k;
				val[0] = 1; val[1] = 1; val[2] = 1;
				error = GRBaddconstr(model, 3, ind, val, GRB_GREATER_EQUAL, 1.0, const_name);
				if (error) goto QUIT;
			}
		}
	}

	/* Optimize model */
	error = GRBoptimize(model);
	if (error) goto QUIT;

	/* Write model to 'mip1.lp' */
	error = GRBwrite(model, "tournament_FVS_GRB.lp");
	if (error) goto QUIT;

	/* Capture solution information */
	error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
	if (error) goto QUIT;

	printf("\nOptimization complete\n");
	if (optimstatus == GRB_OPTIMAL) {
		error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
		if (error) goto QUIT;

		error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, n, sol);
		if (error) goto QUIT;

		printf("Optimal objective: %.4e\n", objval);
		for(int i=0; i<n; i++){
			printf("  x%d=%.6f,", i,  sol[i]);
		}
	} else if (optimstatus == GRB_INF_OR_UNBD || optimstatus == GRB_INFEASIBLE) {
		printf("Model is infeasible or unbounded\n");
	} else {
		printf("Optimization was stopped early\n");
	}

	QUIT:

	/* Error reporting */
	if (error) {
		printf("ERROR: %s\n", GRBgeterrormsg(env));
		exit(1);
	}

	/* Free model */
	GRBfreemodel(model);

	/* Free environment */
	GRBfreeenv(env);

	return sol_ptr;
}

GUROBI_MIP_solution* solve_tournament_H2_GUROBI(dataSet* dsptr)
{
	int n = dsptr->n;

	GRBenv *env = NULL;
  	GRBmodel *model = NULL;
	GUROBI_MIP_solution* sol_ptr = malloc(sizeof(GUROBI_MIP_solution));
  	int error = 0;
  	double* sol = malloc(n*sizeof(double));
  	int* ind = malloc(5*sizeof(int));
  	double* val = malloc(5*sizeof(double));
  	double* obj = malloc(n*sizeof(double));
  	char* vtype = malloc(n*sizeof(int));
	char* const_name = (char*)malloc(sizeof(char)*1024);
  	int optimstatus;
  	double objval;

	sol_ptr->objval = objval;
	sol_ptr->sol = sol;
	sol_ptr->is_2X = NULL;



	 /* Create environment */
	error = GRBemptyenv(&env);
	if (error) goto QUIT;

	error = GRBsetstrparam(env, "LogFile", "tournament_H2_GRB.log");
	if (error) goto QUIT;

	error = GRBstartenv(env);
	if (error) goto QUIT;

	/* Create an empty model */
	error = GRBnewmodel(env, &model, "tournament_H2_GRB", 0, NULL, NULL, NULL, NULL, NULL);
	if (error) goto QUIT;

	/* Add variables */
	for(int i=0; i<n; i++){
		obj[i] = 1;
		vtype[i] = GRB_CONTINUOUS;
	}
	error = GRBaddvars(model, n, 0, NULL, NULL, NULL, obj, NULL, NULL, vtype,
						NULL);
	if (error) goto QUIT;

	/* Change objective sense to minimization */
	error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MINIMIZE);
	if (error) goto QUIT;

	/* Constraint */
	for (int u = 0; u < n; u++) {
		for (int v = 0; v < n; v++) {
			if (u == v) continue;
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
							snprintf(const_name,
							1024,
							"constraint_%d_%d_%d_%d_%d",
							u,v,a,b,c);

							ind[0] = u; ind[1] = v; ind[2] = a; ind[3] = b; ind[4] = c;
							val[0] = 1; val[1] = 1; val[2] = 1; val[3] = 1; val[4] = 1;
							error = GRBaddconstr(model, 5, ind, val, GRB_GREATER_EQUAL, 1.0, const_name);
							if (error) goto QUIT;
						}

					}

				}
			}	
		}
	}

	/* Optimize model */
	error = GRBoptimize(model);
	if (error) goto QUIT;

	/* Write model to 'mip1.lp' */
	error = GRBwrite(model, "tournament_H2_GRB.lp");
	if (error) goto QUIT;

	/* Capture solution information */
	error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
	if (error) goto QUIT;

	printf("\nOptimization complete\n");
	if (optimstatus == GRB_OPTIMAL) {
		error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
		if (error) goto QUIT;

		error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, n, sol);
		if (error) goto QUIT;

		printf("Optimal objective: %.4e\n", objval);
		for(int i=0; i<n; i++){
			printf("  x%d=%.0f,", i,  sol[i]);
		}
	} else if (optimstatus == GRB_INF_OR_UNBD || optimstatus == GRB_INFEASIBLE) {
		printf("Model is infeasible or unbounded\n");
	} else {
		printf("Optimization was stopped early\n");
	}

	QUIT:

	/* Error reporting */
	if (error) {
		printf("ERROR: %s\n", GRBgeterrormsg(env));
		exit(1);
	}

	/* Free model */
	GRBfreemodel(model);

	/* Free environment */
	GRBfreeenv(env);

	return sol_ptr;
}

GUROBI_MIP_solution* solve_tournament_2_coloring_GUROBI(dataSet* dsptr)
{
	printf("\nTournament arcs matrix (n=%d):\n", dsptr->n);
	for (int i = 0; i < dsptr->n; i++) {
		for (int j = 0; j < dsptr->n; j++) {
			printf("%2d ", dsptr->arcs[i][j]);
		}
		printf("\n");
	}

	printf("\nDirected triangles (3-cycles):\n");
	int triangle_count = 0;
	for (int i = 0; i < dsptr->n - 2; i++) {
		for (int j = i + 1; j < dsptr->n - 1; j++) {
			for (int k = j + 1; k < dsptr->n; k++) {
				if (is_directed_triangle(dsptr, i, j, k)) {
					printf("  (%d, %d, %d)\n", i, j, k);
					triangle_count++;
				}
			}
		}
	}
	printf("Total directed triangles: %d\n\n", triangle_count);
	int n = dsptr->n;

	GRBenv *env = NULL;
  	GRBmodel *model = NULL;
	GUROBI_MIP_solution* sol_ptr = malloc(sizeof(GUROBI_MIP_solution));
  	int error = 0;
  	double* sol = malloc(n*sizeof(double));
  	int* ind = malloc(3*sizeof(int));
  	double* val = malloc(3*sizeof(double));
  	double* obj = malloc(n*sizeof(double));
  	char* vtype = malloc(n*sizeof(int));
	char* const_name = (char*)malloc(sizeof(char)*1024);
  	int optimstatus;
  	double objval;

	sol_ptr->objval = objval;
	sol_ptr->sol = sol;
	sol_ptr->is_2X = true;

	error = GRBemptyenv(&env);
	if (error) goto QUIT;

	error = GRBsetstrparam(env, "LogFile", "tournament_FVS_GRB.log");
	if (error) goto QUIT;

	error = GRBstartenv(env);
	if (error) goto QUIT;

	/* Create an empty model */
	error = GRBnewmodel(env, &model, "tournament_2_coloring_GRB", 0, NULL, NULL, NULL, NULL, NULL);
	if (error) goto QUIT;

	/* Add variables */
	for(int i=0; i<n; i++){
		obj[i] = 1;
		vtype[i] = GRB_CONTINUOUS;
	}
	error = GRBaddvars(model, n, 0, NULL, NULL, NULL, obj, NULL, NULL, vtype,
						NULL);
	if (error) goto QUIT;

	/* Change objective sense to minimization */
	error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MINIMIZE);
	if (error) goto QUIT;

	/* Constraint */

	for (int i = 0; i < n - 2; i++) {
		for (int j = i + 1; j < n - 1; j++) {
			for (int k = j + 1; k < n; k++) {
				if (!is_directed_triangle(dsptr, i, j, k)) {
					continue;
				}
				snprintf(const_name,
						 1024,
						 "constraint_%d_%d_%d_lb",
						 i,
						 j,
						 k);

				ind[0] = i; ind[1] = j; ind[2] = k;
				val[0] = 1; val[1] = 1; val[2] = 1;
				error = GRBaddconstr(model, 3, ind, val, GRB_GREATER_EQUAL, 1.0, const_name);
				if (error) goto QUIT;
			}
		}
	}

	for (int i = 0; i < n - 2; i++) {
		for (int j = i + 1; j < n - 1; j++) {
			for (int k = j + 1; k < n; k++) {
				if (!is_directed_triangle(dsptr, i, j, k)) {
					continue;
				}
				snprintf(const_name,
						 1024,
						 "constraint_%d_%d_%d_ub",
						 i,
						 j,
						 k);

				ind[0] = i; ind[1] = j; ind[2] = k;
				val[0] = 1; val[1] = 1; val[2] = 1;
				error = GRBaddconstr(model, 3, ind, val, GRB_LESS_EQUAL, 2.0, const_name);
				if (error) goto QUIT;
			}
		}
	}

	/* Optimize model */
	error = GRBoptimize(model);
	if (error) goto QUIT;

	/* Write model to 'mip1.lp' */
	error = GRBwrite(model, "tournament_FVS_2_coloring.lp");
	if (error) goto QUIT;

	/* Capture solution information */
	error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
	if (error) goto QUIT;

	printf("\nOptimization complete\n");
	if (optimstatus == GRB_OPTIMAL) {
		error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
		if (error) goto QUIT;
		sol_ptr->objval = objval;

		error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, n, sol);
		if (error) goto QUIT;

		printf("Optimal objective: %.4e\n", objval);
		for(int i=0; i<n; i++){
			printf("  x%d=%.0f,", i,  sol[i]);
		}
	} else if (optimstatus == GRB_INF_OR_UNBD || optimstatus == GRB_INFEASIBLE) {
		printf("Model is infeasible- Impossible to 2-color \n");
		sol_ptr->is_2X = false;
	} else {
		printf("Optimization was stopped early\n");
	}

	QUIT:

	/* Error reporting */
	if (error) {
		printf("ERROR: %s\n", GRBgeterrormsg(env));
		exit(1);
	}

	/* Free model */
	GRBfreemodel(model);

	/* Free environment */
	GRBfreeenv(env);

	return sol_ptr;
}

bool is_integral_solution_FVS(dataSet* dsptr, GUROBI_MIP_solution* s)
{
	if (dsptr == NULL || s == NULL || s->sol == NULL) {
		return false;
	}

	for (int i = 0; i < dsptr->n; i++) {
		double x = s->sol[i];
		if (fabs(x) > 1e-6 && fabs(x - 1.0) > 1e-6) {
			return false;
		}
	}
	return true;
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