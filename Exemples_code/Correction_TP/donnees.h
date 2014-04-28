#ifndef DONNEES_H
#define DONNEES_H
#include <stdlib.h>
#include <gtk/gtk.h>

#define MAXSIZE 40
// on pourrait gerer les plateaux avec des *** au lieu de *[][]
// mais ca serait plus difficile a lire

typedef enum { X, O, VIDE, } joueur;

typedef struct {
  // les elements de la vue qui seront un peu dynamiques
  GtkWidget *table;
  GtkWidget *b[MAXSIZE][MAXSIZE];
  GtkWidget *status;
  // le modele
  int taille;	// nombre de lignes / colonnes
  joueur plateau[MAXSIZE][MAXSIZE];
  joueur joueurcourant;
  int tour;	// nombre de coups joues
  int endgame;	// est-que la partie est terminee ?
  int victoire; // est-ce que quelqu'un a gagne ?
} donnees;

void initialiser(donnees *d,int taille,int victoire);	// retour au debut de partie
donnees *creer_donnees(int taille,int coupsgagnants);	// alloue la memoire et cree le plateau
void detruire_donnees(donnees *d);			// liberation de la memoire
void prochain_joueur(donnees *d);			// on change de joueur
int check_win(donnees *d,int jouei,int jouej);		// est-ce que quelqu'un a gagne ?

#endif
