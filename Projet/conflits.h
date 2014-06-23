#ifndef CONFLITS_H_INCLUDED
#define CONFLITS_H_INCLUDED

#include "interface.h"


void detection_conflits(GtkWidget *button,file_opener * donnees);
void get_position_avion(position* pos, pdv* pdv_c,double t);

void integrer_temps(file_opener* donnees);
double conversion_lat(double latitude, file_opener *donnees);
double conversion_longitude(double longitude, file_opener *donnees);


#endif // CONFLITS_H_INCLUDED
