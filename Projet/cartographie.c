#include "interface.h"
#include "ouverture_fichiers.h"
#include "cartographie.h"
#include "filtrage.h"
#include "ajouts_utilisateur.h"
#include "conflits.h"



// Fonction de rafraichissement
gboolean expose_cb (GtkWidget *carte, GdkEventExpose *event, file_opener * donnees)
{
  dessiner (donnees);

  return FALSE;
}


void dessiner(file_opener *donnees)
{
    GdkDrawable* cartew=donnees->carte->window;
    donnees->gc = gdk_gc_new (cartew);
    // On initialise la police et la couleur
    donnees->font=gdk_font_load("6x9");//6x9 - 10x20
int j;
char text[5];

couleur(donnees->gc,donnees->c,-2);
// On trace les parallèles
for(j=35;j<=60;j+=5)
{
sprintf(text,"%d",j);
gdk_draw_line(cartew,donnees->gc,0,conversion_lat(j,donnees)*donnees->ycarte,donnees->xcarte*3,conversion_lat(j,donnees)*donnees->ycarte);
gdk_draw_string(cartew,donnees->font,donnees->gc,3,conversion_lat(j,donnees)*donnees->ycarte-5,text);
}

// Puis les méridiens
for(j=-15;j<=20;j+=5)
{
sprintf(text,"%d",j);
gdk_draw_line(cartew,donnees->gc,conversion_longitude(j,donnees)*donnees->xcarte,0,conversion_longitude(j,donnees)*donnees->xcarte,donnees->ycarte*3);
gdk_draw_string(cartew,donnees->font,donnees->gc,conversion_longitude(j,donnees)*donnees->xcarte+5,8,text);
}
couleur(donnees->gc,donnees->c,10);

 /* ----------------------------------  CONTOUR FRANCE -------------------------------------- */


    {
        double x1,x2,y1,y2;
        // On ouvre le fichier contenant les points
        FILE *f = fopen("contour_france.csv", "rb");
        //printf("f=%p\n", f);
        if(f)
        {
            char buffer[1024];

            while(fgets(buffer, sizeof(buffer), f))
            {
            //  On charge et on converti les points.
                sscanf(buffer, "%lf;%lf", &x1, &y1);
                x1 = conversion_longitude(x1, donnees);
                y1 = conversion_lat(y1, donnees);
                while(fgets(buffer, sizeof(buffer), f))
                {
                //  Si la ligne est "BREAK" on empeche de tracer la ligne afin de retirer les traits entre les iles
                    if(!strcmp(buffer, "BREAK\n"))
                    {
                        break;
                    }

                    sscanf(buffer, "%lf;%lf", &x2, &y2);
                    x2=conversion_longitude(x2, donnees);
                    y2=conversion_lat(y2, donnees);
                    gdk_draw_line(cartew, donnees->gc, x1*donnees->xcarte,y1*donnees->ycarte,x2*donnees->xcarte,y2*donnees->ycarte);
                    x1=x2;
                    y1=y2;
                }
            }
            fclose(f);
        }
    }

 // ----------------------------------  AERODROMES --------------------------------------
// Si des aérodromes sont chargé on commence l'affichage
   if(donnees->debutaero!=NULL)
    {

        aerodrome* pt_aero=donnees->debutaero;
        // Boucle pour parcourir tous les aérodromes
        while(pt_aero->ptsuiv!=NULL)
        {
            dessiner_aerodromes(pt_aero,donnees);
            pt_aero = pt_aero->ptsuiv;
        }
    }

 // ----------------------------------  BALISES --------------------------------------
// La structure est semblable à celle des aérodromes
    if(donnees->debutbalises!=NULL)
    {
        balise* pt_balise=donnees->debutbalises;

        while(pt_balise->ptsuiv!=NULL)
        {
            dessiner_balise(pt_balise,donnees);
            pt_balise = pt_balise->ptsuiv;
        }
    }




 // ----------------------------------  PLAN DE VOL --------------------------------------
// On initialise la couleur à 0 pour donner une couleur à chaque plan de vol
donnees->couleur_avion=0;

    if(donnees->debutpdv!=NULL) // Rentre si un plan de vol est chargé
    {
    pdv * pt_pdv_current=donnees->debutpdv;

        while(pt_pdv_current!=NULL) // Parcours tous les plans de vol
        {
            dessiner_pdv(pt_pdv_current,donnees);
            pt_pdv_current=pt_pdv_current->ptsuiv;
            donnees->couleur_avion+=1;
        }
    }

/* ------------------------------------ CONFLITS  ---------------------------------------- */

// Si il y a des conflits
    if(donnees->deb_conflits!=NULL)
    {
    // On passe en couleur rouge
    couleur(donnees->gc,donnees->c,-3);
    conflit* conflit_current = donnees->deb_conflits;
    // Variable d'affichage si reste nul à la fin de la boucle pas de conflit
    int conflit_affiche = 0;
    //  On parcours les conflits
        while(conflit_current->ptsuiv != NULL)
        {
        pdv* avion1 = conflit_current->pdv1;
        pdv* avion2 = conflit_current->pdv2;

            // On se situe entre le temps de début et de fin de conflit
            if(donnees->temps > conflit_current->temps_deb && donnees->temps < conflit_current->temps_fin)
            {
                position c0; // position au temps t
                if(avion1->affichage && avion2->affichage) // Les deux avions sont affichés
                {
                // Conflits affiché conflit_affiche est différent de 0
                    conflit_affiche++;

                //  On met un gros carré rouge sur la position de l'avion
                    get_position_avion(&c0,avion1,donnees->temps);
                    gdk_draw_rectangle(cartew,donnees->gc,TRUE,conversion_longitude(c0.x,donnees)*donnees->xcarte-3,conversion_lat(c0.y,donnees)*donnees->ycarte-3,7,7);

                    get_position_avion(&c0,avion2,donnees->temps);
                    gdk_draw_rectangle(cartew,donnees->gc,TRUE,conversion_longitude(c0.x,donnees)*donnees->xcarte-3,conversion_lat(c0.y,donnees)*donnees->ycarte-3,7,7);

                //  On change le label de conflit
                    char *markup;
                    markup = g_markup_printf_escaped ("<span foreground=\"#A00000\">%s</span>", "Conflit.");
                    gtk_label_set_markup (GTK_LABEL (donnees->Msg_conflit), markup);
                    g_free (markup);
                }
            }
        conflit_current = conflit_current->ptsuiv;
        }
// Si aucun conflit n'est affiché on passe au label pas de conflit
    if(conflit_affiche==0)
    {
        char *markup;
        markup = g_markup_printf_escaped ("<span foreground=\"#00A000\">%s</span>", "Pas de conflit\n actuellement.");
        gtk_label_set_markup (GTK_LABEL (donnees->Msg_conflit), markup);
        g_free (markup);
    }
    // On remet en noir
    couleur(donnees->gc,donnees->c,10);
    }
    gdk_font_unref(donnees->font);
    g_object_unref (donnees->gc);
}


