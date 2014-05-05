#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>
#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED
//#include "interface.c"

/*typedef struct{
char *chemin_aerodromes;
char *chemin_balises;
char *vols;
int what_file;
GtkWidget *file_selection;
}file_pack;*/

typedef struct{
char *ptchemin;
GtkWidget *file_selection;
int what_file;
}file_opener;


void init_interface();
void APropos(GtkWidget* widget);
void recuperer_chemin(GtkWidget *bouton, file_opener *donnees);
void creer_file_selection(file_opener *donnees);
void charger_fichiers(file_opener *donnees);
void lancer_boite(GtkWidget *bouton, file_opener *donnees);
void combo_selected(GtkWidget *widget,file_opener *donnees);





#endif // INTERFACE_H_INCLUDED
