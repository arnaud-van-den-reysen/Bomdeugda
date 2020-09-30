typedef struct PERSO perso;

void affichagePerso(int positionX, int positionY);
perso *initialisationPerso(perso *personnage);
void DesaffichagePersonnageOldPosition(int, int);
	

int deplacementPersoHaut(perso* personnage,int positionX,int positionY, bool peutAvancer);
int deplacementPersoBas(perso* personnage,int positionX,int positionY, bool peutAvancer);
int deplacementPersoDroit(perso* personnage,int positionX,int positionY, bool peutAvancer);
int deplacementPersoGauche(perso* personnage,int positionX,int positionY, bool peutAvancer);

int getYPerso(perso* personnage);
int getXPerso(perso* personnage);
int getViePerso(perso* personnage);
int getnbBombesPerso(perso* personnage);
double getVitessePerso(perso* personnage);

void setXPersonnage(perso* personnage,int positionX);
void setYPersonnage(perso* personnage,int positionY);
void setViePersonnage(perso* personnage,int viePerso);
void setNbBombesPersonnage(perso* personnage,int NbBombePersonnage);
void setVitessePersonnage(perso* personnage, double Vitesse);

int getOldPositionX();
int getOldPositionY();

bool vitessePerso(perso*,bool peutAvancer);

void setPeutAvancer(bool);
bool getPeutAvancer();


