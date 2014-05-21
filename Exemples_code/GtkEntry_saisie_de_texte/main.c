
#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

enum {
    TEXT_COLUMN,
    TOGGLE_COLUMN,
    N_COLUMN
};

int main(int argc, char **argv)
{
    GtkWidget *pWindow;
    GtkWidget *pListView;
    GtkWidget *pScrollbar;
    GtkListStore *pListStore;
    GtkTreeViewColumn *pColumn;
    GtkCellRenderer *pCellRenderer;
    gchar *sTexte;
    gint i;

    gtk_init(&argc, &argv);

    /* Creation de la fenetre principale */
    pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(pWindow), 320, 200);
    gtk_window_set_title(GTK_WINDOW(pWindow), "GtkTreeView et GtkListStore");
    g_signal_connect(G_OBJECT(pWindow), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    /* Creation du modele */
    pListStore = gtk_list_store_new(N_COLUMN, G_TYPE_STRING, G_TYPE_BOOLEAN);

    sTexte = g_malloc(12);

    /* Insertion des elements */
    for(i = 0 ; i < 10 ; ++i)
    {
        GtkTreeIter pIter;

        sprintf(sTexte, "Ligne %d\0", i);

        /* Creation de la nouvelle ligne */
        gtk_list_store_append(pListStore, &pIter);

        /* Mise a jour des donnees */
        gtk_list_store_set(pListStore, &pIter,
            TEXT_COLUMN, sTexte,
            TOGGLE_COLUMN, TRUE,
            -1);
    }

    g_free(sTexte);

    /* Creation de la vue */
    pListView = gtk_tree_view_new_with_model(GTK_TREE_MODEL(pListStore));

    /* Creation de la premiere colonne */
    pCellRenderer = gtk_cell_renderer_text_new();
    pColumn = gtk_tree_view_column_new_with_attributes("Titre",
        pCellRenderer,
        "text", TEXT_COLUMN,
        NULL);

    /* Ajout de la colonne à la vue */
    gtk_tree_view_append_column(GTK_TREE_VIEW(pListView), pColumn);

    /* Creation de la deuxieme colonne */
    pCellRenderer = gtk_cell_renderer_toggle_new();
    pColumn = gtk_tree_view_column_new_with_attributes("CheckBox",
        pCellRenderer,
        "active", TOGGLE_COLUMN,
        NULL);

    /* Ajout de la colonne à la vue */
    gtk_tree_view_append_column(GTK_TREE_VIEW(pListView), pColumn);

    /* Ajout de la vue a la fenetre */
    pScrollbar = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(pScrollbar),
        GTK_POLICY_AUTOMATIC,
        GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(pScrollbar), pListView);
    gtk_container_add(GTK_CONTAINER(pWindow), pScrollbar);

    gtk_widget_show_all(pWindow);

    gtk_main();

    return EXIT_SUCCESS;
}
