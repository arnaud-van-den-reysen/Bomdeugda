#include <stdlib.h> // Pour pouvoir utiliser exit()
#include <stdio.h> // Pour pouvoir utiliser printf()
#include <math.h> // Pour pouvoir utiliser sin() et cos()
#include <stdbool.h>
#include <time.h>

#include "carte.h"
#include "ennemi.h"

#define TRUE 1
#define FALSE 0

#define ROWS 18
#define COLS 18
#define Wall -1   //cases interdites
#define Infinity 1000 // Must be greater than the sum of rows and columns numbers.

int carteEnnemi[18][18] = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	};

typedef struct coord // Pair of ints.
{
	int Fst;
	int Snd;
}COORD;

typedef struct node // Linked list of COORD.
{	
	int vie;
	COORD Pos;
	struct node* Next;
}NODE;

bool avance_ennemi(bool av, int posX ,int posY)
{	
	static COORD joueur;
	static NODE* nmi;
	static bool timernmi = FALSE;
	static clock_t start_tnmi, end_tnmi;

	getCarte(carteEnnemi);
/*
	for(int i = 0; i<18;i++) {
	for(int j = 0; j<18;j++) {
		printf("%d",carteEnnemi[i][j]);
	}printf("\n");}
*/
	static double diff_tnmi;

	static int** map_matrix;

	static int positionXennemi, positionYennemi, oldPositionXennemi, oldPositionYennemi;
	if(av == TRUE) 
	{		
		av = FALSE;
			
		map_matrix = (int**)malloc(sizeof(int*)*18);
		for (int h = 0;h<18;h++)
		{
			map_matrix[h] = (int*)malloc(sizeof(int)*18);
			if(map_matrix[h] == NULL)
				printf("erreur allocation\n");
			if (map_matrix == NULL)
				printf("erreur allocation\n");
		}
		for (int i=0;i<18;i++)
		{
			for(int j = 0;j<18;j++)
				map_matrix[i][j] = carteEnnemi[i][j];
		}
		
		joueur.Fst=posX;
		joueur.Snd=posY;
		nmi=get_dumpsters(map_matrix,ROWS,COLS);
		
		while(nmi != NULL)
		{	
			printf("flog 1\n");
			printf("NMI: %d\n",nmi->Pos.Snd);
			printf("NMI: %d\n",nmi->Pos.Fst);
			positionXennemi=nmi->Pos.Fst;
			positionYennemi=nmi->Pos.Snd;
			oldPositionXennemi = positionXennemi;
			oldPositionYennemi = positionYennemi;
			NODE* trajectory = get_trajectory(map_matrix,ROWS,COLS, joueur, nmi);
			printf("\n\ntrajectory: ");
			print_list(trajectory);
			printf("\n\n");
			nmi->Pos.Fst=trajectory->Pos.Fst;
			nmi->Pos.Snd=trajectory->Pos.Snd;
			positionXennemi=nmi->Pos.Fst;
			positionYennemi=nmi->Pos.Snd;
			print_list(nmi);
printf("\n");
			carteEnnemi[positionXennemi][positionYennemi] = 5;
			carteEnnemi[oldPositionXennemi][oldPositionYennemi] = 0;
			printf("posENNEMIX: %d\n",positionXennemi);
			printf("posENNEMIY: %d\n",positionYennemi);
			setCarte(map_matrix);
			
			nmi=nmi->Next;
		}
		free(map_matrix);

	}

	
	
	
		//vitesse ennemi
	if(av == FALSE) {
		if(timernmi == FALSE) {
			start_tnmi = clock();
			timernmi = TRUE;
		}
		if(timernmi == TRUE) {
			end_tnmi = clock();
		}
	}
	diff_tnmi = (double)(end_tnmi - start_tnmi) /CLOCKS_PER_SEC;
	printf("diff_tnmi: %f", diff_tnmi);
	printf("\n");
	if(diff_tnmi > 0.5) 
	{
		av = TRUE;
		timernmi = FALSE;
	}
	
	setCarte(carteEnnemi);
	return av;
}
// Import a text map into a char matrix. Make sure the text map is filled with spaces,
// as it needs to have a rectangular shape.
int** convert_to_matrix(char* adress)
{
	FILE *F = fopen(adress, "r");
    if (F == NULL)
	{
        printf("Erreur d'ouverture du fichier.\n");
        return NULL;
	}
	else
	{
		printf("\nThis matrix has %d rows and %d columns:\n", ROWS, COLS);
		int** matrix = (int **) malloc(ROWS * sizeof(int*));
		int i, j;
		for (i=0; i<ROWS; i++)
		{
			fseek(F, ftell(F)+1, SEEK_SET);
			matrix[i] = (int*) malloc(COLS * sizeof(int));
			for (j=0; j<COLS; j++)
			{
				fscanf(F, "%d", &matrix[i][j]);
				fseek(F, ftell(F)+1, SEEK_SET);
			}
			fseek(F, ftell(F)+2, SEEK_SET); /* Dodges carriage return. */
		}
		fclose(F);
		return matrix; // mallocated so the matrix isn't copied entirely.
	}
}

