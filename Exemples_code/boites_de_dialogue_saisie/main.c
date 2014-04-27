#include <stdlib.h>
#include <gtk/gtk.h>
static GtkWidget *pLabel;
static GtkWidget *pWindow;
void LancerBoite(void);
int main(int argc, char **argv)
{
    GtkWidget *pVBox;
    GtkWidget *pButton;

    gtk_init(&argc, &argv);
    pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(pWindow), "GtkDialog");
    gtk_window_set_default_size(GTK_WINDOW(pWindow), 320, 200);
    g_signal_connect(G_OBJECT(pWindow), "destroy", G_CALLBACK(gtk_main_quit),
NULL);
    pVBox = gtk_vbox_new(TRUE, 0);
    gtk_container_add(GTK_CONTAINER(pWindow), pVBox);
    pButton = gtk_button_new_with_label("Cliquez ici pour saisir votre nom");
    gtk_box_pack_start(GTK_BOX(pVBox), pButton, FALSE, TRUE, 0);
    pLabel = gtk_label_new(NULL);
    gtk_box_pack_start(GTK_BOX(pVBox), pLabel, FALSE, FALSE, 0);
    /* Connexion du signal "clicked" pour ouvrir la boite de dialogue */
    g_signal_connect(G_OBJECT(pButton), "clicked", G_CALLBACK(LancerBoite),
NULL);
    gtk_widget_show_all(pWindow);
    gtk_main();
    return EXIT_SUCCESS;
}
void LancerBoite(void)
{
    GtkWidget* pBoite;
    GtkWidget* pEntry;
    const gchar* sNom;
    /* Creation de la boite de dialogue */
    /* 1 bouton Valider */
    /* 1 bouton Annuler */
    pBoite = gtk_dialog_new_with_buttons("Saisie du nom",
        GTK_WINDOW(pWindow),
        GTK_DIALOG_MODAL,
        GTK_STOCK_OK,GTK_RESPONSE_OK,
        GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,
        NULL);
    /* Creation de la zone de saisie */
    pEntry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(pEntry), "Saisissez votre nom");
    /* Insertion de la zone de saisie dans la boite de dialogue */
    /* Rappel : paramètre 1 de gtk_box_pack_start de type GtkBox */
    gtk_box_pack_start(GTK_BOX(pWindow­), pEntry, TRUE, FALSE,0);
    /* Affichage des elements de la boite de dialogue */
    gtk_widget_show_all(GTK_DIALOG(pBoite)­>vbox);
    /* On lance la boite de dialogue et on recupere la reponse */
    switch (gtk_dialog_run(GTK_DIALOG(pBoite)))
    {
        /* L utilisateur valide */
        case GTK_RESPONSE_OK:
Page 62/161Gtk-Fr
Cours Gtk+
            sNom = gtk_entry_get_text(GTK_ENTRY(pEntry));
            gtk_label_set_text(GTK_LABEL(pLabel), sNom);
            break;
        /* L utilisateur annule */
        case GTK_RESPONSE_CANCEL:
        case GTK_RESPONSE_NONE:
        default:
            gtk_label_set_text(GTK_LABEL(pLabel), "Vous n'avez rien saisi !");
            break;
    }
    /* Destruction de la boite de dialogue */
    gtk_widget_destroy(pBoite);
}
