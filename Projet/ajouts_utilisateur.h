#ifndef AJOUTS_UTILISATEUR_H_INCLUDED
#define AJOUTS_UTILISATEUR_H_INCLUDED

#include "interface.h"


void ajouter_plan_de_vol(GtkWidget* bouton,form_pdv* formulaire);
void ajouter_pt_pass(GtkWidget* bouton,form_pdv* formulaire);
void enlever_pt_pass(GtkWidget* bouton,form_pdv* formulaire);
void ajouter_pdv(GtkWidget* bouton,form_pdv* formulaire);
void select_pdv_ed(GtkWidget *bouton, form_pdv* formulaire);
void combo_selected_pdv(GtkWidget *widget,form_pdv* formulaire);

void select_pdv_rm(GtkWidget *bouton, form_pdv* formulaire);
void combo_rm_pdv(GtkWidget *widget,form_pdv* formulaire);
void rm_pdv(form_pdv* formulaire);
void creer_pdv_interactif(GtkWidget* but, form_pdv* formulaire);
void ajout_pt_pdv_interactif(form_pdv* formulaire,double x, double y);
void valider_pdv_interactif(GtkWidget* button, form_pdv* formulaire);
#endif // OUVERTURE_FICHIERS_H_INCLUDED
