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
    GdkColor c;
int j;
char text[5];

couleur(gc,c,-2);
for(j=15;j<=85;j+=5)
{
sprintf(text,"%d",j);
gdk_draw_line(carte,gc,0,conversion_lat(j,donnees)*donnees->ycarte,donnees->xcarte*3,conversion_lat(j,donnees)*donnees->ycarte);
gdk_draw_string(carte,font,gc,3,conversion_lat(j,donnees)*donnees->ycarte-5,text);
}

for(j=-30;j<=45;j+=5)
{
sprintf(text,"%d",j);
gdk_draw_line(carte,gc,conversion_longitude(j,donnees)*donnees->xcarte,0,conversion_longitude(j,donnees)*donnees->xcarte,donnees->ycarte);
gdk_draw_string(carte,font,gc,conversion_longitude(j,donnees)*donnees->xcarte+5,8,text);
}
couleur(gc,c,-1);

 /* ----------------------------------  CONTOUR FRANCE -------------------------------------- */

    double contour[52][2]={{43.378117,-1.787184},{43.396458,-1.686221},{43.441220,-1.593695},{43.500294,-1.544115},
                            {43.573341,-1.491243},{43.654126,-1.445409},{43.861430,-1.390064},{44.152321,-1.309040},
                            {44.442766,-1.256168},{44.759572,-1.246555},{45.034874,-1.198490},{45.352304,-1.157978},
                            {45.572874,-1.068027},{45.840320,-1.122993},{46.187951,-1.203330},{46.384763,-1.506242},
                            {46.809390,-2.126970},{47.236285,-2.275285},{47.561632,-3.041582},{47.746643,-3.513994},
                            {47.850881,-4.191918},{48.053229,-4.720635},{48.146634,-4.259563},{48.264700,-4.617992},
                            {48.585491,-4.620557},{48.864515,-3.112683},{48.516403,-2.659497},{48.639968,-1.415295},
                            {49.708589,-1.905225},{49.694377,-1.268018},{49.273274,-0.180372},{50.009596,1.357714},
                            {50.915246,1.604906},{51.081186,2.560717},{50.396354,3.763720},{49.494983,5.928027},
                            {48.996532,8.169237},{47.593295,7.581469},{46.159006,5.955492},{46.419024,6.801440},
                            {45.924508,7.043139},{45.309766,7.128283},{45.102708,6.625657},{43.836362,7.796047},
                            {43.050679,6.137111},{43.546410,4.082668},{43.034620,3.049953},{42.445605,3.192775},
                            {42.469921,1.753566},{42.720637,0.632961},{42.801298,-0.520604},{43.378117,-1.787184}};

    double contourcorse[38][2]={{43.005655,9.348229},{43.010676,9.445389},{42.802458,9.490021},{42.681934,9.446763},
                                {42.566740,9.527787},{42.431570,9.540146},{42.284425,9.557999},{42.107396,9.552506},
                                {41.956430,9.415177},{41.694453,9.400071},{41.559981,9.343766},{41.363415,9.189957},
                                {41.585665,8.787583},{41.638029,8.802689},{41.687275,8.915299},{41.732896,8.767670},
                                {41.741606,8.654374},{41.896148,8.802689},{41.929361,8.755997},{41.897170,8.606995},
                                {41.974300,8.591202},{42.052865,8.743638},{42.110453,8.687333},{42.128279,8.580903},
                                {42.236656,8.539017},{42.266134,8.690079},{42.364636,8.538331},{42.573313,8.719605},
                                {42.661235,9.049882},{42.727347,9.091767},{42.741469,9.179658},{42.715745,9.255189},
                                {42.673857,9.291581},{42.740460,9.344453},{42.832678,9.309434},{42.899110,9.320420},
                                {42.926267,9.356812},{43.005655,9.348229}};

    {
        double x1,x2,y1,y2;
        FILE *f = fopen("contour_france.csv", "rb");
        //printf("f=%p\n", f);
        if(f){
            char buffer[1024];

            while(fgets(buffer, sizeof(buffer), f)){

                sscanf(buffer, "%lf;%lf", &x1, &y1);
                x1 = conversion_longitude(x1, donnees);
                y1 = conversion_lat(y1, donnees);
                while(fgets(buffer, sizeof(buffer), f)){

                    if(!strcmp(buffer, "BREAK\n")){
                        break;
                    }

                    sscanf(buffer, "%lf;%lf", &x2, &y2);
                    x2=conversion_longitude(x2, donnees);
                    y2=conversion_lat(y2, donnees);
                    gdk_draw_line(carte, gc, x1*donnees->xcarte,y1*donnees->ycarte,x2*donnees->xcarte,y2*donnees->ycarte);
                    x1=x2;
                    y1=y2;
                }
            }
            fclose(f);
        }
    }
