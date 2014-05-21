
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
    GdkPixbuf *pPixBufA;
    GdkPixbuf *pPixBufB;
    gchar *sTexte;
    gint i;

    gtk_init(&argc, &argv);

    /* Creation de la fenetre principale */
    pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(pWindow), 320, 200);
    gtk_window_set_title(GTK_WINDOW(pWindow), "GtkTreeView et GtkTreeStore");
    g_signal_connect(G_OBJECT(pWindow), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    /* Creation du modele */
    pTreeStore = gtk_tree_store_new(N_COLUMN, GDK_TYPE_PIXBUF, G_TYPE_STRING);

    sTexte = g_malloc(16);

    /* Chargement des images */
    pPixBufA = gdk_pixbuf_new_from_file("./icon_computer.png", NULL);
    pPixBufB = gdk_pixbuf_new_from_file("./icon_directory.png", NULL);

    /* Insertion des elements */





        GtkTreeIter pIter11;
        GtkTreeIter pIter21;
        gint j;

        sprintf(sTexte, "Aérodromes");

        /* Creation de la nouvelle ligne */
        gtk_tree_store_append(pTreeStore, &pIter11, NULL);

        /* Mise a jour des donnees */
        gtk_tree_store_set(pTreeStore, &pIter11,
            BMP_COLUMN, pPixBufA,
            TEXT_COLUMN, sTexte,
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
            BMP_COLUMN, pPixBufA,
            TEXT_COLUMN, sTexte,
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

    g_object_unref(pPixBufA);
    g_object_unref(pPixBufB);

    /* Creation de la vue */
    pTreeView = gtk_tree_view_new_with_model(GTK_TREE_MODEL(pTreeStore));






    /* Creation de la première colonne*/
    pCellRenderer = gtk_cell_renderer_toggle_new();
    pColumn = gtk_tree_view_column_new_with_attributes("CheckBox",
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




    /* Ajout de la vue a la fenetre */
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
