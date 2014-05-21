#include "interface.h"
#include "ouverture_fichiers.h"
#include "cartographie.h"



// Fonction de rafraichissement
gboolean expose_cb (GtkWidget *canvas, GdkEventExpose *event, file_opener * donnees)
{
  GdkGC *gc;

  gc = gdk_gc_new (canvas->window);
  dessiner (GDK_DRAWABLE(canvas->window), gc, donnees);
  g_object_unref (gc);

  return FALSE;
}


void dessiner(GdkDrawable* carte, GdkGC *gc, file_opener *donnees)
{
    GdkPoint points[4];
    GdkFont * font=gdk_font_load("6x9");//6x9
    // Tester les différetes tailles
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

    gdk_gc_set_rgb_fg_color (gc, &c);
    gdk_gc_set_rgb_fg_color (gc, &couleur2);

//     dessine un point en 40,40
    gdk_draw_point (carte,gc, 40, 40);

    //dessine une ligne (segment) :
    gdk_draw_line (carte, gc, 0, 0, 50, 50);


//    gc.set_fill(couleur);

    //dessine plusieurs points donnés dans un tableau :
     points[0].x=60;
     points[0].y=60;
     points[1].x=100;
     points[1].y=80;
     points[2].x=150;
     points[2].y=150;
     points[3].x=70;
     points[3].y=120;
    gdk_draw_points(carte, gc, points, 3);

    // dessine un segment :
    gdk_draw_line(carte, gc, 100,100,150, 150);

    //dessine plusieurs segments :
    gdk_draw_lines(carte, gc, points, 3);

    //gdk_draw_rectangle(carte, gc, TRUE, 10, 10, 50, 50);
    //gdk_draw_polygon(carte, gc, TRUE, points, 4);

    gdk_gc_set_rgb_fg_color (gc, &couleur2);
    // affichage de texte
    char *tes="test";
    gdk_draw_string(carte, font, gc, 50, 50, tes);
    gdk_draw_text (carte, font, gc, 100,100, "bonjour tout le de" , 20);

    gdk_font_unref(font);
}