void couleur(GdkGC* gc,GdkColor c,int indice)
{
c.pixel = 5;

if(0 <= indice)
{
indice = indice%11;
// Permet de donner 10 couleur différentes pour les plans de vol ainsi que de repasser en noir
    switch(indice)
    {
    case 0 : gdk_color_parse ("#007fff", &c);break;//bleu
    case 1 : gdk_color_parse ("#e67e30", &c);break;//orange
    case 2 : gdk_color_parse ("#7fdd4c", &c);break;//vert
    case 3 : gdk_color_parse ("#884da7", &c);break;//violet
    case 4 : gdk_color_parse ("#5b3c11", &c);break;//marron
    case 5 : gdk_color_parse ("#008e8e", &c);break;//bleu-vert
    case 6 : gdk_color_parse ("#eed153", &c);break;//jaune+
    case 7 : gdk_color_parse ("#175732", &c);break;//vert+
    case 8 : gdk_color_parse ("#ec3b83", &c);break;//rose
    case 9 : gdk_color_parse ("#2e006c", &c);break;//bleu+
    case 10 : gdk_color_parse("black",&c);break;//noir
    }
}
// Bleue sombre
else if(indice==-2)
{
gdk_color_parse("#9696c8",&c);
}
// Rouge
else if(indice==-3)
{
gdk_color_parse("red",&c);
}

gdk_gc_set_rgb_fg_color (gc, &c);

}















