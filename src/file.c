/*
 * file.c
 *
 *  Created on: Nov 3, 2014
 *  Author: Simone Corsini
 *
 *	Open file and create graph.
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <igraph.h>

typedef struct
{
	igraph_t graph;
	int *argumentsVector;
} graph;

graph openFile_and_createGraph(char *filePath)
{
	igraph_t g;
	igraph_vector_t v;
	FILE *inputFile;
	inputFile = fopen(filePath, "r");

//	printf("Reading file '%s'...\n", filePath);

	if (inputFile == NULL)
	{
		printf("An error occurred while opening the file.\n");
		fclose(inputFile);
		exit(1);
	}
//	else
//		printf("File read successfully!\n");

	graph myGraph = {g, malloc(sizeof(int)*100000)};
	char line[32];   // Variabile che viene usata per leggere ogni riga del file con "fgets()".
	int lineIterator, wordIterator, edgesNumber = 0, intAttacker, intAttacked, vectorCounter = 0, intArgument;
	char attacker[9], attacked[9], argument[9];

	// Conto il numero di attacchi, in modo da inizializzare il vettore v.
	while (fgets(line, sizeof line, inputFile) != NULL)
	{
		// Questa condizione salva l'indice dell'argomento che attacca.
		if (strstr(line, "arg") != NULL)
		{
			wordIterator = 0;
			for (lineIterator = 0; line[lineIterator] != ')'; lineIterator++)
			{
				// Se il carattere è una cifra [0, 9].
				if (line[lineIterator] >= 48 && line[lineIterator] <= 57)
				{
					argument[wordIterator] = line[lineIterator];
					wordIterator++;
				}
			}

			argument[wordIterator] = '\0';

			// Converto le stringhe che rappresentano i due argomenti in interi, così da inserirli nel vettore v.
			intArgument = atoi(argument);
			myGraph.argumentsVector[vectorCounter] = intArgument;
			vectorCounter++;
		}

		else if (strstr(line, "att") != NULL)
			edgesNumber++;
	}

//	printf("Edges number: %d.\n", edgesNumber);
	igraph_vector_init(&v, edgesNumber*2);   // Il secondo argomento è il numero di attacchi * 2.
	rewind(inputFile);   // Riprendo la lettura del file dall'inizio.
	vectorCounter = 0;

	// Ciclo che analizza ogni riga del file di input.
	while (fgets(line, sizeof line, inputFile) != NULL)
	{
		// Se la riga del file contiene un attacco, lo aggiungo al vettore v.
		// Eventuali argomenti che non attaccano né subiscono attacchi sono automaticamente inseriti nel grafo.

		// Questa condizione salva l'indice dell'argomento che attacca.
		if (strstr(line, "att") != NULL)
		{
			wordIterator = 0;
			for (lineIterator = 0; line[lineIterator] != ','; lineIterator++)
			{
				// Se il carattere è una cifra [0, 9].
				if (line[lineIterator] >= 48 && line[lineIterator] <= 57)
				{
					attacker[wordIterator] = line[lineIterator];
					wordIterator++;
				}
			}

			attacker[wordIterator] = '\0';
			wordIterator = 0;

			// Riprendo la lettura della riga dal punto in cui si è fermato lineIterator.
			// Questa condizione salva l'indice dell'argomento che viene attaccato.
			for (; line[lineIterator] != ')'; lineIterator++)
			{
				// Se il carattere è una cifra [0, 9].
				if (line[lineIterator] >= 48 && line[lineIterator] <= 57)
				{
					attacked[wordIterator] = line[lineIterator];
					wordIterator++;
				}
			}

			attacked[wordIterator] = '\0';

			// Converto le stringhe che rappresentano i due argomenti in interi, così da inserirli nel vettore v.
			intAttacker = atoi(attacker);
			intAttacked = atoi(attacked);
//			printf("Attacker: %d. ", intAttacker);
//			printf("Attacked: %d.\n", intAttacked);
			VECTOR(v)[vectorCounter] = intAttacker; VECTOR(v)[vectorCounter+1] = intAttacked;
			vectorCounter += 2;
		}
	}

	// Creo il grafo direzionato, chiudo il file e restituisco alla funzione "main()" il grafo stesso.
	igraph_create(&(myGraph.graph), &v, 0, IGRAPH_DIRECTED);
	fclose(inputFile);

	return myGraph;
}
