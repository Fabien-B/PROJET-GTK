#include <gtk/gtk.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    GtkWidget* window;
    GtkWidget* box;
    GtkWidget *scrollbar;
    int i;

    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 320, 200);
    gtk_window_set_title(GTK_WINDOW(window), "GtkScrolledWindow");
    g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit),0);

    scrollbar = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(window),scrollbar);

    box=gtk_vbox_new(FALSE,5);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrollbar), box);

    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbar), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);

    for(i = 1 ; i <= 10 ; ++i)
    {
        GtkWidget *label;
        char texte[10];

        sprintf(texte, "Label %d", i);

        label = gtk_label_new(texte);

        gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 5);
    }

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
