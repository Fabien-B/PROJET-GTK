#include "ajouts_utilisateur.h"

void ajouter_plan_de_vol(GtkWidget *bouton, file_opener* donnees)
{
    GtkWidget *apdvw;
    GtkWidget* box;
    GtkWidget *scrollbar;
    GtkWidget *lab_nom;
    GtkWidget *lab_altitude;
    GtkWidget *nom_entry;
    GtkWidget *altitude_entry;


    //init fenêtre
    apdvw = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(GTK_WINDOW(apdvw), "Ajouter un plan de vol");
    gtk_window_set_default_size(GTK_WINDOW(apdvw), 10, 500);
    gtk_window_set_position(GTK_WINDOW(apdvw),GTK_WIN_POS_CENTER);

    //init scrollbar
    scrollbar = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(apdvw),scrollbar);

    //création et ajout d'une boite dans la fenètre scroll
    box=gtk_vbox_new(FALSE,0);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrollbar), box);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbar), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS); //Never: désactive la barre, Always, l'inverse



    lab_nom = gtk_label_new("Nom:");
    nom_entry= gtk_entry_new();
    lab_altitude = gtk_label_new("Niveau de vol:");
    altitude_entry= gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box), lab_nom, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), nom_entry, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), lab_altitude, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), altitude_entry, TRUE, FALSE, 0);










    gtk_widget_show_all(apdvw);  //afficher la fenètre

}
