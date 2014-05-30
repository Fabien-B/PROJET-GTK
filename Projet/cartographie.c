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
    GdkFont * font=gdk_font_load("6x9");//6x9
   if(donnees->debutaero!=NULL)
    {
// 0.678800746 => 640 -> 443.432477141


    aerodrome* pt_aero=donnees->debutaero;

        while(pt_aero->ptsuiv!=NULL)
        {

            if(pt_aero->affichage==1)
            {
          //  printf("go avec x = %lf et y = %lf\n",pt_aero->pos_x,pt_aero->pos_y);

              //  printf("IF aero : Je change pas car affichage = %d\n", pt_aero->affichage);
            gdk_draw_string(carte,font,gc, 440*(pt_aero->pos_x),443*(pt_aero->pos_y) + 3, pt_aero->oaci);
            gdk_draw_point(carte,gc, 440*(pt_aero->pos_x),443*(pt_aero->pos_y));
            }
            else
            {
              //  printf("ELSE aero : Je change pas car affichage = %d\n", pt_aero->affichage);
            }
        pt_aero = pt_aero->ptsuiv;
        }
    }

    if(donnees->debutbalises!=NULL)
    {

    balise* pt_balise=donnees->debutbalises;

        while(pt_balise->ptsuiv!=NULL)
        {
            if(pt_balise->affichage==1)
            {
             //   printf("IF balise : Je change car affichage = %d\n", pt_balise->affichage);
                gdk_draw_string(carte,font,gc, 440*(pt_balise->pos_x),443*(pt_balise->pos_y) + 3, pt_balise->nom);
                gdk_draw_point(carte,gc, 440*(pt_balise->pos_x),443*(pt_balise->pos_y));
            }
            else
            {
              //  printf("ELSE balise : Je change pas car affichage = %d\n", pt_balise->affichage);
            }
        pt_balise = pt_balise->ptsuiv;
        }
    }
    gdk_font_unref(font);
}

