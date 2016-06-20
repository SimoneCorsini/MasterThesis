/*
 * createCNF.c
 *
 *  Created on: Nov 3, 2014
 *  Author: Simone Corsini
 *
 *  Functions used to calculate the CNF.
 *  In questo file sono presenti 9 funzioni, corrispondenti alla relativa regola di calcolo della clausola,
 *  e la funzione createCNF() che chiama le funzioni selezionate tra le 9.
 *  Ogni argomento "i", il cui numero di riferimento è compreso tra 0 ed N, viene rapprentato con 3 condizioni:
 *  - In = 3*(i+1) - 2.
 *  - Out = 3*(i+1) - 1.
 *  - Undec = 3*(i+1).
 *
 *  Ognuna delle 9 funzioni riceve in ingresso almeno la struct CNF ed il numero del vertice di riferimento.
 *  Le funzioni 3, 4, 5, 6, 7 e 8 ricevono in ingresso anche un vettore
 *  contenente gli argomenti che attaccano il vertice di riferimento.
 *  In ognuno delle 9 funzioni, i valori che rappresentano le condizioni (In, Out e Undec)
 *  sono calcolati come interi e successivamente salvati come stringhe con la funzione "sprintf()".
 *  Ognuna delle 9 funzioni prevede l'uso della funzione "strcat()"
 *  per salvare il nuovo pezzo di CNF (salvato in newFormula) nella variabile myCNF.expr.
 *  Ognuna delle 9 funzioni calcola il numero di clausole che ha aggiunto e
 *  salva il risultato in myCNF.num_clauses.
 *  Una volta che la CNF è stata aggiornata opportunamente,
 *  essa viene restituita e salvata nella funzione principale (createCNF()).
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

// Il primo metodo viene chiamato in ogni caso.
cnf first(cnf myCNF, int vertex)
{
	int intValue[3];
	char *newFormula = malloc(sizeof(char)*50);

	intValue[0] = 3*(vertex+1) - 2;
	intValue[1] = 3*(vertex+1) - 1;
	intValue[2] = 3*(vertex+1);

	if (strlen(myCNF.expr) == 0)
		sprintf(newFormula, "%d %d %d 0", intValue[0], intValue[1], intValue[2]);
	else
		sprintf(newFormula, "\n%d %d %d 0", intValue[0], intValue[1], intValue[2]);

	strcat(myCNF.expr, newFormula);

	int i;
	for(i=0; i<3; i++)
		intValue[i] *= -1;

	sprintf(newFormula, "\n%d %d 0", intValue[0], intValue[1]);
	strcat(myCNF.expr, newFormula);

	sprintf(newFormula, "\n%d %d 0", intValue[0], intValue[2]);
	strcat(myCNF.expr, newFormula);

	sprintf(newFormula, "\n%d %d 0", intValue[1], intValue[2]);
	strcat(myCNF.expr, newFormula);

	myCNF.num_clauses += 4;

	return myCNF;
}

// Il secondo metodo viene chiamato solo se l'argomento non ha attaccanti.
cnf second(cnf myCNF, int vertex)
{
	int intValue;
	char *newFormula = malloc(sizeof(char)*50);

	intValue = 3*(vertex+1) - 2;
	sprintf(newFormula, "\n%d 0", intValue);
	strcat(myCNF.expr, newFormula);

	intValue = (3*(vertex+1) - 1)*(-1);
	sprintf(newFormula, "\n%d 0", intValue);
	strcat(myCNF.expr, newFormula);

	intValue = (3*(vertex+1))*(-1);
	sprintf(newFormula, "\n%d 0", intValue);
	strcat(myCNF.expr, newFormula);

	myCNF.num_clauses += 3;

	return myCNF;
}

// Questo metodo viene chiamato se l'argomento ha attaccanti.
cnf third(cnf myCNF, int vertex, igraph_vector_t *attackersVector)
{
	int intValue, attacker, j;
	char *newFormula = malloc(sizeof(char)*50);

	intValue = 3*(vertex+1) - 2;
	sprintf(newFormula, "\n%d ", intValue);
	strcat(myCNF.expr, newFormula);

	for(j = 0; j < igraph_vector_size(attackersVector); j++)
	{
		attacker = VECTOR(*attackersVector)[j];
		intValue = (3*(attacker+1) - 1)*(-1);
		sprintf(newFormula, "%d ", intValue);
		strcat(myCNF.expr, newFormula);
	}

	strcat(myCNF.expr, "0");

	myCNF.num_clauses += 1;

	return myCNF;
}

// Questo metodo viene chiamato se l'argomento ha attaccanti.
cnf fourth(cnf myCNF, int vertex, igraph_vector_t *attackersVector)
{
	int intValue[2], attacker, j;
	char *newFormula = malloc(sizeof(char)*50);

	intValue[0] = (3*(vertex+1) - 2)*(-1);

	for(j = 0; j < igraph_vector_size(attackersVector); j++)
	{
		attacker = VECTOR(*attackersVector)[j];
		intValue[1] = 3*(attacker+1) - 1;
		sprintf(newFormula, "\n%d %d 0", intValue[0], intValue[1]);
		strcat(myCNF.expr, newFormula);
	}

	myCNF.num_clauses += igraph_vector_size(attackersVector);

	return myCNF;
}

// Questo metodo viene chiamato se l'argomento ha attaccanti.
cnf fifth(cnf myCNF, int vertex, igraph_vector_t *attackersVector)
{
	int intValue[2], attacker, j;
	char *newFormula = malloc(sizeof(char)*50);

	intValue[1] = 3*(vertex+1) - 1;

	for(j = 0; j < igraph_vector_size(attackersVector); j++)
	{
		attacker = VECTOR(*attackersVector)[j];
		intValue[0] = (3*(attacker+1) - 2)*(-1);
		sprintf(newFormula, "\n%d %d 0", intValue[0], intValue[1]);
		strcat(myCNF.expr, newFormula);
	}

	myCNF.num_clauses += igraph_vector_size(attackersVector);

	return myCNF;
}

// Questo metodo viene chiamato se l'argomento ha attaccanti.
cnf sixth(cnf myCNF, int vertex, igraph_vector_t *attackersVector)
{
	int intValue, attacker, j;
	char *newFormula = malloc(sizeof(char)*50);

	intValue = (3*(vertex+1) - 1)*(-1);
	sprintf(newFormula, "\n%d ", intValue);
	strcat(myCNF.expr, newFormula);

	for(j = 0; j < igraph_vector_size(attackersVector); j++)
	{
		attacker = VECTOR(*attackersVector)[j];
		intValue = 3*(attacker+1) - 2;
		sprintf(newFormula, "%d ", intValue);
		strcat(myCNF.expr, newFormula);
	}

	strcat(myCNF.expr, "0");

	myCNF.num_clauses += 1;

	return myCNF;
}

// Questo metodo viene chiamato se l'argomento ha attaccanti.
cnf seventh(cnf myCNF, int vertex, igraph_vector_t *attackersVector)
{
	int intValue[2], attacker, j, k;
	char *newFormula = malloc(sizeof(char)*(igraph_vector_size(attackersVector)*50 + 50));

	intValue[0] = 3*(vertex+1);

	for(k = 0; k < igraph_vector_size(attackersVector); k++)
	{
		strcat(myCNF.expr, "\n");

		attacker = VECTOR(*attackersVector)[k];
		intValue[1] = (3*(attacker+1))*(-1);
		sprintf(newFormula, "%d %d ", intValue[0], intValue[1]);
		strcat(myCNF.expr, newFormula);

		for(j = 0; j < igraph_vector_size(attackersVector); j++)
		{
			attacker = VECTOR(*attackersVector)[j];
			intValue[1] = 3*(attacker+1) - 2;
			sprintf(newFormula, "%d ", intValue[1]);
			strcat(myCNF.expr, newFormula);
		}

		strcat(myCNF.expr, "0");
	}

	myCNF.num_clauses += igraph_vector_size(attackersVector);

	return myCNF;
}

// Questo metodo viene chiamato se l'argomento ha attaccanti.
cnf eighth(cnf myCNF, int vertex, igraph_vector_t *attackersVector)
{
	int intValue[2], attacker, j;
	char *newFormula = malloc(sizeof(char)*(igraph_vector_size(attackersVector)*50 + 50));

	intValue[0] = (3*(vertex+1))*(-1);

	for(j = 0; j < igraph_vector_size(attackersVector); j++)
	{
		attacker = VECTOR(*attackersVector)[j];
		intValue[1] = (3*(attacker+1) - 2)*(-1);
		sprintf(newFormula, "\n%d %d 0", intValue[0], intValue[1]);
		strcat(myCNF.expr, newFormula);
	}

	sprintf(newFormula, "\n%d ", intValue[0]);
	strcat(myCNF.expr, newFormula);

	for(j = 0; j < igraph_vector_size(attackersVector); j++)
	{
		attacker = VECTOR(*attackersVector)[j];
		intValue[1] = 3*(attacker+1);
		sprintf(newFormula, "%d ", intValue[1]);
		strcat(myCNF.expr, newFormula);
	}

	strcat(myCNF.expr, "0");

	myCNF.num_clauses += igraph_vector_size(attackersVector) + 1;

	return myCNF;
}

// Questo metodo viene chiamato in ogni caso.
cnf ninth(cnf myCNF, int vertexCount)
{
	int intValue, vertex;
	char *newFormula = malloc(sizeof(char)*50);

	strcat(myCNF.expr, "\n");

	for (vertex = 0; vertex < vertexCount; vertex++)
	{
		intValue = 3*(vertex+1) - 2;
		sprintf(newFormula, "%d ", intValue);
		strcat(myCNF.expr, newFormula);
	}

	strcat(myCNF.expr, "0");

	myCNF.num_clauses += 1;

	return myCNF;
}

/*
 * La funzione createCNF() alloca uno spazio di memoria per la struct myCNF, che sarà restituita alla funzione
 * chiamante una volta modificata opportunamente dalle funzioni invocate di seguito.
 * La funzione "igraph_vector_init()" è il costruttore del vettore che rappresenta gli attaccanti. Per ogni
 * argomento, questo vettore viene aggiornato in modo che contenga gli argomenti che lo attaccano.
 * A questo punto, per ogni argomento viene valutato il vettore di attaccanti e vengono chiamate
 * le opportune funzioni tra le 9 precedentemente dichiarate.
 * La variabile myCNF viene modificata ad ogni passaggio e viene restituita alla funzione "main()".
 */
cnf createCNF(cnf myCNF, graph myGraph)
{
	int i, argument;
	igraph_vector_t attackersVector;

	// Inizializzo il vettore che conterrà gli attaccanti dell'argomento analizzato nel ciclo for.
	igraph_vector_init(&attackersVector, igraph_vcount(&(myGraph.graph)));

	for(i = 0; i < igraph_vcount(&(myGraph.graph)); i++)
	{
		argument = myGraph.argumentsVector[i];
		myCNF = first(myCNF, argument);

		igraph_neighbors(&(myGraph.graph), &attackersVector, argument, IGRAPH_IN);

		if (igraph_vector_size(&attackersVector) == 0)
			myCNF = second(myCNF, argument);

		// C_2 encoding, the most efficient.
		else
		{
			myCNF = fourth(myCNF, argument, &attackersVector);
			myCNF = sixth(myCNF, argument, &attackersVector);
			myCNF = eighth(myCNF, argument, &attackersVector);
		}
	}

	myCNF = ninth(myCNF, igraph_vcount(&(myGraph.graph)));

	igraph_vector_destroy(&attackersVector);
	return myCNF;
}
