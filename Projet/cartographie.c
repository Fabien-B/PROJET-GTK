#include "interface.h"
#include "ouverture_fichiers.h"
#include "cartographie.h"
#include "filtrage.h"
#include "ajouts_utilisateur.h"



// Fonction de rafraichissement
gboolean expose_cb (GtkWidget *canvas, GdkEventExpose *event, file_opener * donnees)
{
  GdkGC *gc;
 // canvas = donnees->carte
  gc = gdk_gc_new (canvas->window);
  dessiner (GDK_DRAWABLE(canvas->window), gc, donnees);
  g_object_unref (gc);

  return FALSE;
}


void dessiner(GdkDrawable* carte, GdkGC *gc, file_opener *donnees)
{
    GdkFont * font=gdk_font_load("6x9");//6x9 - 10x20

 /* ----------------------------------  CONTOUR FRANCE -------------------------------------- */

    double contour[52][2]={{0.761080272727,0.247676},{0.759412909091,0.2539861875},{0.755343636364,0.2597690625},{0.749973272727,0.2628678125},
                            {0.743332636364,0.2661723125},{0.735988545455,0.2690369375},{0.717142727273,0.272496},{0.690698090909,0.27756},
                            {0.664294,0.2808645},{0.635493454545,0.2814653125},{0.610466,0.284469375},{0.581608727273,0.287001375},
                            {0.561556909091,0.2926233125},{0.537243636364,0.2891879375},{0.505640818182,0.284166875},{0.487748818182,0.265234875},
                            {0.449146363636,0.226439375},{0.410337727273,0.2171696875},{0.380760727273,0.169276125},{0.363941545455,0.139750375},
                            {0.354465363636,0.097380125},{0.336070090909,0.0643353125},{0.327578727273,0.0931523125},{0.316845454545,0.0707505},
                            {0.287682636364,0.0705901875},{0.262316818182,0.1648323125},{0.293963363636,0.1931564375},{0.282730181818,0.2709190625},
                            {0.185582818182,0.2402984375},{0.186874818182,0.280123875},{0.225156909091,0.34810175},{0.158218545455,0.444232125},
                            {0.0758867272727,0.459681625},{0.0608012727273,0.5194198125},{0.123058727273,0.5946075},{0.205001545455,0.7298766875},
                            {0.250315272727,0.8699523125},{0.377882272727,0.8332168125},{0.508272181818,0.73159325},{0.484634181818,0.784465},
                            {0.529590181818,0.7995711875},{0.585475818182,0.8048926875},{0.604299272727,0.7734785625},{0.719421636364,0.8466279375},
                            {0.790847363636,0.7429444375},{0.745780909091,0.61454175},{0.792307272727,0.5499970625},{0.845854090909,0.5589234375},
                            {0.843643545455,0.468972875},{0.820851181818,0.3989350625},{0.813518363636,0.32683725},{0.761080272727,0.247676}};


    int i;
    for(i=0;i<51;i++)
    {
        gdk_draw_line(carte, gc, contour[i][1]*XCARTE,contour[i][0]*YCARTE,contour[i+1][1]*XCARTE,contour[i+1][0]*YCARTE);
    }

//gdk_draw_string(carte,font,gc,0.5*YCARTE,0.5*YCARTE,"AA");

 /* ----------------------------------  AERODROMES -------------------------------------- */

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
            gdk_draw_string(carte,font,gc, XCARTE*(pt_aero->pos_x)+2,YCARTE*(pt_aero->pos_y) + 7, pt_aero->oaci);
            gdk_draw_point(carte,gc, XCARTE*(pt_aero->pos_x),YCARTE*(pt_aero->pos_y));
            }
            else
            {
              //  printf("ELSE aero : Je change pas car affichage = %d\n", pt_aero->affichage);
            }
        pt_aero = pt_aero->ptsuiv;
        }
    }




 /* ----------------------------------  BALISES -------------------------------------- */

    if(donnees->debutbalises!=NULL)
    {

    balise* pt_balise=donnees->debutbalises;

        while(pt_balise->ptsuiv!=NULL)
        {
            if(pt_balise->affichage==1)
            {
             //   printf("IF balise : Je change car affichage = %d\n", pt_balise->affichage);
                gdk_draw_string(carte,font,gc, XCARTE*(pt_balise->pos_x)+2,YCARTE*(pt_balise->pos_y) + 7, pt_balise->nom);
                gdk_draw_point(carte,gc, XCARTE*(pt_balise->pos_x),YCARTE*(pt_balise->pos_y));
            }
            else
            {
              //  printf("ELSE balise : Je change pas car affichage = %d\n", pt_balise->affichage);
            }
        pt_balise = pt_balise->ptsuiv;
        }
    }




 /* ----------------------------------  PLAN DE VOL -------------------------------------- */


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
            position* loc_avion=malloc(sizeof(position));
