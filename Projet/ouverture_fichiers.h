#ifndef OUVERTURE_FICHIERS_H_INCLUDED
#define OUVERTURE_FICHIERS_H_INCLUDED

#include "interface.h"

void recuperer_chemin(GtkWidget *bouton, file_opener *donnees);
void creer_file_selection(file_opener *donnees);
void charger_fichiers(file_opener *donnees);
void conversion(file_opener *donnees);
void lancer_boite(GtkWidget *bouton, file_opener *donnees);
void combo_selected(GtkWidget *widget,file_opener *donnees);
void liberer_memoire(GtkWidget *bouton, file_opener *donnees);

void creer_file_save_selection(GtkWidget *bouton,file_opener *donnees);
void recuperer_save_chemin(GtkWidget *bouton, file_opener *donnees);
void sauver_fichiers(file_opener *donnees);

#endif // OUVERTURE_FICHIERS_H_INCLUDED
