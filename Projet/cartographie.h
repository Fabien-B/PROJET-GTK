#ifndef CARTOGRAPHIE_H_INCLUDED
#define CARTOGRAPHIE_H_INCLUDED

#include "interface.h"

gboolean expose_cb (GtkWidget *canvas, GdkEventExpose *event, file_opener *donnees);
void dessiner(file_opener *donnees);
void couleur(GdkGC* gc,GdkColor c,int indice);
//position* Position_avion(file_opener* donnees,pdv* avion,position* loc);
void dessiner_pdv(pdv* pt_pdv_current,file_opener* donnees);
void dessiner_aerodromes(aerodrome* pt_aero,file_opener* donnees);
void dessiner_balise(balise* pt_balise,file_opener* donnees);


#endif // OUVERTURE_FICHIERS_H_INCLUDED

