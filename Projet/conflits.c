#include "conflits.h"



void detection_conflits(GtkWidget *button, file_opener * donnees)
{
    position* pos1=malloc(sizeof(position));
    position* pos2=malloc(sizeof(position));
    pdv* pdv1=donnees->debutpdv;

    donnees->deb_conflits=malloc(sizeof(conflit));
    conflit* conflit_current=donnees->deb_conflits;
    conflit_current->ptsuiv=NULL;
    while(pdv1!=NULL)
    {
        pdv* pdv2=pdv1->ptsuiv;
        while(pdv2!=NULL)
        {
        int dverticale;
        if(pdv1->altitude>410)      //réglage de la distance de conflit verticale en fonction de l'altitude
        {
            dverticale=10;
        }
        else
        {
            dverticale=20;
        }
//g_print("pdv1:%s\npdv2:%s\n",pdv1->nom,pdv2->nom);
            if(abs(pdv1->altitude-pdv2->altitude)<dverticale) //on ne détecte les conflits que si les vols sont à la même altitude
            {
                double t;
                int conf=0;
                int memconflit=0;
                double td=pdv1->temps_depart;
                if(pdv2->temps_depart>td)
                {
                    td=pdv2->temps_depart;
                }

                double ta=pdv1->temps_arrivee;
                if(pdv2->temps_depart>ta)
                {
                    ta=pdv2->temps_arrivee;
                }


                for(t=td;t<=ta;t+=donnees->deltat_conflits)
                {
                    get_position_avion(pos1,pdv1,t);
                    get_position_avion(pos2,pdv2,t);
                    double lat1=pos1->y;
                    double long1=pos1->x;

//g_print("x1=%lf,y1=%lf\n",pos1->x,pos1->y);
//g_print("x2=%lf,y2=%lf\n",pos2->x,pos2->y);
                       // double D=sqrt(pow((pos2->x-pos1->x)*680,2)+pow((pos2->y-pos1->y)*660,2));

                        double dlat=3340*3.14/180*(pos2->y-pos1->y);                             //distance projeté sur un méridien en NM,  rayon de la terre = 6371km = 3340NM
                        double latm=(pos2->y+pos1->y)/2;
                        double r=3340*cos(latm*3.14/180);
                        double dlong=r*3.14*(pos2->x-pos1->x)/180.0;               //distance projeté sur l'autre axe en NM
                        double D=sqrt(pow(dlat,2)+pow(dlong,2));                //distance entre les 2 points en NM (approximation: rayon de la terre infini)
//g_print("D=%lf\n",D);

                        if(D<donnees->distance_conflit)
                        {
                            conf=1;
                            if(conf!=memconflit)
                            {
                                conflit_current->latitude=lat1;
                                conflit_current->longitude=long1;
                                conflit_current->pdv1=pdv1;
                                conflit_current->pdv2=pdv2;
                                conflit_current->temps_deb=t;


//g_print("CONFLIT entre %s et %s à %02d:%02d |%lf  %lf  D=%lf\n",conflit_current->pdv1->nom,conflit_current->pdv2->nom,h,m,conflit_current->latitude,conflit_current->longitude,conflit_current->D);


                            }
                            //memconflit=conf;
                        }
                        else
                        {
                            conf=0;
                        }

                        if(conf!=memconflit && !conf)
                        {
//g_print("plus de conflit! t=%lf\n",t);

                            conflit_current->temps_fin=t;

                            conflit_current->ptsuiv=malloc(sizeof(conflit));
                            conflit_current=conflit_current->ptsuiv;
                            conflit_current->ptsuiv=NULL;
                        }
                        memconflit=conf;
                }
            }
            pdv2=pdv2->ptsuiv;


        }

        pdv1=pdv1->ptsuiv;


//g_print("\n\n\n\n");
    }

    free(pos1);
    free(pos2);
}








