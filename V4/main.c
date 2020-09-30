#include <stdlib.h> // Pour pouvoir utiliser exit()
#include <stdio.h> // Pour pouvoir utiliser printf()
#include <math.h> // Pour pouvoir utiliser sin() et cos()
#include "GfxLib.h" // Seul cet include est necessaire pour faire du graphique
#include "BmpLib.h" // Cet include permet de manipuler des fichiers BMP
#include "ESLib.h" // Pour utiliser valeurAleatoire()
#include <time.h> //utiliser la fonction time() et diftime()

#include "carte.h"
#include "menu.h"
#include "personnage.h"
#include "bombe.h"
//#include "ennemi.h"

// Largeur et hauteur par defaut d'une image correspondant a nos criteres
#define LargeurFenetre 630
#define HauteurFenetre 630

#define TRUE 1
#define FALSE 0

#define ROWS 18
#define COLS 18
#define Wall -1   //cases interdites
#define Infinity 1000 // Must be greater than the sum of rows and columns numbers.

typedef struct coord COORD;
typedef struct node NODE;

int** convert_to_matrix(char* adress);
int*** make_dist_matrix(int** matrix, int rows, int cols);
void print_3d_int_mat(int*** matrix, int n, int m);
void print_int_mat(int** matrix);
void print_coord(COORD pos);
void print_list(NODE* list);
NODE* new_head(NODE* list, COORD pos);
NODE* delete_pos(NODE* list, int i, int j);

COORD get_dustmen(int** matrix, int rows, int cols);
COORD get_ennemi(int** matrix, int rows, int cols);
NODE* get_dumpsters(int** matrix, int rows, int cols);
NODE* visit(int*** dist_map, NODE* list, int rows, int cols, int i_prev, int j_prev, int distance, int i, int j);
int*** Dijkstra(int** matrix, int rows, int cols, int i0, int j0);
NODE* follow_trail(int*** dist_map, int i0, int j0, COORD nearest);
NODE* get_trajectory(int** matrix, int rows, int cols,COORD joueur, NODE* ennemi);
bool avance_ennemi(bool av,int posX,int posY);

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

/* La fonction de gestion des evenements, appelee automatiquement par le systeme
des qu'une evenement survient */
void gestionEvenement(EvenementGfx evenement);

int carte[18][18] = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
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
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
	
