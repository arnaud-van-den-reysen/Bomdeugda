#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "carte.h"
#include "personnage.h"

//definition boolean
#define TRUE 1
#define FALSE 0

#define true 1
#define false 0
	
typedef struct PERSO{
	int posX;
	int posY;
	double vitesse;
	int vie;
	int nbBombes;
}perso;

int cartePerso[18][18] = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
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

	
static int oldPositionX;
static int oldPositionY;
static int viePerso = 2;
static bool timerPerso = FALSE;
static clock_t start_tPerso, end_tPerso;
static double diff_tPerso;
static bool peutAvancer = TRUE;

static double vitPerso = 0.125;
static int nbBombesPerso = 1;

perso *initialisationPerso(perso* personnage){
	personnage = (perso*)malloc(sizeof(perso));
	personnage->posX = recherchePositionX();
	personnage->posY = recherchePositionY();
	personnage->vitesse = vitPerso;
	personnage->vie = viePerso;
	personnage->nbBombes = nbBombesPerso;
	return personnage;
}



void affichagePerso(int positionX, int positionY){
	//affiche le perso par dessus le reste
	getCarte(cartePerso);
	cartePerso[positionX][positionY]=3;
	setCarte(cartePerso);
}
	

	
int getXPerso(perso* personnage){
	return personnage->posX;
}

int getYPerso(perso* personnage){
	return personnage->posY;
}

int getViePerso(perso* personnage){
	return personnage->vie;
}

int getnbBombesPerso(perso* personnage){
	return personnage->nbBombes;
}

double getVitessePerso(perso* personnage){
	return personnage->vitesse;
}

void setXPersonnage(perso* personnage,int positionX){
	personnage->posX=positionX;
}

void setYPersonnage(perso* personnage,int positionY){
	personnage->posY=positionY;
}

void setViePersonnage(perso* personnage,int viePerso){
	personnage->vie = viePerso;
}

void setNbBombesPersonnage(perso* personnage,int NbBombePersonnage){
	personnage->nbBombes = NbBombePersonnage;
}

void setVitessePersonnage(perso* personnage, double Vitesse){
	personnage->vitesse = Vitesse;
}

int getOldPositionX(){
	return oldPositionX;
}
int getOldPositionY() {
	return oldPositionY;
}

bool vitessePerso(perso* personnage,bool peutAvancer) {
	//vitesse personnage
	if(peutAvancer == FALSE) {
		if(timerPerso == FALSE) {
			start_tPerso = clock();
			timerPerso = TRUE;
		}
		if(timerPerso == TRUE) {
			end_tPerso = clock();
		}
	}
	diff_tPerso = (double)(end_tPerso - start_tPerso) /CLOCKS_PER_SEC;
	if(diff_tPerso > getVitessePerso(personnage)) {
		setPeutAvancer(TRUE);
		timerPerso = FALSE;
	}
	return peutAvancer;
}

void DesaffichagePersonnageOldPosition(int oldPositionX, int oldPositionY){

	//affiche le perso par dessus le reste
	getCarte(cartePerso);
	if ((cartePerso[oldPositionX][oldPositionY]!=6) && (cartePerso[oldPositionX][oldPositionY]!=5))
		cartePerso[oldPositionX][oldPositionY]=0;
	setCarte(cartePerso);
						
}

bool getPeutAvancer() {
	return peutAvancer;
}
void setPeutAvancer(bool pav) {
	peutAvancer = pav;
}



int deplacementPersoHaut(perso* personnage,int positionX,int positionY, bool peutAvancer){
	if(peutAvancer == TRUE) {
	
		if ((cartePerso[positionX][positionY+1]==1) || (cartePerso[positionX][positionY+1]==4)) {
			return positionY;
		} 
		
		if (cartePerso[positionX][positionY+1]==5) {
			personnage->vie-=1;
			return positionY;
		}
		
		if (cartePerso[positionX][positionY+1]==6) {
			personnage->vie-=1;
			oldPositionX = positionX;
			oldPositionY = positionY;
			positionY += 1;
			return positionY;
		}
	
		else {
			oldPositionX = positionX;
			oldPositionY = positionY;
			positionY += 1;
		}
		setPeutAvancer(FALSE);
	}		
	return positionY;
			
}

int deplacementPersoBas(perso* personnage,int positionX,int positionY, bool peutAvancer){
	if(peutAvancer == TRUE) {
		if ((cartePerso[positionX][positionY-1]==1) || (cartePerso[positionX][positionY-1]==4)){
			return positionY;
		} 			
	
		if (cartePerso[positionX][positionY-1]==5) {
			personnage->vie-=1;
			return positionY;
		}
	
		if (cartePerso[positionX][positionY-1]==6) {
			personnage->vie-=1;
			oldPositionX = positionX;
			oldPositionY = positionY;
			positionY -= 1;
			return positionY;
		}
	
		else {
			oldPositionX = positionX;
			oldPositionY = positionY;
			positionY -= 1;
		}
		setPeutAvancer(FALSE);
	}
	return positionY;
}

int deplacementPersoGauche(perso* personnage,int positionX,int positionY, bool peutAvancer){
	if(peutAvancer == TRUE) {
	
		if ((cartePerso[positionX-1][positionY]==1) || (cartePerso[positionX-1][positionY]==4)) {
			return positionX;
		} 
	
		if (cartePerso[positionX-1][positionY]==5) {
			personnage->vie-=1;
			return positionX;
		}
	
		if (cartePerso[positionX-1][positionY]==6) {
			personnage->vie-=1;
			oldPositionX = positionX;
			oldPositionY = positionY;
			positionX -= 1;
			return positionX;
		}
		else {
			oldPositionX = positionX;
			oldPositionY = positionY;
			positionX -= 1;
		}
		setPeutAvancer(FALSE);
	}
	return positionX;
}

int deplacementPersoDroit(perso* personnage,int positionX, int positionY, bool peutAvancer){
	if(peutAvancer == TRUE) {

		if((cartePerso[positionX+1][positionY]==1) || (cartePerso[positionX+1][positionY]==4)) {
			return positionX;
		} 
	
		if (cartePerso[positionX+1][positionY]==5) {
			personnage->vie-=1;
			return positionX;
		}
		
		if (cartePerso[positionX+1][positionY]==6) {
			personnage->vie-=1;
			oldPositionX = positionX;
			oldPositionY = positionY;
			positionX += 1;
			return positionX;
		}
	
		else {
			oldPositionX = positionX;
			oldPositionY = positionY;
			positionX += 1;
		}
		setPeutAvancer(FALSE);
	}
	return positionX;
}