void dessiner_pdv(pdv* pt_pdv_current, file_opener* donnees)
{
    GdkDrawable* cartew=donnees->carte->window;
            if(pt_pdv_current->affichage==1) // Vérifie si le plan de vol doit être affiché
            {
            position* loc_avion=malloc(sizeof(position));
//            Position_avion(donnees,pt_pdv_current,loc_avion);
            get_position_avion(loc_avion,pt_pdv_current,donnees->temps);
            loc_avion->y=conversion_lat(loc_avion->y,donnees)*donnees->ycarte;
            loc_avion->x=conversion_longitude(loc_avion->x,donnees)*donnees->xcarte;

        //  On change la couleur du tracé pour le carré et le nom de l'avion
            couleur(donnees->gc,donnees->c,donnees->couleur_avion);
            gdk_draw_rectangle(cartew,donnees->gc,TRUE,loc_avion->x-2,loc_avion->y-2,5,5);
            gdk_draw_string(cartew,donnees->font,donnees->gc,loc_avion->x+3,loc_avion->y-2,pt_pdv_current->nom);
            couleur(donnees->gc,donnees->c,10);


            int x1=-9,x2=0,y1=0,y2=0; // Variables de stockage des coordonnées pour les points de passage

            pt_pass * pt_pass_current = pt_pdv_current->pass_debut;

                if(x1==-9 && pt_pass_current->ptsuiv!=NULL) // Cas du premier point
                {

                    if(pt_pass_current->type_point==0) // Si le point est un aérodrome
                    {
                        aerodrome * pass_aerodrome = pt_pass_current->point;
                        //printf("\n AERODROME 1 \n");
                        x1=conversion_longitude(pass_aerodrome->longitude,donnees)* donnees->xcarte;
                        y1=conversion_lat(pass_aerodrome->latitude,donnees)* donnees->ycarte;

                        // Affichage de l'aérodrome
                        pass_aerodrome->affichage=1;
                        dessiner_aerodromes(pass_aerodrome,donnees);
                    }

                    if(pt_pass_current->type_point==1) // Si le point est une balise
                    {
                        balise * pass_balise = pt_pass_current->point;
                       // printf("\n BALISE 1 \n");
                        x1=conversion_longitude(pass_balise->longitude,donnees)* donnees->xcarte;
                        y1=conversion_lat(pass_balise->latitude,donnees)* donnees->ycarte;

                        // Affichage de la balise
                        pass_balise->affichage=1;
                        dessiner_balise(pass_balise,donnees);
                    }

                    if(pt_pass_current->type_point==2) // Si le point est une ptgpx
                    {
                        ptgpx * pass = pt_pass_current->point;
                       // printf("\n BALISE 1 \n");
                        x1=conversion_longitude(pass->longitude,donnees)* donnees->xcarte;
                        y1=conversion_lat(pass->latitude,donnees)* donnees->ycarte;

                        // Affichage de la balise
                        //gdk_draw_string(carte,font,gc, x1 + 2,y1 + 7, pass_balise->nom);
                        gdk_draw_point(cartew,donnees->gc, x1,y1);
                    }

                pt_pass_current = pt_pass_current->ptsuiv;

                }


                while(pt_pass_current->ptsuiv!=NULL) // Parcours de tous les autres points de passage
                {


                    if(pt_pass_current->type_point==0) // Si le point est un aérodrome
                    {
                    aerodrome * pass_aerodrome = pt_pass_current->point;
                   // printf("\n AERODROME \n");
                    x2=conversion_longitude(pass_aerodrome->longitude,donnees)* donnees->xcarte;
                    y2=conversion_lat(pass_aerodrome->latitude,donnees)* donnees->ycarte;

                    // Affichage de l'aerodrome
                    pass_aerodrome->affichage=1;
                    dessiner_aerodromes(pass_aerodrome,donnees);
                    }

                    if(pt_pass_current->type_point==1) // Si le point est une balise
                    {
                    balise * pass_balise = pt_pass_current->point;
                  //  printf("\n BALISE \n");

                    x2=conversion_longitude(pass_balise->longitude,donnees)* donnees->xcarte;
                    y2=conversion_lat(pass_balise->latitude,donnees)* donnees->ycarte;

                    // Affichage de la balise
                    pass_balise->affichage=1;
                    dessiner_balise(pass_balise,donnees);
                    }

                    if(pt_pass_current->type_point==2) // Si le point est un ptgpx
                    {
                    ptgpx * pass = pt_pass_current->point;

                    x2=conversion_longitude(pass->longitude,donnees)* donnees->xcarte;
                    y2=conversion_lat(pass->latitude,donnees)* donnees->ycarte;

                    // Affichage de la balise
                    int x=conversion_longitude(pass->longitude,donnees)* donnees->xcarte;
                    int y=conversion_lat(pass->latitude,donnees)* donnees->ycarte;
                    //gdk_draw_string(carte,font,gc, x + 2,y + 7, pass_balise->nom);
                    gdk_draw_point(cartew,donnees->gc, x,y);
                    }



//                  Une couleur différente par avion
                    couleur(donnees->gc,donnees->c,donnees->couleur_avion);

//                  Trace la ligne et remet la couleur normalement.
                    gdk_draw_line(cartew, donnees->gc, x1,y1,x2,y2);
                    couleur(donnees->gc,donnees->c,10);

                    // Le point 2 devient le point 1
                    x1=x2;
                    y1=y2;
                    pt_pass_current = pt_pass_current->ptsuiv;

                }

            free(loc_avion);
            }
}


