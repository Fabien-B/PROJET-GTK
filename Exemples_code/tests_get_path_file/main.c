#include <stdlib.h>
#include <gtk/gtk.h>
// pour forcer le rafraichissement de la zone de dessin, on peut faire appel à : gtk_widget_queue_draw(GTK_WIDGET(area));

// fonction qui dessine tout :
void dessiner(GdkDrawable* area, GdkGC *gc);

// fonction qui gère le rafraichissement du dessin :
gboolean expose_cb (GtkWidget *canvas, GdkEventExpose *event, gpointer user_data)
{
  GdkGC *gc;

  gc = gdk_gc_new (canvas->window);
  dessiner (canvas->window, gc);
  g_object_unref (gc);

  return FALSE;
}


int main (int argc, char *argv[])
{
 // GtkWidget *button1 = NULL, *button2=NULL;
  GtkWidget *win = NULL;
  GtkWidget *hbox = NULL;

  /* Initialize GTK+ */
  g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, (GLogFunc) gtk_false, NULL);
  gtk_init (&argc, &argv);
  g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, g_log_default_handler, NULL);

  /* Create the main window */
  win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_container_set_border_width (GTK_CONTAINER (win), 8);
  gtk_window_set_title (GTK_WINDOW (win), "Hello World");
  gtk_window_set_position (GTK_WINDOW (win), GTK_WIN_POS_CENTER);
  gtk_widget_realize (win);
  g_signal_connect (win, "destroy", gtk_main_quit, NULL);

  gtk_window_set_default_size(win, 400, 400);

  /* Create a vertical box with buttons */
  hbox = gtk_hbox_new (TRUE, 6);
  gtk_container_add (GTK_CONTAINER (win), hbox);


    // création de la zone de dessin :
  GtkWidget* area=gtk_drawing_area_new ();
  gtk_drawing_area_size (area, 400,400);
  gtk_box_pack_start (GTK_BOX (hbox), area, TRUE, TRUE, 0);


  // gestion de l'évènement  expose_event sur area :
    g_signal_connect (G_OBJECT (area), "expose-event", G_CALLBACK (expose_cb), NULL);

   /* Enter the main loop */
  gtk_widget_show_all (win);
  gtk_main ();
  return 0;
}

//****************************************************
void dessiner(GdkDrawable* area, GdkGC *gc)
{
    GdkPoint points[4];
    GdkFont * font=gdk_font_load("Book Antiqua");
    // polices disponibles sous c:\windows\fonts sous windows, /usr/share/fonts/X11/misc sous linux

    // première méthode pour définir avec précision une couleur :
    GdkColor c;
    c.pixel = 0;
    c.red = 200 << 8 ;
    c.green = 10 << 8;
    c.blue = 100 << 8;

    // deuxième méthode : utiliser une couleur prédéfinie.
    GdkColor  couleur1;
    gdk_color_parse ("red", &couleur1);

    GdkColor  couleur2;
    gdk_color_parse ("blue", &couleur2);

    //gdk_gc_set_rgb_fg_color (gc, &c);
    gdk_gc_set_rgb_fg_color (gc, &couleur1);

    // dessine un point en 40,40
    //gdk_draw_point (area,gc, 40, 40);

    //dessine une ligne (segment) :
    gdk_draw_line (area, gc, 0, 0, 50, 50);


    //gc.set_fill(couleur);

    //dessine plusieurs points donnés dans un tableau :
     points[0].x=60;
     points[0].y=60;
     points[1].x=100;
     points[1].y=80;
     points[2].x=150;
     points[2].y=150;
     points[3].x=70;
     points[3].y=120;
    //gdk_draw_points(area, gc, points, 3);

    // dessine un segment :
    //gdk_draw_line(area, gc, 100,100,150, 150);

    //dessine plusieurs segments :
    //gdk_draw_lines(area, gc, points, 3);

    //gdk_draw_rectangle(area, gc, TRUE, 10, 10, 50, 50);
    //gdk_draw_polygon(area, gc, TRUE, points, 4);

    gdk_gc_set_rgb_fg_color (gc, &couleur2);
    gdk_draw_string(area, font, gc, 50, 50, "bonjour");
    gdk_draw_text (area, font, gc, 100,100, "bonjour tout le monde" , 15);

    gdk_font_unref(font);
}
