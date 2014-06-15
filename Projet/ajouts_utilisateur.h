#ifndef AJOUTS_UTILISATEUR_H_INCLUDED
#define AJOUTS_UTILISATEUR_H_INCLUDED

#include "interface.h"

/*typedef struct form_pdv{
    GtkWidget* wind;
    GtkWidget *nom_entry;
    char nom[10];
    GtkWidget *nom_label;
    GtkWidget * hour_box;
    GtkWidget * heure_label;
    GtkWidget * spinh;
    double heure;
    GtkWidget * spinm;
    double minutes;
    GtkWidget *altitude_entry;
    char altitude[5];
    GtkWidget *altitude_label;
    GtkWidget *vitesse_entry;
    char vitesse[5];
    GtkWidget *vitesse_label;
    GtkWidget *pass_entry[20];
    char pass[20][20];
    GtkWidget *pass_label[20];
    GtkWidget *ok_bouton;
    int nb_pt_int;
    file_opener* donnees;
}form_pdv;*/

void ajouter_plan_de_vol(GtkWidget* bouton,form_pdv* formulaire);
void ajouter_pt_pass(GtkWidget* bouton,form_pdv* formulaire);
void ajouter_pdv(GtkWidget* bouton,form_pdv* formulaire);

#endif // OUVERTURE_FICHIERS_H_INCLUDED
