#include <stdlib.h> // Pour pouvoir utiliser exit()
#include <stdio.h> // Pour pouvoir utiliser printf()
#include <math.h> // Pour pouvoir utiliser sin() et cos()
#include "GfxLib.h" // Seul cet include est necessaire pour faire du graphique
#include "BmpLib.h" // Cet include permet de manipuler des fichiers BMP
#include "ESLib.h" // Pour utiliser valeurAleatoire()

int carteFile[18][18] = {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,1},
	{1,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,1},
	{1,0,0,0,5,0,0,0,0,0,0,0,0,0,5,0,0,1},
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

int taille = 35;

int dimension = 18;

int positionXJoueur;
int positionYJoueur;
int* listspawnX;
int* listspawnY;

int getTaille() {
	return taille;
}
void getCarte(int newCarte[dimension][dimension]) {
	for (int i=0;i<dimension;i++){
		for(int j=0;j<dimension;j++)
			newCarte[i][j] = carteFile[i][j];
	}
}

void setCarte(int newCarte[dimension][dimension]) {
	for (int i=0;i<dimension;i++){
		for(int j=0;j<dimension;j++)
			carteFile[i][j] = newCarte[i][j];
	}

}
int recherchePositionX() {
	for(int i = 0; i<dimension;i++) {
		for(int j = 0;j<dimension;j++) {
			if(carteFile[i][j]==3) {
				positionXJoueur = i;
			}
		}
	}
	return positionXJoueur;
}

int recherchePositionY() {
	for(int i = 0; i<dimension;i++) {
		for(int j = 0;j<dimension;j++) {
			if(carteFile[i][j]==3) {
				positionYJoueur = j;
			}
		}
	}
	return positionYJoueur;
}

int* recherchePositionSpawnX() {
	int s = 0;
	listspawnX = (int*)malloc(sizeof(int)*4);
	for(int i = 0; i<18;i++) {
		for(int j = 0;j<18;j++) {
			if(carteFile[i][j]==2) {
				listspawnX[s] = i;
				s=s+1;
			}
		}
	}
	return listspawnX;
}

int* recherchePositionSpawnY() {
	int s = 0;
	listspawnY = (int*)malloc(sizeof(int)*4);
	for(int i = 0; i<18;i++) {
		for(int j = 0;j<18;j++) {
			if(carteFile[i][j]==2) {
				listspawnY[s] = j;
				s=s+1;
			}
		}
	}
	return listspawnY;
}
			

void affichageCarte(int newCarte[18][18]) {
	DonneesImageRGB *ennemi = NULL;
	DonneesImageRGB *bombe = NULL;
	DonneesImageRGB *explosion = NULL;
	DonneesImageRGB *perso = NULL;
	DonneesImageRGB *mur = NULL;
	DonneesImageRGB *spawner = NULL;

	ennemi = lisBMPRGB("images/ennemi.bmp");
	bombe = lisBMPRGB("images/bombe.bmp");
	explosion = lisBMPRGB("images/explosion.bmp");
	perso = lisBMPRGB("images/bomberman.bmp");
	mur = lisBMPRGB("images/mur.bmp");
	spawner = lisBMPRGB("images/portal.bmp");

	// On part d'un fond d'ecran blanc
	effaceFenetre (255, 255, 255);
	
	//affichage de la matrice carte[][]
	for(int i = 0; i<dimension;i++) {
		for(int j = 0; j<dimension; j++) {
			if(newCarte[i][j]==0) couleurCourante(255,255,255); 	//fond
			
			if(newCarte[i][j]==7) couleurCourante(255 ,102 ,204); //bonus
			rectangle((i*taille), (j*taille),(i*taille)+taille,(j*taille)+taille);



			if(newCarte[i][j]==1)  ecrisImage(i*taille, j*taille, mur->largeurImage, mur->hauteurImage, mur->donneesRGB);	//mur indestructible
			if(newCarte[i][j]==2)  ecrisImage(i*taille, j*taille, spawner->largeurImage, spawner->hauteurImage, spawner->donneesRGB);	//spawner
			if(newCarte[i][j]==3)  ecrisImage(i*taille, j*taille, perso->largeurImage, perso->hauteurImage, perso->donneesRGB);	//perso
			if(newCarte[i][j]==4) ecrisImage(i*taille, j*taille, bombe->largeurImage, bombe->hauteurImage, bombe->donneesRGB);	//bombe
			if(newCarte[i][j]==5) ecrisImage(i*taille, j*taille, ennemi->largeurImage, ennemi->hauteurImage, ennemi->donneesRGB); //ennemi
			if(newCarte[i][j]==6) ecrisImage(i*taille, j*taille, explosion->largeurImage, explosion->hauteurImage, explosion->donneesRGB);		//explosion
			
		}
	}
}

