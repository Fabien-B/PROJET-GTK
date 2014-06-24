#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED
#define XCARTE 550
#define YCARTE 660
#define DISTANCE_CONFLIT 20

typedef struct aerodrome{
    double latitude;
    double longitude;
    char nom[100];
    char oaci[5];
    int affichage;      //1 si affichage activé, 0 sinon
    GtkWidget* coch;
    struct aerodrome * ptsuiv;
}aerodrome;

typedef struct balise{
    char nom[10];
    double latdeg;
    int dirlat;         //1 si nord, -1 si sud
    double latitude;
    double longitude;
    int dirlong;        //1 si est, -1 si ouest
    int affichage;      //1 si affichage activé, 0 sinon
    GtkWidget* coch;
    struct balise * ptsuiv;
}balise;


typedef struct pt_pass{
    void *point;
    int type_point; // 0 aérodrome et 1 balise
    double temps;
    struct pt_pass * ptsuiv;
}pt_pass;

typedef struct pdv{
    char nom[100];
    int heure;
    int minute;
    int altitude;
    int vitesse;
    pt_pass *pass_debut;
    int affichage;      //1 si affichage activé, 0 sinon
    GtkWidget* coch;
    double temps_depart;
    double temps_arrivee;
    struct pdv * ptsuiv;
}pdv;

typedef struct conflit{
    pdv* pdv1;
    pdv* pdv2;
    double temps_deb;
    double temps_fin;
    double latitude;
    double longitude;
    struct conflit* ptsuiv;
}conflit;

typedef struct position{
    double x;
    double y;
}position;

typedef struct file_opener{
    char *ptchemin;
    GtkWidget *file_selection;
    int what_file;
    aerodrome *debutaero;
    balise *debutbalises;
    pdv *debutpdv;
    pdv *finpdv;
    int distance_conflit;
    double deltat_conflits;
    GtkWidget *carte;
    GtkWidget *heure_label;
    double temps;
    double latitude_max;
    double dlat;
    double longitude_min;
    double dlong;
    int xcarte;
    int ycarte;
    GtkWidget *Window;
    GtkWidget *mother_box;
    GtkWidget* boite;
    conflit *deb_conflits;
    position *start;
    position *bord;
    position *old;
    double clic_distance;
    GtkWidget *Msg_conflit;
    GtkWidget *Distance_clic;
    GtkWidget *Position_curseur;
    gint tag_lecture;
    GtkObject* adj2;

}file_opener;


typedef struct form_pdv{
    GtkWidget* wind;
    GtkWidget *nom_entry;
    char nom[10];
    GtkWidget *nom_label;
    GtkWidget * hour_box;
    GtkWidget * heure_label;
    GtkWidget * spinh;
    double heure;
    GtkWidget * spinm;
    double minutes;
    GtkWidget * spinfl;
    int altitude;
    GtkWidget * spinvi;
    int vitesse;
    GtkWidget *altitude_label;
    GtkWidget *vitesse_entry;
    GtkWidget *vitesse_label;
    GtkWidget *pass_entry[40];
    char pass[40][10];
    GtkWidget *pass_label[10];
    GtkWidget *ok_bouton;
    int nb_pt_int;
    file_opener* donnees;
}form_pdv;



void initialisation(int argc, char *argv[]);
void creer_interface(file_opener* donnees, form_pdv* formulaire);
void APropos(GtkWidget* widget);
void voir_pdv(GtkWidget *bouton, file_opener* donnees);
void parametres(GtkWidget* bouton, form_pdv* formulaire);
void redessiner(GtkWidget* button, GtkWidget * carte);
void recup_temps(GtkAdjustment* adj, file_opener* donnees);
void scroll_event(GtkWidget* carte,GdkEventScroll* event,file_opener* donnees);
void press_event(GtkWidget* carte, GdkEventButton* event, file_opener* donnees);
void drag_event(GtkWidget* carte, GdkEventMotion* event, file_opener* donnees);
void enter_event(GtkWidget* carte, GdkEventMotion* event, file_opener* donnees);


void rapide_file(GtkWidget * widget, file_opener * donnees);
void visu_carte_default(GtkWidget* button, form_pdv* formulaire);
void voir_conflits(GtkWidget *bouton, file_opener* donnees);

void my_getsize(GtkWidget *widget, GtkAllocation *allocation, form_pdv* formulaire);
void my_getsizecarte(GtkWidget *widget, GtkAllocation *allocation, void *data);
void my_getsizetemps(GtkWidget *widget, GtkAllocation *allocation, void *data);

gboolean animation(file_opener* donnees);
void play(GtkWidget* bouton,file_opener* donnees);
void stop(GtkWidget* bouton,file_opener* donnees);

#endif // INTERFACE_H_INCLUDED
