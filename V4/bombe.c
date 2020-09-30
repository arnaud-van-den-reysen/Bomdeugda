#include <stdlib.h> // Pour pouvoir utiliser exit()
#include <stdio.h> // Pour pouvoir utiliser printf()
#include <math.h> // Pour pouvoir utiliser sin() et cos()
#include <stdbool.h>
#include <time.h>

#include "personnage.h"
#include "carte.h"

#define TRUE 1
#define FALSE 0

#define true 1
#define false 0

int carteBombe[18][18] = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
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

static int bombePositionX, bombePositionY;

static int nbBombe = 0;

static bool timer = FALSE;

static bool explobombe = FALSE;
static bool explosion = TRUE;

static clock_t start_tBombe = 0, end_tBombe = 0;
static double diff_tBombe = 0;

static clock_t start_tExplosion = 0, end_tExplosion = 0;
static double diff_tExplosion = 0;

static int oldbombePositionX;
static int oldbombePositionY;

static int positionX, positionY, viePerso;

static int vieNMI = 2;
static int* listspawnX;
static int* listspawnY;

static bool bombePose;
	
bool gestionAffichageBombe(perso* personnage, bool bombePose) {
	
	viePerso = getViePerso(personnage);

	getCarte(carteBombe);

	positionX = getXPerso(personnage);
	positionY = getYPerso(personnage);

	listspawnX = recherchePositionSpawnX();
	listspawnY = recherchePositionSpawnY();

	// tempo bombe
	if(explobombe == TRUE){
		if(explosion == TRUE) {
			start_tExplosion=clock();
			explosion = FALSE;
		}
		if(explosion == FALSE)
			end_tExplosion=clock();
	}
	diff_tExplosion=(double)(end_tExplosion-start_tExplosion) / CLOCKS_PER_SEC;
	if(diff_tExplosion>0.125){
	
		carteBombe[oldbombePositionX][oldbombePositionY]=0;	
		if (carteBombe[oldbombePositionX+1][oldbombePositionY]==6)
			carteBombe[oldbombePositionX+1][oldbombePositionY]=0;
			
		if (carteBombe[oldbombePositionX-1][oldbombePositionY]==6)
			carteBombe[oldbombePositionX-1][oldbombePositionY]=0;
				
		if (carteBombe[oldbombePositionX][oldbombePositionY+1]==6)
			carteBombe[oldbombePositionX][oldbombePositionY+1]=0;
						
		if (carteBombe[oldbombePositionX][oldbombePositionY-1]==6)
			carteBombe[oldbombePositionX][oldbombePositionY-1]=0;
										
		explosion = TRUE;
		explobombe = FALSE;
	}
	
	
	
	//position de la bombe
	if(bombePose == TRUE) {
		carteBombe[bombePositionX][bombePositionY]=4;
		
		if(timer == FALSE) {
			start_tBombe=clock();
			timer = TRUE;
		}
		if(timer == TRUE) {
			end_tBombe=clock();
		}
	}
	
	//affichage bombe
	diff_tBombe = (double)(end_tBombe-start_tBombe) / CLOCKS_PER_SEC;	
	if(diff_tBombe > 0.5) {
		carteBombe[bombePositionX][bombePositionY]=6;
		if (carteBombe[bombePositionX][bombePositionY] == carteBombe[positionX][positionY])
			viePerso-=1;
		if (carteBombe[bombePositionX+1][bombePositionY]!=1){
			if(carteBombe[bombePositionX+1][bombePositionY] == 3) /*gestion perte de vie perso par bombe*/
				viePerso-=1;
			if(carteBombe[bombePositionX+1][bombePositionY] == 5) /*gestion perte de vie adversaire par bombe*/
				vieNMI=vieNMI-1;
			carteBombe[bombePositionX+1][bombePositionY]=6;
		}
		if (carteBombe[bombePositionX-1][bombePositionY]!=1){
			if(carteBombe[bombePositionX-1][bombePositionY] == 3)
				viePerso-=1;
			if(carteBombe[bombePositionX-1][bombePositionY] == 5)
				vieNMI-=1;
			carteBombe[bombePositionX-1][bombePositionY]=6;
		}
		if (carteBombe[bombePositionX][bombePositionY+1]!=1){
			if(carteBombe[bombePositionX][bombePositionY+1] == 3)
				viePerso-=1;
			if(carteBombe[bombePositionX][bombePositionY+1] == 5)
				vieNMI-=1;
			carteBombe[bombePositionX][bombePositionY+1]=6;
		}
		if (carteBombe[bombePositionX][bombePositionY-1]!=1){
			if(carteBombe[bombePositionX][bombePositionY-1] == 3)
				viePerso-=1;
			if(carteBombe[bombePositionX][bombePositionY-1] == 5)
				vieNMI=vieNMI-1;
			carteBombe[bombePositionX][bombePositionY-1]=6;
		}
		bombePose = FALSE;
		timer = FALSE;
		end_tBombe=0;
		start_tBombe=0;
		explobombe=TRUE;
	}
				
	if (explobombe == FALSE){		
		oldbombePositionX=bombePositionX;
		oldbombePositionY=bombePositionY;
	}
	
	setViePersonnage(personnage,viePerso);
	setCarte(carteBombe);
	return bombePose;

}			

void poseBombe(int positionX, int positionY,int nbBombes){
	getCarte(carteBombe);
	if ((listspawnX[0]==positionX) && (listspawnY[0]==positionY))
		return;
	if ((listspawnX[1]==positionX) && (listspawnY[1]==positionY))
		return;
	if ((listspawnX[2]==positionX) && (listspawnY[2]==positionY))
		return;
	if ((listspawnX[3]==positionX) && (listspawnY[3]==positionY))
		return;
	//reset compteur nbBombe sur le terrain
	nbBombe=0;
	//recherche et compte le nbBombe sur le terrain
	for(int i = 0; i<18;i++) {
		for(int j = 0; j<18; j++) {
			if(carteBombe[i][j]==4) nbBombe += 1;
			
		}
	}
	//si le nombre de bombes posé est supérieur au nombre de bombes du joueur : break
	//sinon
	printf("nbBombe: %d\n",nbBombe);
	if(nbBombe >= nbBombes) {
		setPoseBombe(false);   
		return;
	} else {
		//bombePose = TRUE;
		bombePositionX = positionX;
		bombePositionY = positionY;
		carteBombe[positionX][positionY]=4;
		setCarte(carteBombe);
		setPoseBombe(true);
	}
	return;
}

void setPoseBombe(bool bP) {
	bombePose = bP;
}
