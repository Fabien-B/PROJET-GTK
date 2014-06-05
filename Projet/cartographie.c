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
            gdk_draw_string(carte,font,gc, XCARTE*(pt_aero->pos_x),YCARTE*(pt_aero->pos_y) + 3, pt_aero->oaci);
            gdk_draw_point(carte,gc, XCARTE*(pt_aero->pos_x),YCARTE*(pt_aero->pos_y));
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
                gdk_draw_string(carte,font,gc, XCARTE*(pt_balise->pos_x),YCARTE*(pt_balise->pos_y) + 3, pt_balise->nom);
                gdk_draw_point(carte,gc, XCARTE*(pt_balise->pos_x),YCARTE*(pt_balise->pos_y));
            }
            else
            {
              //  printf("ELSE balise : Je change pas car affichage = %d\n", pt_balise->affichage);
            }
        pt_balise = pt_balise->ptsuiv;
        }
    }



    if(donnees->debutpdv!=NULL) // Rentre si un plan de vol est chargé
    {
//printf("boucle : 1\n");
    pdv * pt_pdv_current=donnees->debutpdv;

        while(pt_pdv_current!=NULL) // Parcours tous les plans de vol
        {
       // printf("boucle : 2\n");


            if(pt_pdv_current->affichage==1) // Vérifie si le plan de vol doit être affiché
            {
           // printf("boucle : 3\n");


            int x1=-9,x2=0,y1=0,y2=0; // Variables de stockage des coordonnées pour les points de passage

            pt_pass * pt_pass_current = pt_pdv_current->pass_debut;

                if(x1==-9 && pt_pass_current->ptsuiv!=NULL) // Cas du premier point
                {
                //printf("boucle : 4, point de type = %d\n",pt_pass_current->type_point);

                    if(pt_pass_current->type_point==0) // Si le point est un aérodrome
                    {
                        aerodrome * pass_aerodrome = pt_pass_current->point;
                        //printf("\n AERODROME 1 \n");

                        x1 = (pass_aerodrome->pos_x) * XCARTE;
                        y1 = (pass_aerodrome->pos_y) * YCARTE;
                        //printf("recup x = %d, y = %d\n",x1,y1);

                        // Affichage de l'aérodrome
                        gdk_draw_string(carte,font,gc, XCARTE*(pass_aerodrome->pos_x) + 2,YCARTE*(pass_aerodrome->pos_y) + 7, pass_aerodrome->oaci);
                        gdk_draw_point(carte,gc, XCARTE*(pass_aerodrome->pos_x),YCARTE*(pass_aerodrome->pos_y));
                    }

                    if(pt_pass_current->type_point==1) // Si le point est une balise
                    {
                        balise * pass_balise = pt_pass_current->point;
                       // printf("\n BALISE 1 \n");

                        x1 = (pass_balise->pos_x) * XCARTE;
                        y1 = (pass_balise->pos_y) * YCARTE;
                       // printf("recup x = %d, y = %d\n",x1,y1);

                        // Affichage de la balise
                        gdk_draw_string(carte,font,gc, XCARTE*(pass_balise->pos_x) + 2,YCARTE*(pass_balise->pos_y) + 7, pass_balise->nom);
                        gdk_draw_point(carte,gc, XCARTE*(pass_balise->pos_x),YCARTE*(pass_balise->pos_y));
                    }

                pt_pass_current = pt_pass_current->ptsuiv;

                }
                else
                {
                printf("Parcours incorrect");
                }

                while(pt_pass_current->ptsuiv!=NULL) // Parcours de tous les points de passage
                {
               // printf("boucle : 5\n");


                        if(pt_pass_current->type_point==0) // Si le point est un aérodrome
                    {
                    aerodrome * pass_aerodrome = pt_pass_current->point;
                   // printf("\n AERODROME \n");
                    x2 = (pass_aerodrome->pos_x) * XCARTE;
                    y2 = (pass_aerodrome->pos_y) * YCARTE;
                  //  printf("recup x = %d, y = %d\n",x2,y2);

                    // Affichage de l'aerodrome
                    gdk_draw_string(carte,font,gc, XCARTE*(pass_aerodrome->pos_x) + 2,YCARTE*(pass_aerodrome->pos_y) + 7, pass_aerodrome->oaci);
                    gdk_draw_point(carte,gc, XCARTE*(pass_aerodrome->pos_x),YCARTE*(pass_aerodrome->pos_y));
                    }

                    if(pt_pass_current->type_point==1) // Si le point est une balise
                    {
                    balise * pass_balise = pt_pass_current->point;
                  //  printf("\n BALISE \n");

                    x2 = (pass_balise->pos_x) * XCARTE;
                    y2 = (pass_balise->pos_y) * YCARTE;
                   // printf("recup x = %d, y = %d\n",x2,y2);

                    // Affichage de la balise
                    gdk_draw_string(carte,font,gc, XCARTE*(pass_balise->pos_x) + 2,YCARTE*(pass_balise->pos_y) + 7, pass_balise->nom);
                    gdk_draw_point(carte,gc, XCARTE*(pass_balise->pos_x),YCARTE*(pass_balise->pos_y));
                    }

                    gdk_draw_line(carte, gc, x1,y1,x2,y2);


                    // Le point 2 devient le point 1
                    x1=x2;
                    y1=y2;
                    pt_pass_current = pt_pass_current->ptsuiv;
                }


            }
            else
            {
           // printf("affichage refusé car affichage = %d \n",pt_pdv_current->affichage);
            }
        pt_pdv_current=pt_pdv_current->ptsuiv;
        }
   // printf("\nFIN PLAN DE VOL \n\n");
    }

    gdk_font_unref(font);
}