// Makes a 3-dimensional matrix, which will encode the matrix of distances from a source,
// and the coordinates of the node from which the current one is from.
int*** make_dist_matrix(int** matrix, int rows, int cols)
{
	int*** dist_matrix = (int***) malloc(rows * sizeof(int**));
    int i;
    for (i=0; i<rows; i++)
	{
		dist_matrix[i] = (int**) malloc(cols * sizeof(int*));
		int j;
        for (j=0; j<cols; j++)
		{
            //int dist_matrix[i][j][3] = {0, 0, 0};
            dist_matrix[i][j] = (int*) malloc(3 * sizeof(int));
            int c = matrix[i][j];
            if (c == 0 || c == 3) // Don't forget the starting point.
				dist_matrix[i][j][0] = Infinity;
			else
				dist_matrix[i][j][0] = Wall;
		}
	}
	return dist_matrix; // mallocated so the matrix isn't copied entirely.
}

// Print the map matrix.
void print_int_mat(int** matrix)
{
	printf("\n");
	int i;
	for (i=0; i<ROWS; i++)
	{
		int j;
		for (j=0; j<COLS; j++)
			printf("%d", matrix[i][j]);
		printf("\n");
	}
}
// Lists things:

void print_coord(COORD pos)
{
	printf("(%d, %d)", pos.Fst, pos.Snd);
}

void print_list(NODE* list)
{
	if (list != NULL)
	{
		print_coord(list -> Pos);
		printf(", ");
		print_list(list -> Next);
	}
}

// Adds a COORD at the star of the list.
// Use it like that: list = new_head(list, pos);
NODE* new_head(NODE* list, COORD pos)
{
	NODE* New = NULL;
	New = malloc(sizeof(NODE));
	if (New == NULL)
		printf("\nImpossible dallouer.\n");
	else
	{
        New -> Pos = pos;
        New -> Next = list;
	}
	return New;
}
// Deletes the first occurrence of a COORD in the list,
// does nothing if the COORD isn't found.
NODE* delete_pos(NODE* list, int i, int j)
{
	if (list == NULL)
	{
		printf("\nValue not found.\n");
		return NULL;
	}
	if ( (list -> Pos).Fst == i && (list -> Pos).Snd == j )
	{
		NODE* path = list -> Next;
		free(list);
		return path;
	}
	list -> Next = delete_pos(list -> Next, i, j);
	return list;
}
// Pathfinding things:

// Get the dustmen initial position.pour le perso
COORD get_dustmen(int** matrix, int rows, int cols)
{
	COORD pos;
	int i = 0, j;
	while (i<rows)
	{
		j = 0;
		while (j<cols)
		{
            if (matrix[i][j] == 3)
            {
				pos.Fst = i;
				pos.Snd = j;
				return pos;
            }
			j++;
		}
		i++;
	}
	if (i == rows)
		printf("\nNo dustmen found.\n");
	return pos;
}
COORD get_ennemi(int** matrix, int rows, int cols)
{
	COORD pos;
	int i = 0, j;
	while (i<rows)
	{
		j = 0;
		while (j<cols)
		{
            if (matrix[i][j] == 5)
            {
				pos.Fst = i;
				pos.Snd = j;
				return pos;
            }
			j++;
		}
		i++;
	}
	if (i == rows)
		printf("\nNo dustmen found.\n");
	return pos;
}

