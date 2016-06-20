/*
 * SAT.c
 *
 *  Created on: Nov 3, 2014
 *  Author: Simone Corsini
 *
 *  Call functions in "file.c", "createCNF.c" and "algorithmFunctions.c" in order to to create CNF and call SAT Solver.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <igraph.h>
#include "glucoselib.h"

// Definizione della struttura che conterrà l'espressione ed il numero di clausole.
typedef struct
{
	char *expr;
	int num_clauses;
	int undec_args;
	int not_in_args;
} cnf;

typedef struct
{
	igraph_t graph;
	int *argumentsVector;
} graph;

typedef struct
{
	char *ep;
	int num_extensions;
} preferredExtension;

// Prototipi delle funzioni presenti negli altri file.
//graph openFile_and_createGraph();
graph openFile_and_createGraph(char *filePath);
cnf createCNF(cnf myCNF, graph myGraph);
cnf firstIf(cnf cnfdf, int ret_value, int num_variables, int last_comp_found[], graph myGraph);
cnf secondIf(cnf myCNF, int num_variables, int last_comp_found[], graph myGraph, preferredExtension myPE);

int main(int argc, char *argv[])
{
	if (argc == 2)
	{
		char *inputFile = malloc(sizeof(char)*50);
		inputFile = (char *) argv[1];

		// Funzione che crea una tabella di attributi da assegnare agli argomenti del grafo.
		// Concretamente, essa viene chiamata per una visualizzazione più immediata del risultato del SAT Solver.
		igraph_i_set_attribute_table(&igraph_cattribute_table);

		// Chiamata alla funzione presente nel file "file.c", che legge un file e crea un grafo orientato.
		igraph_t g;
		graph myGraph = {g, malloc(sizeof(int)*100000)};

		myGraph = openFile_and_createGraph(inputFile);

		cnf myCNF = {malloc(sizeof(char)*999999999), 0, 0, 0};
		myCNF.expr[0] = '\0';   // Assegnazione necessaria per il corretto funzionamento di "strcat()".

		cnf cnfdf = {malloc(sizeof(char)*999999999), 0, 0, 0};

		// Chiamata alla funzione presente nel file "createCNF.c", la quale chiama altre funzioni e calcola la CNF.
		myCNF = createCNF(myCNF, myGraph);

		preferredExtension myPE = {malloc(sizeof(char)*999999999), 0};
		myPE.ep[0] = '\0';   // Assegnazione necessaria per il corretto funzionamento di "strcat()".

		int num_variables = 3*(igraph_vcount(&(myGraph.graph)));
		int last_comp_found[num_variables];
		int ret_value = 0, pref_cand;

		// Enumerating the preferred extensions of an Argumentation Framework.
		do
		{
			strcpy(cnfdf.expr, myCNF.expr);
			cnfdf.num_clauses = myCNF.num_clauses;
			pref_cand = 0;

			do
			{
				cnfdf.undec_args = 0;
				cnfdf.not_in_args = 0;

				ret_value = glucoseSAT(cnfdf.expr, num_variables, cnfdf.num_clauses, last_comp_found);

				if (ret_value == 10)
				{
					pref_cand = 1;
					cnfdf = firstIf(cnfdf, ret_value, num_variables, last_comp_found, myGraph);
				}

			} while (ret_value == 10 && cnfdf.undec_args == 1);

			if (pref_cand == 1)
			{
				myCNF = secondIf(myCNF, num_variables, last_comp_found, myGraph, myPE);
				myPE.num_extensions++;
			}

		} while (pref_cand == 1);

		if (strlen(myPE.ep) == 0)
			strcat(myPE.ep, "{}");

		printf("%s", myPE.ep);

		igraph_destroy(&(myGraph.graph));
	}

	else
	{
		printf("Missing input file.\n");

		printf("###Running\n");
		printf("./IgraphSAT <inputfile>\n");
	}

	return 0;
}
