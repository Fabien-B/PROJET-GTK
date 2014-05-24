#include <stdlib.h>
#include <gtk/gtk.h>

void OnToggle(GtkWidget *pToggle, gpointer data);
void OnEtatBtn(GtkWidget *pWidget, gpointer pToggle);
void OnAspectBtn(GtkWidget *pWidget, gpointer pToggle);

int main(int argc, char **argv)
{
    GtkWidget *pWindow;
    GtkWidget *pToggleBtn;

    GtkWidget *pVBox;
    gchar *sLabel;

    gtk_init(&argc,&argv);

    pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(pWindow), "GtkToggleButton");
    gtk_window_set_default_size(GTK_WINDOW(pWindow), 320, 200);

    pVBox = gtk_vbox_new(TRUE, 0);
    gtk_container_add(GTK_CONTAINER(pWindow), pVBox);

    /* Création du label du bouton */
    sLabel = g_locale_to_utf8("Etat : Relâché - Aspect : Normal", -1, NULL, NULL, NULL);
    /* Création du bouton GtkToggleButton */
    pToggleBtn = gtk_check_button_new_with_label(sLabel);
    //pToggleBtn = gtk_check_button_new();
    /* Le label sLabel n'est plus utile */
    g_free(sLabel);

    gtk_box_pack_start(GTK_BOX(pVBox), pToggleBtn, FALSE, FALSE, 0);


    gtk_widget_show_all(pWindow);

    /* Connexion des signaux */
   // g_signal_connect(G_OBJECT(pToggleBtn), "toggled", G_CALLBACK(OnToggle), NULL);

    g_signal_connect(G_OBJECT(pWindow), "destroy", G_CALLBACK(gtk_main_quit), NULL);


    gtk_main();

    return EXIT_SUCCESS;
}

void OnEtatBtn(GtkWidget *pWidget, gpointer pToggle)
{
    gboolean bEtat;


    bEtat = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pToggle));


    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pToggle), (bEtat ^ TRUE));
}

void OnAspectBtn(GtkWidget *pEtatBtn, gpointer pToggle)
{
    gboolean bInconsistent;

    // Récupération de l aspect du bouton
    bInconsistent = gtk_toggle_button_get_inconsistent(GTK_TOGGLE_BUTTON(pToggle));

    // Modification de l'aspect du bouton
    gtk_toggle_button_set_inconsistent(GTK_TOGGLE_BUTTON(pToggle), (bInconsistent ^ TRUE));

    // On émet le signal "toggle" pour changer le texte du bouton /
    gtk_toggle_button_toggled(GTK_TOGGLE_BUTTON(pToggle));
}

void OnToggle(GtkWidget *pToggle, gpointer data)
{
    gboolean bEtat;
    gboolean bInconsistent;
    gchar *sLabel;
    gchar *sLabelUtf8;

    // Récupération de l'état du bouton
    bEtat = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pToggle));
    // Recuperation de l aspect du bouton
    bInconsistent = gtk_toggle_button_get_inconsistent(GTK_TOGGLE_BUTTON(pToggle));

    // Construction du label du bouton
    sLabel = g_strdup_printf("Etat : %s - Aspect : %s",
        bEtat ? "Enfoncé" : "Relâché",
        bInconsistent ? "Modifié" : "Normal");
    // Encodage du label en UTF8
    sLabelUtf8 = g_locale_to_utf8(sLabel, -1, NULL, NULL, NULL);

    // Modification du label du bouton
    gtk_button_set_label(GTK_BUTTON(pToggle), sLabelUtf8);

    // Les chaînes sLabel et sLabelUtf8 n'ont plus d'utilité
    g_free(sLabel);
    g_free(sLabelUtf8);
}