void get_position_avion(position* pos, pdv* pdv_c,double t)
{
    if(t<pdv_c->temps_depart || t>pdv_c->temps_arrivee)
    {
        pos->y=-100;
//g_print("Avion pas partis, ou déja arrivé\n");
    }
    else
    {
        pt_pass* pass_c=pdv_c->pass_debut;
        while(t>pass_c->ptsuiv->temps)
        {
            pass_c=pass_c->ptsuiv;
        }
        double dt=t-pass_c->temps;


        double lat1,lat2,long1,long2;

            if(pass_c->type_point)
            {
                balise* pt=pass_c->point;
                lat1=pt->latitude;
                long1=pt->longitude;
            }
            else
            {
                aerodrome* pt=pass_c->point;
                lat1=pt->latitude;
                long1=pt->longitude;
            }

            if(pass_c->ptsuiv->type_point)
            {
                balise* pt=pass_c->ptsuiv->point;
                lat2=pt->latitude;
                long2=pt->longitude;
            }
            else
            {
                aerodrome* pt=pass_c->ptsuiv->point;
                lat2=pt->latitude;
                long2=pt->longitude;
            }

            double dlat=3340*3.14/180*(lat2-lat1);                             //distance projeté sur un méridien en NM,  rayon de la terre = 6371km = 3340NM
            double latm=(lat1+lat2)/2;
            double r=3340*cos(latm*3.14/180);
            double dlong=r*3.14*(long2-long1)/180.0;               //distance projeté sur l'autre axe en NM
            double D=sqrt(pow(dlat,2)+pow(dlong,2));

        double d=pdv_c->vitesse/60*dt;

        pos->y=lat1+d/D*(lat2-lat1);
        pos->x=long1+d/D*(long2-long1);                     //pas vraiment ça mais c'est compliqué...

//g_print("%lf   %lf\n",pos->x,pos->y);
    }
}












void integrer_temps(file_opener* donnees)
{

    pdv* pdv_current=donnees->debutpdv;
    while(pdv_current!=NULL)
    {
        pdv_current->temps_depart=60*pdv_current->heure+pdv_current->minute;
        pt_pass* pass_current=pdv_current->pass_debut;
        pass_current->temps=pdv_current->temps_depart;
        while(pass_current->ptsuiv->ptsuiv!=NULL)
        {
            double lat1,lat2,long1,long2;
            if(pass_current->type_point)
            {
                balise* pt=pass_current->point;
                lat1=pt->latitude;
                long1=pt->longitude;
//g_print("lat,long %s",pt->nom);
            }
            else
            {
                aerodrome* pt=pass_current->point;

                lat1=pt->latitude;
                long1=pt->longitude;
//g_print("lat,long %s",pt->oaci);
            }

            if(pass_current->ptsuiv->type_point)
            {
                balise* pt=pass_current->ptsuiv->point;
                lat2=pt->latitude;
                long2=pt->longitude;
//g_print("  %s",pt->nom);
            }
            else
            {
                aerodrome* pt=pass_current->ptsuiv->point;
                lat2=pt->latitude;
                long2=pt->longitude;
//g_print("   %s",pt->oaci);
            }

            //double D=sqrt(pow((x2-x1)*680,2)+pow((y2-y1)*660,2));
            double dlat=3340*3.14/180*(lat2-lat1);                             //distance projeté sur un méridien en NM,  rayon de la terre = 6371km = 3340NM
            double latm=(lat1+lat2)/2;
            double r=3340*cos(latm*3.14/180);
            double dlong=r*3.14*(long2-long1)/180.0;               //distance projeté sur l'autre axe en NM
            double D=sqrt(pow(dlat,2)+pow(dlong,2));                //distance entre les 2 points en NM (approximation: rayon de la terre infini)

//g_print(" = %lf   %lf   D=%lf   latm=%lf\n",dlat,dlong,D,latm);
//g_print("%lf   %lf   r=%lf\n\n",long1,long2,r);
//g_print("D=%lf\n",D);
            pass_current->ptsuiv->temps=pass_current->temps+D/(pdv_current->vitesse/60);
//g_print("t=%lf\n",pass_current->ptsuiv->temps);


            pass_current=pass_current->ptsuiv;

        }
        pdv_current->temps_arrivee=pass_current->temps;
//g_print("arrivée:%lf\n",pdv_current->temps_arrivee);
        pdv_current=pdv_current->ptsuiv;
    }
    detection_conflits(NULL,donnees);

}





double conversion_lat(double latitude, file_opener *donnees)
{
    double r=(donnees->latitude_max-latitude)/donnees->dlat;
    return r;
}


double conversion_longitude(double longitude, file_opener *donnees)
{
    double r=(-donnees->longitude_min+longitude)/donnees->dlong;
    return r;
}