void dessiner_aerodromes(aerodrome* pt_aero,file_opener* donnees)
{
    GdkDrawable* cartew=donnees->carte->window;
    //  Si l'affichage est autorisé
    if(pt_aero->affichage==1)
    {
    // On récupère on on converti les points
    double x=conversion_longitude(pt_aero->longitude,donnees);
    double y=conversion_lat(pt_aero->latitude,donnees);

    // On trace le point et le nom
    gdk_draw_string(cartew,donnees->font,donnees->gc, donnees->xcarte*x+2,donnees->ycarte*y + 7, pt_aero->oaci);
    gdk_draw_point(cartew,donnees->gc, donnees->xcarte*x,donnees->ycarte*y);
    }
    else
    {
      //  printf("ELSE aero : Je change pas car affichage = %d\n", pt_aero->affichage);
    }


}

void dessiner_balise(balise* pt_balise,file_opener* donnees)
{
    GdkDrawable* cartew=donnees->carte->window;
    if(pt_balise->affichage==1)
    {
     //   printf("IF balise : Je change car affichage = %d\n", pt_balise->affichage);
        double x=conversion_longitude(pt_balise->longitude,donnees);
        double y=conversion_lat(pt_balise->latitude,donnees);
        gdk_draw_string(cartew,donnees->font,donnees->gc, donnees->xcarte*x+2,donnees->ycarte*y + 7, pt_balise->nom);
        gdk_draw_point(cartew,donnees->gc, donnees->xcarte*x,donnees->ycarte*y);
    }
    else
    {
      //  printf("ELSE balise : Je change pas car affichage = %d\n", pt_balise->affichage);
    }

}