// Finds the filled dumpsters and put them into a list pour les ennemis
NODE* get_dumpsters(int** matrix, int rows, int cols)
{
	NODE* list = NULL;
	int i, j;
	for (i=0; i<rows; i++)
	{
		for (j=0; j<cols; j++)
		{
		    if (matrix[i][j] == 5)
		    {
		    	COORD pos;
		    	pos.Fst = i;
		    	pos.Snd = j;
		    	list = new_head(list, pos);
		    }
		}
	}
	if (list == NULL)
		printf("\nNo dumpster found.\n");
	return list;
}

// Affect the distance of an unvisited node, and saves the node from which it comes from.
NODE* visit(int*** dist_map, NODE* list, int rows, int cols, int i_prev, int j_prev, int distance, int i, int j)
{
	if ( i<0 || i == rows || j<0 || j == cols || dist_map[i][j][0] != Infinity ) // Out of map or wall.
		return list;
	else
	{
		dist_map[i][j][0] = distance;
		dist_map[i][j][1] = i_prev; // Previous node coordinates.
		dist_map[i][j][2] = j_prev;
		COORD pos;
		pos.Fst = i; // Current node coordinates.
		pos.Snd = j;
        return new_head(list, pos);
	}
}

// Fill the distance matrix with the actual distances from the source.
int*** Dijkstra(int** matrix, int rows, int cols, int i0, int j0)
{
	COORD source;
	source.Fst = i0;
	source.Snd = j0;
	int*** dist_map = make_dist_matrix(matrix, rows, cols);
	dist_map[i0][j0][0] = 0;
	int distance = 1;
	NODE* to_keep = NULL;
	NODE* to_test = new_head(NULL, source);
	while (to_keep != NULL || to_test != NULL)
	{
        if (to_test == NULL)
		{
			to_test = to_keep;
			to_keep = NULL;
			distance++;
		}
		else
		{
			int i = (to_test -> Pos).Fst;
			int j = (to_test -> Pos).Snd;
			to_keep = visit(dist_map, to_keep, rows, cols, i, j, distance, (i-1), j);
			to_keep = visit(dist_map, to_keep, rows, cols, i, j, distance, i, (j-1));
			to_keep = visit(dist_map, to_keep, rows, cols, i, j, distance, i, (j+1));
			to_keep = visit(dist_map, to_keep, rows, cols, i, j, distance, (i+1), j);
			to_test = to_test -> Next;
		}
	}
	return dist_map;
}

// Follow an already found path.
NODE* follow_trail(int*** dist_map, int i0, int j0, COORD nearest)
{
	NODE* trail = new_head(NULL, nearest);
	int i = nearest.Fst;
	int j = nearest.Snd;
	while (i != i0 || j != j0)
	{
		COORD prev;
		prev.Fst = dist_map[i][j][1];
		prev.Snd = dist_map[i][j][2];
		trail = new_head(trail, prev);
		i = prev.Fst;
		j = prev.Snd;
	}
	return trail -> Next;
	//return reverse_list(trail); // Nope.
}

NODE* get_trajectory(int** matrix, int rows, int cols,COORD joueur, NODE* ennemi)
{
	int i0 = ennemi->Pos.Fst;
	int j0 = ennemi->Pos.Snd;
	int*** dist_map = Dijkstra(matrix, rows, cols, i0, j0);
	printf("\ndist_map:\n");
	print_3d_int_mat(dist_map, rows, cols);
	NODE* trail = follow_trail(dist_map, i0, j0, joueur);
	return trail;
}

// Print the distance matrix.
void print_3d_int_mat(int*** matrix, int n, int m)
{
	printf("\n");
	int i, j, d;
	for (i=0; i<n; i++)
	{
		for (j=0; j<m; j++)
		{
			d = matrix[i][j][0];
			if (d == Wall)
				printf("%d",Wall);
			else if (d == Infinity)
				printf("0");
			else
				printf("%d", d);
		}
		printf("\n");
	}
}

