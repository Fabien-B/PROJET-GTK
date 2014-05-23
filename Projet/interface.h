#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>
#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED


typedef struct aerodrome{
    double latitude;
    double longitude;
    char nom[20];
    char oaci[4];
    int affichage;      //1 si affichage activé, 0 sinon <=====> ne serviras peut être à rien, info à récupérer sur la case à cocher quand elle seras là.
    int pos_x;          //positions x et y sur la zone de dessin, à recalculer quand la zone change de taille
    int pos_y;
    GtkWidget* coch;
    struct aerodrome * ptsuiv;
}aerodrome;

typedef struct balise{
    char nom[10];
    int latdeg;
    int latmin;
    double latsec;
    int dirlat;         //1 si nord, -1 si sud
    int longdeg;
    int longmin;
    double longsec;
    int dirlong;        //1 si est, -1 si ouest
    int pos_x;          //positions x et y sur la zone de dessin, à recalculer quand la zone change de taille
    int pos_y;
    GtkWidget* coch;
    struct balise * ptsuiv;
}balise;

typedef struct file_opener{
char *ptchemin;
GtkWidget *file_selection;
int what_file;
aerodrome *debutaero;
balise * debutbalises;
}file_opener;

void init_interface();
void APropos(GtkWidget* widget);






#endif // INTERFACE_H_INCLUDED