//            Position_avion(donnees,pt_pdv_current,loc_avion);
            get_position_avion(loc_avion,pt_pdv_current,donnees->temps);
            loc_avion->x = loc_avion->x * XCARTE;
            loc_avion->y = loc_avion->y * YCARTE;
//            printf("\n");
//            g_print("%lf",loc_avion->x);
//            g_print("%lf",loc_avion->y);

            gdk_draw_point(carte,gc,loc_avion->x,loc_avion->y);
            gdk_draw_string(carte,font,gc,loc_avion->x+2,loc_avion->y-1,pt_pdv_current->nom);


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



//                  Une couleur par niveau de vol
                     if(pt_pdv_current->altitude<140)
                    {
                      GdkColor c;
                    c.pixel = 5;
                    c.red = (150 - pt_pdv_current->altitude )*5 << 8 ;
                    c.green = 10 << 8;
                    c.blue = 10 << 8;
                    gdk_gc_set_rgb_fg_color (gc, &c);
                    }
                    else if(pt_pdv_current->altitude>180)
                    {
                      GdkColor c;
                    c.pixel = 5;
                    c.red = 10 << 8 ;
                    c.green = (pt_pdv_current->altitude -180)*5 << 8;
                    c.blue = 10 << 8;
                    gdk_gc_set_rgb_fg_color (gc, &c);
                    }
                    else
                    {
                      GdkColor c;
                    c.pixel = 5;
                    c.red = 10 << 8 ;
                    c.green = 10 << 8;
                    c.blue = (190 - pt_pdv_current->altitude)*5 << 8;
                    gdk_gc_set_rgb_fg_color (gc, &c);
                    }

//                  Trace la ligne et remet la couleur normalement.
                    gdk_draw_line(carte, gc, x1,y1,x2,y2);

                    GdkColor c;
                    c.pixel = 0;
                    c.red = 0 << 8 ;
                    c.green = 0 << 8;
                    c.blue = 0 << 8;
                    gdk_gc_set_rgb_fg_color (gc, &c);

                    // Le point 2 devient le point 1
                    x1=x2;
                    y1=y2;
                    pt_pass_current = pt_pass_current->ptsuiv;

                }


            }
//            else
//            {
//            printf("affichage refusé car affichage = %d \n",pt_pdv_current->affichage);
//            }
        pt_pdv_current=pt_pdv_current->ptsuiv;
        }
//    printf("\nFIN PLAN DE VOL \n\n");
    }

    gdk_font_unref(font);
}

