#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include "Functions.h"
#include<ilcplex/cplex.h>


int main(int argc, char **argv)
{
	int rval =0;	
	//File instance name
	//-F option
    //    char instance_file[1024];
    //    snprintf(       instance_file,
    //                    1024,
    //                    "%s",
    //                    "instance.csv");

	//char c;
    //    while ((c=getopt (argc, argv,"F:h")) != EOF)
	//{
	//	switch(c)
	//	{
    //                    case 'F':
	//			snprintf(       instance_file,
	//					1024,
	//					"%s",
	//					optarg);
	//			break;

	//		case 'h':
	//			fprintf(stderr,"Usage: ./TP2 [options]\nOptions:\n\n");
	//			fprintf(stderr,"******** INSTANCE DATA ********\n");
	//			fprintf(stderr,"\t-F Instance file name to load............................(default %s).\n",instance_file);
	//			break;
	//		default:
	//			exit(0);
	//	}
	//}

	//dataSet data;

	dataSet* data = create_instance(5,"tournament1.csv");
	if (data == NULL) {
		fprintf(stderr, "Failed to create instance.\n");
		return 1;
	}
	solve_2X_tournament(data);

	return rval;
}

