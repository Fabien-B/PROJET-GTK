//structure pour les fichiers de donnees
#include <stdlib.h>
#include <gtk/gtk.h>


int main(int argc, char *argv[])
{
    GtkWidget *Window;
  //  GtkWidget *mother_box;

    GtkWidget *carte;




    gtk_init(&argc,&argv);

    Window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(Window), "Le projet de ouf!!!");
    gtk_window_set_default_size(GTK_WINDOW(Window), 10, 10);
    g_signal_connect(G_OBJECT(Window), "destroy", G_CALLBACK(gtk_main_quit), NULL);


// Creation et ajout de la GtkBox mère verticale
  //  mother_box = gtk_vbox_new(FALSE, 0);
 //   gtk_container_add(GTK_CONTAINER(Window), mother_box);

    carte=gtk_drawing_area_new ();
    gtk_widget_set_size_request (carte, 500, 250);
    gtk_container_add(GTK_CONTAINER(Window), carte);
   // gtk_box_pack_start(GTK_CON(Window), carte, FALSE, FALSE, 0);


cairo_t *cr;
GdkWindow* aze=gtk_widget_get_root_window(Window);
cr = gdk_cairo_create(GDK_DRAWABLE(aze));
cairo_rectangle(cr, 0.0, 0.0, 50, 75);
cairo_set_source_rgb(cr, 0.6, 0.6, 1.0);
cairo_fill(cr);
cairo_destroy(cr);



    gtk_widget_show_all(Window);
    gtk_main ();
    return 0;
}


