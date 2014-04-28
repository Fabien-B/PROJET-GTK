#include <gtk/gtk.h>
#include "donnees.h"
#include "callbacks.h"

/* taille par defaut du damier: 3x3, et 3 alignes sont gagnants */
#define TAILLE 3
#define VICTOIRE 3


int main(int argc,char *argv[]){
  GtkWidget *fenetre,*hbox,*vbox,*newp,*quit;
  donnees *d;
  gtk_init(&argc,&argv);

  /* creation des widgets, gestion de la geometrie */
  d=creer_donnees(TAILLE,VICTOIRE);
  fenetre=gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(fenetre),"jeu de morpion");
  gtk_window_set_default_size(GTK_WINDOW(fenetre),200,250);
  vbox = gtk_vbox_new(FALSE,10);
  gtk_container_add(GTK_CONTAINER(fenetre),vbox);
  hbox = gtk_hbox_new(FALSE,10);
  gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,0);
  gtk_box_pack_start(GTK_BOX(vbox),d->table,TRUE,TRUE,0);
  gtk_box_pack_start(GTK_BOX(vbox),d->status,FALSE,TRUE,0);
  newp=gtk_button_new_with_label("nouvelle partie");
  quit=gtk_button_new_with_label("quitter");
  gtk_box_pack_start(GTK_BOX(hbox),newp,FALSE,FALSE,0);
  gtk_box_pack_end(GTK_BOX(hbox),quit,FALSE,FALSE,0);
  gtk_widget_show_all(fenetre);

  /* ajout du comportement */
  g_signal_connect(G_OBJECT(fenetre),"delete-event",G_CALLBACK(gtk_main_quit),NULL);
  g_signal_connect(G_OBJECT(quit),"clicked",G_CALLBACK(gtk_main_quit),NULL);
  g_signal_connect(G_OBJECT(newp),"clicked",G_CALLBACK(new_game),d);

  /* boucle principale */
  gtk_main();
  detruire_donnees(d);
  return 0;
}
