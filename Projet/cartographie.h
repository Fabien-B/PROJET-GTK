#ifndef CARTOGRAPHIE_H_INCLUDED
#define CARTOGRAPHIE_H_INCLUDED

#include "interface.h"

gboolean expose_cb (GtkWidget *canvas, GdkEventExpose *event, file_opener *donnees);
void dessiner(GdkDrawable* carte, GdkGC *gc, file_opener *donnees);


#endif // OUVERTURE_FICHIERS_H_INCLUDED

