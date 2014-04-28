#include "callbacks.h"

#ifdef TAILLE_FIXE
/* version plus simplifiée, en restant en 3x3 */
void new_game(GtkWidget *widget, donnees *d){ initialiser(d); }
#endif

void taillecb(GtkWidget *widget, int *d){
  int i = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(widget));
  if ((i>=3)&&(i<MAXSIZE)) *d=i;
}

void new_game(GtkWidget *widget, donnees *d){
  GtkWidget *dialog, *content_area, *w;
  GtkWidget *taille_spin, *victoire_spin, *box;
  static int new_taille, new_victoire;
  gint reponse;
  new_taille=d->taille;
  new_victoire=d->victoire;
  w=gtk_widget_get_parent(gtk_widget_get_parent(d->table));
  dialog = gtk_dialog_new_with_buttons ("nouvelle partie",
      GTK_WINDOW(w),
      //GTK_DIALOG_DESTROY_WITH_PARENT,
      GTK_DIALOG_MODAL,
      GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
      GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT,
      NULL);
  //content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
  box=gtk_hbox_new(FALSE,10);
  //gtk_container_add (GTK_CONTAINER (content_area), box);
  gtk_box_pack_start(GTK_DIALOG(dialog)->vbox,box,TRUE,TRUE,10);
  taille_spin=gtk_spin_button_new_with_range(1,MAXSIZE,1);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(taille_spin),d->taille);
  victoire_spin=gtk_spin_button_new_with_range(1,MAXSIZE,1);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(victoire_spin),d->victoire);
  gtk_box_pack_start( GTK_BOX(box) , gtk_label_new("taille:") ,FALSE,FALSE,0);
  gtk_box_pack_start( GTK_BOX(box) , taille_spin ,TRUE,TRUE,0);
  gtk_box_pack_start( GTK_BOX(box) , gtk_label_new("victoire:") ,FALSE,FALSE,0);
  gtk_box_pack_start( GTK_BOX(box) , victoire_spin,TRUE,TRUE,0);
  g_signal_connect( G_OBJECT(taille_spin), "value-changed", G_CALLBACK(taillecb), &new_taille );
  g_signal_connect( G_OBJECT(victoire_spin), "value-changed", G_CALLBACK(taillecb), &new_victoire );
  gtk_widget_show_all(box);
  reponse=gtk_dialog_run(GTK_DIALOG(dialog));
  switch (reponse) {
    case GTK_RESPONSE_ACCEPT:
      // si il y a changement de paramètres ( taille, etc. ), il faut tout
      // refaire, enlever le plateau, etc.
      g_print("%d %d",new_taille,new_victoire);
      initialiser(d,new_taille,new_victoire);
      break;
    case GTK_RESPONSE_REJECT:
      // annule, rien a faire
      break;
    default:
      // dialogue annulle
      g_print("retour bizzare: %d\n",reponse);
      break;
  }
  gtk_widget_destroy(dialog);
}

void maj_bouton(GtkWidget *bouton,joueur j){
  gtk_button_set_label(GTK_BUTTON(bouton),(j==X)?"X":"O");
}

void joue(GtkWidget *widget, donnees *d){
  int i=0,j=0;
  char s[1024];
  if (d->endgame) return;
  // retrouver le bouton ou le coup a été joué
  while (1) {
    if (widget==d->b[i][j]) break;
    i++;
    if (i==d->taille) {i=0;j++;}
  }
  // est-ce que la case est vide ?
  if (d->plateau[i][j]!=VIDE) {
    sprintf(s,"case (%d,%d) occupee, %s",i,j,((d->joueurcourant==X)?"joueur: X":"joueur: O"));
    gtk_label_set_text(GTK_LABEL(d->status),s);
  } else {
    // met a jour la vue
    maj_bouton(d->b[i][j],d->joueurcourant);
    // met a jour le modele
    d->plateau[i][j]=d->joueurcourant;
    if (!check_win(d,i,j)) prochain_joueur(d);
  }
}
