#ifndef OUVERTURE_FICHIERS_H_INCLUDED
#define OUVERTURE_FICHIERS_H_INCLUDED

#include "interface.h"

void recuperer_chemin(GtkWidget *bouton, file_opener *donnees);
void creer_file_selection(file_opener *donnees);
void charger_fichiers(file_opener *donnees);
void lancer_boite(GtkWidget *bouton, file_opener *donnees);
void combo_selected(GtkWidget *widget,file_opener *donnees);

#endif // OUVERTURE_FICHIERS_H_INCLUDED
