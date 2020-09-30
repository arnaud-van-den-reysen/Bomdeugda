#include <stdlib.h> // Pour pouvoir utiliser exit()
#include <stdio.h> // Pour pouvoir utiliser printf()
#include <math.h> // Pour pouvoir utiliser sin() et cos()
#include "GfxLib.h" // Seul cet include est necessaire pour faire du graphique
#include "BmpLib.h" // Cet include permet de manipuler des fichiers BMP
#include "ESLib.h" // Pour utiliser valeurAleatoire()

static DonneesImageRGB *menu = NULL; // L'image a afficher au centre de l'ecran
static DonneesImageRGB *curseur = NULL; // L'image a afficher pour le curseur

//gestion du curseur
static int count=1;

static int var;

//Dans initialisation
void initialisationMenu() {
	menu = lisBMPRGB("images/menu.bmp");
	curseur = lisBMPRGB("images/curseur.bmp");
}

//Dans affichage
void affichageMenu() {
	// On part d'un fond d'ecran blanc
	effaceFenetre (39, 44, 101);

	// Affichage d'une image
	if (menu != NULL) // Si l'image a pu etre lue
	{
		// On affiche l'image
		ecrisImage(0, 0, menu->largeurImage, menu->hauteurImage, menu->donneesRGB);
	}

	//deplacement du curseur
	if (curseur != NULL)
	{
		switch(count)
		{
			case 1:
				ecrisImage(0, 540, curseur->largeurImage, curseur->hauteurImage, curseur->donneesRGB);
				break;
			case 2:
				ecrisImage(0, 360, curseur->largeurImage, curseur->hauteurImage, curseur->donneesRGB);					
				break;
			case 3:
				ecrisImage(0, 200, curseur->largeurImage, curseur->hauteurImage, curseur->donneesRGB);
				break;
			case 4:
				ecrisImage(0, 25, curseur->largeurImage, curseur->hauteurImage, curseur->donneesRGB);
				break;
		}
	}
}

//Sur touche Entree
void sortieMenu() {
	if(count==4)
	{
		libereDonneesImageRGB(&menu); /* On libere la structure image,
		c'est plus propre, meme si on va sortir du programme juste apres */
		libereDonneesImageRGB(&curseur);					
		termineBoucleEvenements();
	}
}

//lance un partie
int lancementJeu() {
	if(count==1) {
		var = 1;
	}
	return var;
}

//Sur touche Z
void curseurHaut() {
	if(count==1)
	{
		count=4;
	}
	else count--;
}

//Sur touche S
void curseurBas() {
	if(count==4) 
	{
		count=1;
	}
	else count++;
}




