//position* Position_avion(file_opener* donnees, pdv* avion, position* loc)
//{
//position *delta_pass=malloc(sizeof(position));
//
//
//
//
//    if(avion->pass_debut->temps < donnees->temps)
//    {
//    loc->x=20;
//    loc->y=20;
//    pt_pass* pt_pass_current = avion->pass_debut;
//    pt_pass* pt_pass_previous = avion->pass_debut;
//
//        while(pt_pass_current->ptsuiv!=NULL && pt_pass_current->temps < donnees->temps)
//        {
//        pt_pass_previous = pt_pass_current;
//        pt_pass_current = pt_pass_current->ptsuiv;
//        }
//
////        Detection du type des points de passages précédant et suivant
//        if(pt_pass_previous->type_point==0)
//        {
//        aerodrome* pt_aero_previous = pt_pass_previous->point;
//        printf("Point précédant aerodrome : %s\n",pt_aero_previous->oaci);
//
//            if(pt_pass_current->type_point==0)
//            {
//                aerodrome* pt_aero_current = pt_pass_current->point;
//                printf("Point suivant aerodrome : %s\n",pt_aero_current->oaci);
//
////                On recalcule la position de l'avion à partir du point précédant plus du trajet effectué
//                loc->x = pt_aero_previous->pos_x;
//                loc->y = pt_aero_previous->pos_y;
//
////                Calcul du chemin entre le deux points de passages
//                delta_pass->x = pt_aero_current->pos_x - pt_aero_previous->pos_x ;
//                delta_pass->y = pt_aero_current->pos_y - pt_aero_previous->pos_y ;
//            }
//
//            if(pt_pass_current->type_point==1)
//            {
//                balise* pt_balise_current = pt_pass_current->point;
//                printf("Point suivant balise : %s\n",pt_balise_current->nom);
//
//                loc->x = pt_aero_previous->pos_x;
//                loc->y = pt_aero_previous->pos_y;
//
//                delta_pass->x = pt_balise_current->pos_x - pt_aero_previous->pos_x ;
//                delta_pass->y = pt_balise_current->pos_y - pt_aero_previous->pos_y ;
//            }
//        }
//
//
//        if(pt_pass_previous->type_point==1)
//        {
//        balise* pt_balise_previous = pt_pass_previous->point;
//        printf("Point précédant balise : %s\n",pt_balise_previous->nom);
//
//            if(pt_pass_current->type_point==0)
//            {
//                aerodrome* pt_aero_current = pt_pass_current->point;
//                printf("Point suivant aerodrome : %s\n",pt_aero_current->oaci);
//
//                loc->x = pt_balise_previous->pos_x;
//                loc->y = pt_balise_previous->pos_y;
//
//                delta_pass->x = pt_aero_current->pos_x - pt_balise_previous->pos_x ;
//                delta_pass->y = pt_aero_current->pos_y - pt_balise_previous->pos_y ;
//            }
//
//            if(pt_pass_current->type_point==1)
//            {
//                balise* pt_balise_current = pt_pass_current->point;
//                printf("Point suivant balise : %s\n",pt_balise_current->nom);
//
//                loc->x = pt_balise_previous->pos_x;
//                loc->y = pt_balise_previous->pos_y;
//
//                delta_pass->x = pt_balise_current->pos_x - pt_balise_previous->pos_x ;
//                delta_pass->y = pt_balise_current->pos_y - pt_balise_previous->pos_y ;
//            }
//        }
//
//// Donne le pourcentage de chemin effectué entre les deux points de passage
//double facteur_temps = (donnees->temps - pt_pass_previous->temps) / (pt_pass_current->temps - pt_pass_previous->temps);
//
//     loc->x = (loc->x + (delta_pass->x) * facteur_temps) * XCARTE ;
//     loc->y = (loc->y + (delta_pass->y) * facteur_temps) * YCARTE ;
//
//    }
//    else
//    {
//    printf("avion encore au sol\n");
//    }
//
//free(delta_pass);
//
//
//return loc;
//}

//  GdkPoint points[4];
//    GdkFont * font=gdk_font_load("10x20");//6x9
//    // Tester les différetes tailles
//    // polices disponibles sous c:\windows\fonts sous windows, /usr/share/fonts/X11/misc sous linux
//
//    // première méthode pour définir avec précision une couleur :
//    GdkColor c;
//    c.pixel = 0;
//    c.red = 200 << 8 ;
//    c.green = 10 << 8;
//    c.blue = 100 << 8;
//
//    // deuxième méthode : utiliser une couleur prédéfinie.
//    GdkColor  couleur1;
//    gdk_color_parse ("red", &couleur1);
//
//    GdkColor  couleur2;
//    gdk_color_parse ("blue", &couleur2);
//
//    gdk_gc_set_rgb_fg_color (gc, &c);
//    gdk_gc_set_rgb_fg_color (gc, &couleur2);
//
////     dessine un point en 40,40
//    gdk_draw_point (carte,gc, 40, 40);
//
//    //dessine une ligne (segment) :
//   // gdk_draw_line (carte, gc, 0, 0, 50, 50);
//
//
////    gc.set_fill(couleur);
//
//    //dessine plusieurs points donnés dans un tableau :
//     points[0].x=60;
//     points[0].y=60;
//     points[1].x=100;
//     points[1].y=80;
//     points[2].x=150;
//     points[2].y=150;
//     points[3].x=70;
//     points[3].y=120;
//    gdk_draw_points(carte, gc, points, 3);
//
//    // dessine un segment :
//   // gdk_draw_line(carte, gc, 100,100,150, 150);
//
//    //dessine plusieurs segments :
//  //  gdk_draw_lines(carte, gc, points, 3);
//
//    //gdk_draw_rectangle(carte, gc, TRUE, 10, 10, 50, 50);
//    //gdk_draw_polygon(carte, gc, TRUE, points, 4);
//
//    gdk_gc_set_rgb_fg_color (gc, &couleur2);
//    // affichage de texte
//    char *tes="test";
//    gdk_draw_string(carte, font, gc, 50, 50, tes);
//    gdk_draw_text (carte, font, gc, 100,100, "Carte" , 5);
//
//    gdk_font_unref(font);
//}