/*
    double x1,x2,y1,y2;
    x1=conversion_longitude(contour[0][1],donnees);
    y1=conversion_lat(contour[0][0],donnees);
    for(i=0;i<51;i++)
    {
        x2=conversion_longitude(contour[i+1][1],donnees);
        y2=conversion_lat(contour[i+1][0],donnees);
        gdk_draw_line(carte, gc, x1*donnees->xcarte,y1*donnees->ycarte,x2*donnees->xcarte,y2*donnees->ycarte);

        x1=x2;
        y1=y2;
    }

    x1=conversion_longitude(contourcorse[0][1],donnees);
    y1=conversion_lat(contourcorse[0][0],donnees);
    for(i=0;i<37;i++)
    {
        x2=conversion_longitude(contourcorse[i+1][1],donnees);
        y2=conversion_lat(contourcorse[i+1][0],donnees);
        gdk_draw_line(carte, gc, x1*donnees->xcarte,y1*donnees->ycarte,x2*donnees->xcarte,y2*donnees->ycarte);

        x1=x2;
        y1=y2;
    }
*/
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
            double x=conversion_longitude(pt_aero->longitude,donnees);
            double y=conversion_lat(pt_aero->latitude,donnees);
              //  printf("IF aero : Je change pas car affichage = %d\n", pt_aero->affichage);
            gdk_draw_string(carte,font,gc, donnees->xcarte*x+2,donnees->ycarte*y + 7, pt_aero->oaci);
            gdk_draw_point(carte,gc, donnees->xcarte*x,donnees->ycarte*y);
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
                double x=conversion_longitude(pt_balise->longitude,donnees);
                double y=conversion_lat(pt_balise->latitude,donnees);
                gdk_draw_string(carte,font,gc, donnees->xcarte*x+2,donnees->ycarte*y + 7, pt_balise->nom);
                gdk_draw_point(carte,gc, donnees->xcarte*x,donnees->ycarte*y);
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
            loc_avion->y=conversion_lat(loc_avion->y,donnees)*donnees->ycarte;
            loc_avion->x=conversion_longitude(loc_avion->x,donnees)*donnees->xcarte;
           // loc_avion->x = loc_avion->x * donnees->xcarte;
           // loc_avion->y = loc_avion->y * donnees->ycarte;
