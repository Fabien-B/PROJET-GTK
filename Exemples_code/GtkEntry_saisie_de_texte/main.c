#include <stdlib.h>
#include <gtk/gtk.h>
typedef struct{
    GtkWidget *pWindow;
    GtkWidget *pVBox;
    GtkWidget *pEntry;
    GtkWidget *pButton;
    GtkWidget *pLabel;
}fenetre;

void OnUpdate(GtkWidget *pEntry, gpointer data);
int main(int argc, char **argv)
{
    fenetre *aaa=NULL;
    gtk_init(&argc, &argv);

    aaa = g_malloc(sizeof(fenetre));
    aaa->pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(aaa->pWindow), "Le widget GtkEntry");
    gtk_window_set_default_size(GTK_WINDOW(aaa->pWindow), 320, 200);
    g_signal_connect(G_OBJECT(aaa->pWindow), "destroy",
G_CALLBACK(gtk_main_quit), NULL);
    aaa->pVBox = gtk_vbox_new(TRUE, 0);
    gtk_container_add(GTK_CONTAINER(aaa->pWindow), aaa->pVBox);
    /* Creation du GtkEntry */
    aaa->pEntry = gtk_entry_new();
    /* Insertion du GtkEntry dans la GtkVBox */
    gtk_box_pack_start(GTK_BOX(aaa->pVBox), aaa->pEntry, TRUE, FALSE, 0);
    aaa->pButton = gtk_button_new_with_label("Copier");
    gtk_box_pack_start(GTK_BOX(aaa->pVBox), aaa->pButton, TRUE, FALSE, 0);
    aaa->pLabel = gtk_label_new(NULL);
    gtk_box_pack_start(GTK_BOX(aaa->pVBox), aaa->pLabel, TRUE, FALSE, 0);
    /* Connexion du signal "activate" du GtkEntry (validation par la touche entrée)*/
    g_signal_connect(G_OBJECT(aaa->pEntry), "activate", G_CALLBACK(OnUpdate),(gpointer) aaa);
    /* Connexion du signal "clicked" du GtkButton */
    /* La donnee supplementaire est la GtkVBox pVBox */
    g_signal_connect(G_OBJECT(aaa->pButton), "clicked", G_CALLBACK(OnUpdate),(gpointer*) aaa);
    gtk_widget_show_all(aaa->pWindow);
    gtk_main();
    g_free(aaa);
    return EXIT_SUCCESS;
}
/* Fonction callback execute lors du signal "activate" */
void OnUpdate(GtkWidget *pEntry, gpointer data)
{
    const gchar *sText;
    fenetre *aaa;

    /* Recuperation de data */
    aaa = (fenetre*) data;
    /* Recuperation du texte contenu dans le GtkEntry */
    sText = gtk_entry_get_text(GTK_ENTRY(aaa->pEntry));
    /* Modification du texte contenu dans le GtkLabel */
    gtk_label_set_text(GTK_LABEL(aaa->pLabel), sText);
}
