#ifndef CALLBACKS_H
#define CALLBACKS_H
#include "donnees.h"
#include <gtk/gtk.h>

void new_game(GtkWidget *widget, donnees *d);	// une nouvelle partie
void joue(GtkWidget *widget, donnees *d);	// un bouton a ete enfonce

#endif
