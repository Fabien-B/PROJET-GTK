#ifndef FILTRAGE_H_INCLUDED
#define FILTRAGE_H_INCLUDED

#include "interface.h"

void filtres(GtkWidget* button, file_opener* donnees);
void select_all_aero(GtkWidget* button, file_opener* donnees);
void select_all_balises(GtkWidget* button, file_opener* donnees);
void select_all_pdv(GtkWidget* button, file_opener* donnees);
void invert_selection_aero(GtkWidget* button, file_opener* donnees);
void invert_selection_balise(GtkWidget* button, file_opener* donnees);
void invert_selection_pdv(GtkWidget* button, file_opener* donnees);
void check_aero(GtkWidget *pToggle, aerodrome* pt_current);
void check_balise(GtkWidget *pToggle, balise* pt_current);
void check_pdv(GtkWidget *pToggle, pdv* pt_current);

void detection_conflits(GtkWidget *button,file_opener * donnees);
void get_position_avion(position* pos, pdv* pdv_c,double t);
#endif // FILTRAGE_H_INCLUDED
