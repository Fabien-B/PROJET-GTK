#include <stdlib.h>
#include "donnees.h"
#include "callbacks.h"

void affiche_tour(donnees *d){
  // met a jour le label pour indiquer le joueur courant
  char *s=(d->joueurcourant==X)?"joueur: X":"joueur: O";
  gtk_label_set_text(GTK_LABEL(d->status),s);
}


void creer_table(donnees *d) {
  int i,j;
  d->table=gtk_table_new(d->taille,d->taille,TRUE); // ici, si FALSE, il y aura des soucis
  for (i=0;i<d->taille;i++) for (j=0;j<d->taille;j++){
    d->b[i][j]=gtk_button_new_with_label(" ");
    // je leur donne un nom pour pouvoir faire des associations
    // et parametrer leur style dans le fichier ~/.gtkrc-2.0
    gtk_widget_set_name ( d->b[i][j] , "morpion" );
    gtk_table_attach_defaults(GTK_TABLE(d->table),d->b[i][j],i,i+1,j,j+1);
    g_signal_connect(G_OBJECT(d->b[i][j]),"clicked",G_CALLBACK(joue),d);
  }
}

void initialiser(donnees *d,int taille, int coupsgagnants){
  static GdkColor c;
  gdk_color_parse("white",&c);
  int i,j;
  GtkWidget *box;
  d->joueurcourant=X;
  d->endgame=0;
  d->tour=0;
  if (d->table==NULL)
    creer_table(d);
  else if (d->taille!=taille) {
    // il faut remplacer la table dans le vbox des parents ...
    d->taille=taille;
    box=gtk_widget_get_parent(d->table);
    gtk_widget_ref(d->status);
    gtk_container_remove(GTK_CONTAINER(box),d->table);
    gtk_container_remove(GTK_CONTAINER(box),d->status);
    creer_table(d);
    gtk_widget_show_all(d->table);
    gtk_box_pack_start(GTK_BOX(box),d->table,TRUE,TRUE,0);
    gtk_box_pack_start(GTK_BOX(box),d->status,TRUE,TRUE,0);
    gtk_widget_unref(d->status);
  }
  for (i=0;i<d->taille;i++) for (j=0;j<d->taille;j++) {
    // la vue
    gtk_button_set_label(GTK_BUTTON(d->b[i][j])," ");
    gtk_widget_modify_bg(d->b[i][j],GTK_STATE_NORMAL,&c);
    affiche_tour(d);
    // le modele TODO
    d->plateau[i][j]=VIDE;
  }
  affiche_tour(d);
}

donnees *creer_donnees(int taille,int coupsgagnants){
  donnees *d = (donnees*)malloc(sizeof(donnees));
  if (taille>MAXSIZE){
    fprintf(stderr,"et je perds la memoire\n");
    exit(-1);
  }
  d->taille=taille;
  d->victoire=coupsgagnants;
  d->table=NULL;
  d->status=gtk_label_new("status");
  gtk_widget_set_name( d->status , "labelmorpion" );
  initialiser(d,taille,coupsgagnants);
  return d;
}

void detruire_donnees(donnees *d){
  free(d);
}

int compte(donnees *d,int x,int y,int dx,int dy) {
  int total=0;
  // depuis (x,y) en avancant sur le vecteur (dx,dy)
  // combien y a-t-il de d->joueurcourant ?
  while( (x<d->taille) && (x>=0) && (y<d->taille) &&(y>=0)) {
    if (d->plateau[x][y] != d->joueurcourant) return total;
    x+=dx;
    y+=dy;
    total++;
  }
  return total;
}

void decore(donnees *d,int x,int y,int dx,int dy) {
  static GdkColor c;
  gdk_color_parse("yellow",&c);
  do {
    gtk_widget_modify_bg(d->b[x][y],GTK_STATE_NORMAL,&c);
    x+=dx;
    y+=dy;
  } while ( (x<d->taille) && (x>=0) && (y<d->taille) && (y>=0) && (d->plateau[x][y] == d->joueurcourant));
}

int alignement_gagnant(donnees *d,int x,int y,int dx,int dy) {
  //Ma fonction compte comptabilise la case courante
  // du coup, avec la somme, elle compte deux fois
  // une fois dans le vecteur, une fois dans l'autre sens
  int combien = (compte(d,x,y,dx,dy)+compte(d,x,y,-dx,-dy)) ;
  if ( combien > d->victoire) {
    decore(d,x,y,dx,dy);
    decore(d,x,y,-dx,-dy);
    return 1;
  }
  return 0;
}

int check_win(donnees *d,int jouei,int jouej) {
  char s[1024];
  // compte le nombre de jetons alignés sur 4 axes ( ligne, colonne, deux
  // diagonales.
  if ( alignement_gagnant(d,jouei,jouej,1,0) || alignement_gagnant(d,jouei,jouej,0,1)
    || alignement_gagnant(d,jouei,jouej,1,1) || alignement_gagnant(d,jouei,jouej,1,-1) ){
    sprintf(s,"VICTOIRE de %s !!!",(d->joueurcourant==X)?"X":"0");
    gtk_label_set_text(GTK_LABEL(d->status),s);
    d->endgame=1;
  }
  return d->endgame; // sinon, il reste a 0
}

void prochain_joueur(donnees *d){
  d->tour++;
  // verifie si la partie est terminee
  if (d->tour==d->taille*d->taille) {
    d->endgame=1;
    gtk_label_set_text(GTK_LABEL(d->status),"guerre thermonucleaire annulee");
  } else {
    d->joueurcourant=(d->joueurcourant==X)?O:X;
    affiche_tour(d);
  }
}
