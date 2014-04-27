#include <stdlib.h>
#include <gtk/gtk.h>
int main(int argc, char **argv)
{
    GtkWidget *pWindow;

    //
   // GtkWidget *box1;
    GtkWidget *pVBox;

    GtkWidget *pHBox;
    GtkWidget *pButton[4];
    gtk_init(&argc,&argv);
    pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(pWindow), "Jeu de Morpion");
    gtk_window_set_default_size(GTK_WINDOW(pWindow), 320, 400);
    g_signal_connect(G_OBJECT(pWindow), "destroy", G_CALLBACK(gtk_main_quit), NULL);




    /* Creation de la GtkBox verticale */
    pVBox = gtk_vbox_new(TRUE, 0);
    //box1 = gtk_hbox_new(TRUE, 0); //horizontale
    /* Ajout de la GtkVBox dans la fenetre */
    gtk_container_add(GTK_CONTAINER(pWindow), pVBox);
    /* Creation des boutons */
    pButton[0] = gtk_button_new_with_label("Bouton 0");
    pButton[1] = gtk_button_new_with_label("Bouton 1");
    pButton[2] = gtk_button_new_with_label("Bouton 2");
    pButton[3] = gtk_button_new_with_label("Bouton 3");
    /* Ajout de Bouton 1 dans la GtkVBox */
    gtk_box_pack_start(GTK_BOX(pVBox), pButton[0], TRUE, FALSE, 0);
    /* Creation de la box horizontale */
    pHBox = gtk_hbox_new(TRUE, 0);
    /* Ajout de la GtkHBox dans la GtkVBox */
    gtk_box_pack_start(GTK_BOX(pVBox), pHBox, TRUE, TRUE, 0);
    /* Ajout des boutons 2 et 3 dans la GtkHBox */
    gtk_box_pack_start(GTK_BOX(pHBox), pButton[1], TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(pHBox), pButton[2], TRUE, FALSE, 0);
    /* Ajout du dernier bouton dans la GtkVBox */
    gtk_box_pack_start(GTK_BOX(pVBox), pButton[3], TRUE, TRUE, 0);
    gtk_widget_show_all(pWindow);
    gtk_main();
    return EXIT_SUCCESS;
}