int carteBase[18][18]={{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,1},
	{1,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,1,0,0,0,1,1,1,1,1,1,0,0,1,0,0,1},
	{1,0,1,0,1,0,0,0,0,0,0,0,0,0,1,0,0,1},
	{1,0,1,0,1,0,1,0,1,1,1,0,0,0,1,0,0,1},
	{1,2,1,0,1,0,1,0,3,0,1,0,0,0,1,0,0,1},
	{1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,2,1},
	{1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,0,1},
	{1,0,1,0,1,0,1,0,0,0,1,0,1,0,1,0,0,1},
	{1,0,1,0,1,0,0,0,0,0,0,0,0,0,1,0,0,1},
	{1,0,1,0,0,0,1,1,1,1,1,1,0,0,1,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,1,1,1,0,0,0,0,5,0,0,0,1,1,1,0,1},
	{1,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};
	

int j;
int taille;
perso* personnage;

int main(int argc, char **argv)
{
	initialiseGfx(argc, argv);
	
	prepareFenetreGraphique("GfxLib", LargeurFenetre, HauteurFenetre);
	
	/* Lance la boucle qui aiguille les evenements sur la fonction gestionEvenement ci-apres,
		qui elle-meme utilise fonctionAffichage ci-dessous */
	lanceBoucleEvenements();
	
	return 0;
}




/* La fonction de gestion des evenements, appelee automatiquement par le systeme
des qu'une evenement survient */
void gestionEvenement(EvenementGfx evenement)
{
	static bool pleinEcran = false; // Pour savoir si on est en mode plein ecran ou pas
	static DonneesImageRGB *image = NULL; // L'image a afficher au centre de l'ecran
	//position personnage
	static int positionX;
	static int positionY;
	
	static int oldPositionX;
	static int oldPositionY;
	//static int vie = 2;
	
	//position bombe
	static int nbBombe = 1;
	static bool bombePose = FALSE;
/*
	static int bombePositionX, bombePositionY;

	
	static bool timer = FALSE;
	
	static bool explobombe = FALSE;
	static bool explosion = TRUE;
	
	static clock_t start_tBombe = 0, end_tBombe = 0;
	static double diff_tBombe = 0;
	
	static clock_t start_tExplosion = 0, end_tExplosion = 0;
	static double diff_tExplosion = 0;
	
	static int oldbombePositionX;
	static int oldbombePositionY;
*/
	
	//position ennemi
	static int carreX;
	static int carreY;
	static int vieNMI = 1;
	static bool ennemituer = FALSE;
	
	//gestion des pages
	static int var = 0;
	
	static int* listspawnX, *listspawnY;
	
	static bool peutAvancer = TRUE;	

	static DonneesImageRGB *gameover = NULL;
	
	//ennemi
	static bool av = TRUE;
	static COORD joueur;
	static NODE* ennemi;
	static bool timernmi = FALSE;
	static clock_t start_tnmi, end_tnmi;
	static double diff_tnmi;
	static int** map_matrix;
	static int positionXennemi, positionYennemi, oldPositionXennemi, oldPositionYennemi;

	//spawner
	static int nbr;
	static int s;
	static int baseEnnemi = 1;
	NODE* newEnnemi =NULL;
	NODE* headEnnemi = NULL;


	switch (evenement)
	{
		case Initialisation:
			gameover = lisBMPRGB("images/gameover.bmp");
			getCarte(carte);
			
			taille = getTaille();

			//initialisation du menu avec ces images
			initialisationMenu();

			

			//recherche position initial joueur
			personnage = initialisationPerso(personnage);
			if(personnage == NULL) printf("erreur allocation personnage\n");
			positionX = getXPerso(personnage);
			positionY = getYPerso(personnage);

			//init spawns
			listspawnX = recherchePositionSpawnX();
			listspawnY = recherchePositionSpawnY();
			
			//carre noir
			for(int i = 0; i<18;i++) {
				for(int j = 0;j<18;j++) {
					if(carte[i][j]==5) {
						carreX = i;
						carreY = j;
					}
				}
			}
			demandeTemporisation(20);
			break;
		
		case Temporisation:
			if(var == 1) {
				printf("avance_ennemi: %d\n", av);
				
				

				oldPositionX = getOldPositionX();
				oldPositionY = getOldPositionY();

				if (getViePerso(personnage) > 0){
					//pathfinding
					//av = avance_ennemi(av,positionX,positionY);
					//pathfinding
printf("flag 1\n");
					if(av == TRUE) {	
printf("flag 2\n");	
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
								map_matrix[i][j] = carte[i][j];
						}
						joueur.Fst=positionX;
						joueur.Snd=positionY;
						//printf("\njoueur: ");
						//print_coord(joueur);
						ennemi = get_dumpsters(map_matrix,ROWS,COLS);
						while(ennemi != NULL) {
							//printf("\nennemi: ");
					
							positionXennemi=ennemi->Pos.Fst;
							positionYennemi=ennemi->Pos.Snd;
							oldPositionXennemi = positionXennemi;
							oldPositionYennemi = positionYennemi;
							//print_list(ennemi);
							//printf("\n");
							NODE* trajectory = get_trajectory(map_matrix,ROWS,COLS, joueur, ennemi);
							//printf("\n\ntrajectory: ");
							//print_list(trajectory);
							//printf("\n");
							ennemi->Pos.Fst=trajectory->Pos.Fst;
							ennemi->Pos.Snd=trajectory->Pos.Snd;
							positionXennemi=ennemi->Pos.Fst;
							positionYennemi=ennemi->Pos.Snd;
							//print_list(ennemi);
							carte[positionXennemi][positionYennemi] = 5;
							carte[oldPositionXennemi][oldPositionYennemi] = 0;
							ennemi=ennemi->Next;
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
					//printf("%f", diff_tnmi);
					//printf("\n");
					if(diff_tnmi > 0.5) {
						av = TRUE;
						timernmi = FALSE;
					}
					//Fin path finding



					//gestion spawner
					//listspawnX = recherchePositionSpawnX();
					//listspawnY = recherchePositionSpawnY();
					/*NODE* headList = NULL;
					headList = (NODE*)malloc(sizeof(NODE));
					nbr = rand() % 3;

					headEnnemi = (NODE*)malloc(sizeof(NODE));
					headEnnemi->vie = 1;
					headEnnemi->Next = NULL;
					headEnnemi = headList;*/
					//printf("Detail: %d\n",nbr+baseEnnemi);
					/*for (int i=0;i<nbr+baseEnnemi;i++){
						newEnnemi = (NODE*)malloc(sizeof(NODE));
						newEnnemi->vie = 1;
						headList->Next = newEnnemi;
						newEnnemi = headList;
					}

					while (headEnnemi->Next != NULL){

						s = rand() % 4;
						headEnnemi->Pos.Fst = listspawnX[s];
						headEnnemi->Pos.Snd = listspawnY[s];
						carte[headEnnemi->Pos.Fst][headEnnemi->Pos.Snd] = 5;
						setCarte(carte);
						headList = headEnnemi;
						headEnnemi = headEnnemi->Next;
						
						free(headList);
					}*/
					//fin gestion spawner

					//toutes les cases de fond de map
					if ((carte[oldPositionX][oldPositionY]!=6) && (carte[oldPositionX][oldPositionY]!=5) && (carte[oldPositionX][oldPositionY] != 2))
						carte[oldPositionX][oldPositionY]=0;
					DesaffichagePersonnageOldPosition(oldPositionX, oldPositionY);
					//conserve les spawns
			
					for (int s=0;s<4;s++)
						carte[listspawnX[s]][listspawnY[s]] = 2;
					setCarte(carte);
				
					peutAvancer = getPeutAvancer();
					
					peutAvancer = vitessePerso(personnage,peutAvancer);
					//perteur de vie
					if (vieNMI>0){
						carte[carreX][carreY]=5;
						ennemituer = TRUE;
					}
					else if (ennemituer == TRUE){
						carte[carreX][carreY]=0;
						ennemituer = FALSE;
					}

					//gestion bombe
					/*tempo*/
					
					
					bombePose = gestionAffichageBombe(personnage,bombePose);
					affichagePerso(positionX, positionY);
					getCarte(carte);
				
				}
			
			
				else {
					for(int i = 0; i<18;i++) {
						for(int j = 0;j<18;j++) {
							var = 2;
							carte[i][j]=0;
						}
					}
				}

				setCarte(carte);
			}
					
			rafraichisFenetre();
			break;
			
		case Affichage:
			
			if(var==0) affichageMenu();
			if(var==1) affichageCarte(carte);
			if(var==2) ecrisImage(0, 0, gameover->largeurImage, gameover->hauteurImage, gameover->donneesRGB);

			break;
			
		case Clavier:
			printf("%c : ASCII %d\n", caractereClavier(), caractereClavier());

			switch (caractereClavier())
			{
				//touches position
				case 'D':
				case 'd':
					positionX = deplacementPersoDroit(personnage,positionX,positionY, peutAvancer);
					break;
				case 'Q':
				case 'q':
					positionX = deplacementPersoGauche(personnage,positionX,positionY, peutAvancer);
					break;
				case 'S':
				case 's':
					if(var == 0) {
						curseurBas();
						break;
					}
					if(var == 1) {
						positionY = deplacementPersoBas(personnage,positionX,positionY, peutAvancer);
					}
					break;
				case 'Z':
				case 'z':
					if(var == 0) {
						curseurHaut();
						break;
					}
					if(var == 1) {
						positionY = deplacementPersoHaut(personnage,positionX,positionY, peutAvancer);
					}
					break;
				//touche bombe	
				case 'B':
				case 'b':
					bombePose = poseBombe(positionX, positionY, nbBombe);
					break;

				case 13:
					if(var == 0) {
						var = lancementJeu();
						sortieMenu();
						break;
					}
					break;
				case 27:
					if(var == 1) {
						var = 0;
						break;
					}
					break;
				


//touches utilitaires
				case 'P': /* Pour sortir quelque peu proprement du programme */
				case 'p':
					libereDonneesImageRGB(&image); /* On libere la structure image,
					c'est plus propre, meme si on va sortir du programme juste apres */
					termineBoucleEvenements();
					break;

				case 'F':
				case 'f':
					pleinEcran = !pleinEcran; // Changement de mode plein ecran
					if (pleinEcran)
						modePleinEcran();
					else
						redimensionneFenetre(LargeurFenetre, HauteurFenetre);
					break;

				case 'R':
				case 'r':
					// Configure le systeme pour generer un message Temporisation
					// toutes les 20 millisecondes (rapide)
					demandeTemporisation(20);
					break;

				case 'L':
				case 'l':
					// Configure le systeme pour generer un message Temporisation
					// toutes les 100 millisecondes (lent)
					demandeTemporisation(100);
					break;

				case 'M':
				case 'm':
					// Configure le systeme pour ne plus generer de message Temporisation
					demandeTemporisation(-1);
					break;
			}
			break;
			
		case ClavierSpecial:
			printf("ASCII : %d\n", toucheClavier());
			break;

		case BoutonSouris:
			if (etatBoutonSouris() == GaucheAppuye)
			{
				printf("Bouton gauche appuye en : (%d, %d)\n", abscisseSouris(), ordonneeSouris());
			}
			else if (etatBoutonSouris() == GaucheRelache)
			{
				printf("Bouton gauche relache en : (%d, %d)\n", abscisseSouris(), ordonneeSouris());
			}
			break;
		
		case Souris: // Si la souris est deplacee
			break;
		
		case Inactivite: // Quand aucun message n'est disponible
			break;
		
		case Redimensionnement: // La taille de la fenetre a ete modifie ou on est passe en plein ecran
			
			printf("Largeur : %d\t", largeurFenetre());
			printf("Hauteur : %d\n", hauteurFenetre());
			break;
	}
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
int*** make_dist_matrix(int** matrix, int rows, int cols) {
	int*** dist_matrix = (int***) malloc(rows * sizeof(int**));
	for (int i=0; i<rows; i++) {
		dist_matrix[i] = (int**) malloc(cols * sizeof(int*));
		for (int j=0; j<cols; j++) {
			//int dist_matrix[i][j][3] = {0, 0, 0};
			dist_matrix[i][j] = (int*) malloc(3 * sizeof(int));
			int c = matrix[i][j];
		    	if (c == 0 || c == 3) { // Don't forget the starting point.
				dist_matrix[i][j][0] = Infinity;
			}else{
				dist_matrix[i][j][0] = Wall;
			}
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
	//printf("\ndist_map:\n");
	//print_3d_int_mat(dist_map, rows, cols);
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