//            printf("\n");
//            g_print("%lf",loc_avion->x);
//            g_print("%lf",loc_avion->y);

            gdk_draw_point(carte,gc,loc_avion->x,loc_avion->y);
            gdk_draw_rectangle(carte,gc,TRUE,loc_avion->x-2,loc_avion->y-2,5,5);
            gdk_draw_string(carte,font,gc,loc_avion->x+3,loc_avion->y-2,pt_pdv_current->nom);


            int x1=-9,x2=0,y1=0,y2=0; // Variables de stockage des coordonnées pour les points de passage

            pt_pass * pt_pass_current = pt_pdv_current->pass_debut;

                if(x1==-9 && pt_pass_current->ptsuiv!=NULL) // Cas du premier point
                {
                //printf("boucle : 4, point de type = %d\n",pt_pass_current->type_point);

                    if(pt_pass_current->type_point==0) // Si le point est un aérodrome
                    {
                        aerodrome * pass_aerodrome = pt_pass_current->point;
                        //printf("\n AERODROME 1 \n");
                        x1=conversion_longitude(pass_aerodrome->longitude,donnees)* donnees->xcarte;
                        y1=conversion_lat(pass_aerodrome->latitude,donnees)* donnees->ycarte;
                        //x1 = (pass_aerodrome->pos_x) * donnees->xcarte;
                        //y1 = (pass_aerodrome->pos_y) * donnees->ycarte;
                        //printf("recup x = %d, y = %d\n",x1,y1);

                        // Affichage de l'aérodrome
                        gdk_draw_string(carte,font,gc, x1 + 2,y1 + 7, pass_aerodrome->oaci);
                        gdk_draw_point(carte,gc, x1,y1);
                    }

                    if(pt_pass_current->type_point==1) // Si le point est une balise
                    {
                        balise * pass_balise = pt_pass_current->point;
                       // printf("\n BALISE 1 \n");
                        x1=conversion_longitude(pass_balise->longitude,donnees)* donnees->xcarte;
                        y1=conversion_lat(pass_balise->latitude,donnees)* donnees->ycarte;
                        //x1 = (pass_balise->pos_x) * donnees->xcarte;
                        //y1 = (pass_balise->pos_y) * donnees->ycarte;
                       // printf("recup x = %d, y = %d\n",x1,y1);

                        // Affichage de la balise
                        gdk_draw_string(carte,font,gc, x1 + 2,y1 + 7, pass_balise->nom);
                        gdk_draw_point(carte,gc, x1,y1);
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
                    //x2 = (pass_aerodrome->pos_x) * donnees->xcarte;
                   // y2 = (pass_aerodrome->pos_y) * donnees->ycarte;
                    x2=conversion_longitude(pass_aerodrome->longitude,donnees)* donnees->xcarte;
                    y2=conversion_lat(pass_aerodrome->latitude,donnees)* donnees->ycarte;
                  //  printf("recup x = %d, y = %d\n",x2,y2);

                    // Affichage de l'aerodrome
                    int x=conversion_longitude(pass_aerodrome->longitude,donnees)*donnees->xcarte;
                    int y=conversion_lat(pass_aerodrome->latitude,donnees)*donnees->ycarte;
                    gdk_draw_string(carte,font,gc, x + 2,y + 7, pass_aerodrome->oaci);
                    gdk_draw_point(carte,gc, x,y);
                    }

                    if(pt_pass_current->type_point==1) // Si le point est une balise
                    {
                    balise * pass_balise = pt_pass_current->point;
                  //  printf("\n BALISE \n");

                   // x2 = (pass_balise->pos_x) * donnees->xcarte;
                  //  y2 = (pass_balise->pos_y) * donnees->ycarte;
                    x2=conversion_longitude(pass_balise->longitude,donnees)* donnees->xcarte;
                    y2=conversion_lat(pass_balise->latitude,donnees)* donnees->ycarte;
                   // printf("recup x = %d, y = %d\n",x2,y2);
                    int x=conversion_longitude(pass_balise->longitude,donnees)* donnees->xcarte;
                    int y=conversion_lat(pass_balise->latitude,donnees)* donnees->ycarte;
                    // Affichage de la balise
                    gdk_draw_string(carte,font,gc, x + 2,y + 7, pass_balise->nom);
                    gdk_draw_point(carte,gc, x,y);
                    }



//                  Une couleur par niveau de vol
                    couleur(gc,c,pt_pdv_current->altitude);


//                  Trace la ligne et remet la couleur normalement.
                    gdk_draw_line(carte, gc, x1,y1,x2,y2);

                    couleur(gc,c,-1);

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

/* ------------------------------------ CONFLITS  ---------------------------------------- */


    if(donnees->deb_conflits!=NULL)
    {
    couleur(gc,c,-3);
    conflit* conflit_current = donnees->deb_conflits;
    int conflit_affiche = 0;

        while(conflit_current->ptsuiv != NULL)
        {
        pdv* avion1 = conflit_current->pdv1;
        pdv* avion2 = conflit_current->pdv2;


//        g_print("debut : %lf, fin : %lf",conflit_current->temps_deb,conflit_current->temps_fin);
            if(donnees->temps > conflit_current->temps_deb && donnees->temps < conflit_current->temps_fin)
            {
            conflit_affiche++;
                position c0;
                g_print("Conflit \n");
                if(avion1->affichage)
                {
                    get_position_avion(&c0,avion1,donnees->temps);
                    gdk_draw_rectangle(carte,gc,TRUE,conversion_longitude(c0.x,donnees)*donnees->xcarte-3,conversion_lat(c0.y,donnees)*donnees->ycarte-3,7,7);
                }

                if(avion1->affichage)
                {
                    get_position_avion(&c0,avion2,donnees->temps);
                    gdk_draw_rectangle(carte,gc,TRUE,conversion_longitude(c0.x,donnees)*donnees->xcarte-3,conversion_lat(c0.y,donnees)*donnees->ycarte-3,7,7);
                }

                    char *markup;
                    markup = g_markup_printf_escaped ("<span foreground=\"#A00000\">%s</span>", "Conflit.");
                    gtk_label_set_markup (GTK_LABEL (donnees->Msg_conflit), markup);
                    g_free (markup);
            }

        conflit_current = conflit_current->ptsuiv;
        }

    if(conflit_affiche==0)
    {
        char *markup;
        markup = g_markup_printf_escaped ("<span foreground=\"#00A000\">%s</span>", "Pas de conflit.");
        gtk_label_set_markup (GTK_LABEL (donnees->Msg_conflit), markup);
        g_free (markup);
//        gtk_label_set_text(GTK_LABEL(donnees->Msg_conflit),"centre2");
    }

    couleur(gc,c,-1);
    }


    gdk_font_unref(font);
}


void couleur(GdkGC* gc,GdkColor c,int altitude)
{
c.pixel = 5;

if(0 < altitude && altitude < 140)
{
c.red = (150 - altitude )*5 << 8 ;
c.green = 10 << 8;
c.blue = 10 << 8;
//g_print("avion en : rouge \n");
}
else if(140 < altitude && altitude < 180)
{
c.red = 10 << 8 ;
c.green = (altitude -180)*5 << 8;
c.blue = 10 << 8;
//g_print("avion en : vert \n");
}
else if(180 < altitude && altitude < 300)
{
c.red = 10 << 8 ;
c.green = 10 << 8;
c.blue = (190 - altitude)*5 << 8;
//g_print("avion en : bleue \n");
}
else if(altitude==-1)
{
c.red = 0 << 8 ;
c.green = 0 << 8;
c.blue = 0 << 8;
//g_print("avion en : noir \n\n");
}
else if(altitude==-2)
{
c.red = 150 << 8 ;
c.green = 150 << 8;
c.blue = 200 << 8;
}
else if(altitude==-3)
{
c.red = 250 << 8 ;
c.green = 0 << 8;
c.blue = 0 << 8;
}

gdk_gc_set_rgb_fg_color (gc, &c);

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
//     loc->x = (loc->x + (delta_pass->x) * facteur_temps) * donnees->xcarte ;
//     loc->y = (loc->y + (delta_pass->y) * facteur_temps) * donnees->ycarte ;
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
