
#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

enum {
    BMP_COLUMN,
    TEXT_COLUMN,
    N_COLUMN,
    TOGGLE_COLUMN
};

/* Utilisateurs Visual C++ : il faut ajouter gdk_pixbuf-2.0.lib dans les options du linker */

int main(int argc, char **argv)
{
    GtkWidget *pWindow;
    GtkWidget *pTreeView;
    GtkWidget *pScrollbar;
    GtkTreeStore *pTreeStore;
    GtkTreeViewColumn *pColumn;
    GtkCellRenderer *pCellRenderer;

    gchar *sTexte;


    gtk_init(&argc, &argv);

    /* Creation de la fenetre principale */
    pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(pWindow), 320, 200);
    gtk_window_set_title(GTK_WINDOW(pWindow), "GtkTreeView et GtkTreeStore");
    g_signal_connect(G_OBJECT(pWindow), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    /* Creation du modele */
    pTreeStore = gtk_tree_store_new(N_COLUMN, GDK_TYPE_PIXBUF, G_TYPE_STRING);

    sTexte = g_malloc(16);






        GtkTreeIter pIter11;
        GtkTreeIter pIter21;
        gint j;

        sprintf(sTexte, "Aérodromes");

        /* Creation de la nouvelle ligne */
        gtk_tree_store_append(pTreeStore, &pIter11, NULL);


            gtk_tree_store_set(pTreeStore, &pIter11,
            TEXT_COLUMN, sTexte,
            TOGGLE_COLUMN, TRUE,
            -1);


        for(j = 0 ; j < 2 ; ++j)
        {
            sprintf(sTexte, "Repertoire %d", j);

            /* Creation de la nouvelle ligne enfant */
            gtk_tree_store_append(pTreeStore, &pIter21, &pIter11);



                gtk_tree_store_set(pTreeStore, &pIter21,
            TEXT_COLUMN, sTexte,
            TOGGLE_COLUMN, TRUE,
            -1);
        }




        GtkTreeIter pIter12;
        GtkTreeIter pIter22;

        sprintf(sTexte, "Balises");

        /* Creation de la nouvelle ligne */
        gtk_tree_store_append(pTreeStore, &pIter12, NULL);

        /* Mise a jour des donnees */
        gtk_tree_store_set(pTreeStore, &pIter12,
            TEXT_COLUMN, sTexte,
            TOGGLE_COLUMN, TRUE,
            -1);

        for(j = 0 ; j < 2 ; ++j)
        {
            sprintf(sTexte, "Repertoire %d", j);

            /* Creation de la nouvelle ligne enfant */
            gtk_tree_store_append(pTreeStore, &pIter22, &pIter12);



                gtk_tree_store_set(pTreeStore, &pIter22,
            TEXT_COLUMN, sTexte,
            TOGGLE_COLUMN, TRUE,
            -1);
        }




    g_free(sTexte);



    /* Creation de la vue */
    pTreeView = gtk_tree_view_new_with_model(GTK_TREE_MODEL(pTreeStore));






    /* Creation de la première colonne*/
    pCellRenderer = gtk_cell_renderer_toggle_new();
    pColumn = gtk_tree_view_column_new_with_attributes("coucou!",
        pCellRenderer,
        "active", TOGGLE_COLUMN,
        NULL);

    /* Ajout de la colonne à la vue */
    gtk_tree_view_append_column(GTK_TREE_VIEW(pTreeView), pColumn);





    /* Creation de la deuxième colonne */

            pCellRenderer = gtk_cell_renderer_text_new();
    pColumn = gtk_tree_view_column_new_with_attributes("Label",
        pCellRenderer,
        "text", TEXT_COLUMN,
        NULL);





    /* Ajout de la colonne à la vue */
    gtk_tree_view_append_column(GTK_TREE_VIEW(pTreeView), pColumn);









//    g_signal_connect(G_OBJECT(pToggleBtn), "toggled", G_CALLBACK(OnToggle), NULL);








//     Ajout de la vue a la fenetre
    pScrollbar = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(pScrollbar),
        GTK_POLICY_AUTOMATIC,
        GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(pScrollbar), pTreeView);
    gtk_container_add(GTK_CONTAINER(pWindow), pScrollbar);

    gtk_widget_show_all(pWindow);

    gtk_main();

    return EXIT_SUCCESS;
}




/*
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
*/
