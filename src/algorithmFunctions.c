/*
 * algorithmFunctions.c
 *
 *  Created on: Nov 24, 2014
 *  Author: Simone Corsini
 *
 * Contains the first and second "if" of the main algorithm.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <igraph.h>

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

// Funzione che stampa la CO trovata ed effettua il primo "if" dell'algoritmo.
cnf firstIf(cnf cnfdf, int ret_value, int num_variables, int last_comp_found[], graph myGraph)
{
	char *newFormula = malloc(sizeof(char)*50), *remaining = malloc(sizeof(char)*(igraph_vcount(&(myGraph.graph))*50 + 50));
	remaining[0] = '\0';   // Assegnazione necessaria per il corretto funzionamento di "strcat()".
	int i, intValue, argument;

	for(i = 0; i < num_variables/3; i++)
	{
		argument = myGraph.argumentsVector[i];
		intValue = 3*(argument+1) - 2;

		// Se l'argomento è In.
		if (last_comp_found[argument*3] > 0)
		{
			// Righe 12-14 dell'algoritmo.
			sprintf(newFormula, "\n%d 0", intValue);
			strcat(cnfdf.expr, newFormula);
			cnfdf.num_clauses++;
		}

		// Righe 16-18 dell'algoritmo.
		else
		{
			// Se l'argomento è Undec.
			if (last_comp_found[argument*3+2] > 0)
			{

				cnfdf.undec_args = 1;

				sprintf(newFormula, "%d ", intValue);
				strcat(remaining, newFormula);
			}
		}
	}

	// Riga 19 dell'algoritmo.
	if (cnfdf.undec_args == 1)
	{
		strcat(remaining, "0");
		strcat(cnfdf.expr, "\n");
		strcat(cnfdf.expr, remaining);

		cnfdf.num_clauses++;
	}

	return cnfdf;
}

// Funzione che stampa la PE trovata ed effettua il secondo "if" dell'algoritmo.
cnf secondIf(cnf myCNF, int num_variables, int last_comp_found[], graph myGraph, preferredExtension myPE)
{
	int i, argument, intValue;
	char *newArgument = malloc(sizeof(char)*5), *newFormula = malloc(sizeof(char)*50), *oppSolution = malloc(sizeof(char)*(igraph_vcount(&(myGraph.graph))*50 + 50));
	oppSolution[0] = '\0';   // Assegnazione necessaria per il corretto funzionamento di "strcat()".

	myCNF.not_in_args = 0;

	strcat(myPE.ep, "{");

	for(i = 0; i < num_variables/3; i++)
	{
		argument = myGraph.argumentsVector[i];

		// Se l'argomento non è In.
		if (last_comp_found[argument*3] < 0)
		{
			// Righe 25-27 dell'algoritmo.
			myCNF.not_in_args = 1;

			intValue = 3*(argument+1) - 2;

			sprintf(newFormula, "%d ", intValue);
			strcat(oppSolution, newFormula);
		}

		else
		{
			if(myPE.ep[strlen(myPE.ep)-1] != '{')
				strcat(myPE.ep, " ");

			sprintf(newArgument, "a%d", argument);
			strcat(myPE.ep, newArgument);
		}
	}

	strcat(myPE.ep, "}\n");

	// Riga 28 dell'algoritmo.
	if (myCNF.not_in_args == 1)
	{
		myCNF.num_clauses++;

		strcat(oppSolution, "0");
		strcat(myCNF.expr, "\n");
		strcat(myCNF.expr, oppSolution);
	}

	return myCNF;
}
