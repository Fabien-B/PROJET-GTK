#include "filtrage.h"
#include "ouverture_fichiers.h"

void filtres(GtkWidget* button, file_opener* donnees)
{
    GtkWidget *filw;
    GtkWidget* box;
    GtkWidget *scrollbar;
    GtkWidget* boxaero;
    GtkWidget* boxbalises;
    GtkWidget* boxpdv;
    GtkWidget* box1;
    //GtkWidget* actualiserbt;
    //init fenêtre
    filw = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(GTK_WINDOW(filw), "FILTRES");
    gtk_window_set_default_size(GTK_WINDOW(filw), 10, 500);
    gtk_window_set_position(GTK_WINDOW(filw),GTK_WIN_POS_CENTER);

    //init scrollbar
    scrollbar = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(filw),scrollbar);

    //création et ajout d'une boite dans la fenètre scroll
    box1=gtk_vbox_new(FALSE,0);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrollbar), box1);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbar), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS); //Never: désactive la barre, Always, l'inverse

//    actualiserbt=gtk_button_new_with_label("Actualiser l'affichage");
//    gtk_box_pack_start(GTK_BOX(box1),actualiserbt,FALSE,FALSE,0);
//    g_signal_connect(G_OBJECT(actualiserbt), "clicked", G_CALLBACK(redessiner_widget), donnees->carte);

    box=gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(box1), box, FALSE, FALSE, 0);


    boxaero=gtk_vbox_new(FALSE,0);      //3 boites pour les aérodromes, les balises et les plan de vols
    boxbalises=gtk_vbox_new(FALSE,0);
    boxpdv=gtk_vbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(box), boxaero, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), boxbalises, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), boxpdv, FALSE, FALSE, 0);




if(donnees->debutaero!=NULL)
{
    GtkWidget* lab;
    GtkWidget* saa;
    GtkWidget* is;

    lab=gtk_label_new("Aérodromes"); //un label et deux boutons pour aider à la sélection
    saa=gtk_button_new_with_label("sélectionner/(D) tout");
    is=gtk_button_new_with_label("inverser la sélection");
    gtk_box_pack_start(GTK_BOX(boxaero),lab,FALSE,FALSE,0);
    gtk_box_pack_start(GTK_BOX(boxaero),saa,FALSE,FALSE,0);
    gtk_box_pack_start(GTK_BOX(boxaero),is,FALSE,FALSE,0);
    g_signal_connect(G_OBJECT(saa), "clicked", G_CALLBACK(select_all_aero), donnees);
    g_signal_connect(G_OBJECT(is), "clicked", G_CALLBACK(invert_selection_aero), donnees);

    aerodrome* pt_current=donnees->debutaero;
    while(pt_current->ptsuiv!=NULL)             //création et initialisation des checkbox
    {
        char label[100];
        sprintf(label,"%s\n",pt_current->nom);
        pt_current->coch=gtk_check_button_new_with_label(label);
        gtk_box_pack_start(GTK_BOX(boxaero),pt_current->coch,FALSE,FALSE,0);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pt_current->coch), (pt_current->affichage));
        g_signal_connect(G_OBJECT(pt_current->coch), "toggled", G_CALLBACK(check_aero), pt_current);
        g_signal_connect(G_OBJECT(pt_current->coch), "toggled", G_CALLBACK(redessiner_widget), donnees->carte);
        pt_current=pt_current->ptsuiv;
    }


}

if(donnees->debutbalises!=NULL)
{
    GtkWidget* lab;
    GtkWidget* sab;
    GtkWidget* is;

    lab=gtk_label_new("Balises");                    //un label et deux boutons pour aider à la sélection
    sab=gtk_button_new_with_label("sélectionner/(D) tout");
    is=gtk_button_new_with_label("inverser la sélection");
    gtk_box_pack_start(GTK_BOX(boxbalises),lab,FALSE,FALSE,0);
    gtk_box_pack_start(GTK_BOX(boxbalises),sab,FALSE,FALSE,0);
    gtk_box_pack_start(GTK_BOX(boxbalises),is,FALSE,FALSE,0);
    g_signal_connect(G_OBJECT(sab), "clicked", G_CALLBACK(select_all_balises), donnees);
    g_signal_connect(G_OBJECT(is), "clicked", G_CALLBACK(invert_selection_balise), donnees);

    balise* pt_current=donnees->debutbalises;
    while(pt_current->ptsuiv!=NULL)             //création et initialisation des checkbox
    {
        char label[100];
        sprintf(label,"%s\n",pt_current->nom);
        pt_current->coch=gtk_check_button_new_with_label(label);
        gtk_box_pack_start(GTK_BOX(boxbalises),pt_current->coch,FALSE,FALSE,0);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pt_current->coch), (pt_current->affichage));
        g_signal_connect(G_OBJECT(pt_current->coch), "toggled", G_CALLBACK(check_balise), pt_current);
        g_signal_connect(G_OBJECT(pt_current->coch), "toggled", G_CALLBACK(redessiner_widget), donnees->carte);
        pt_current=pt_current->ptsuiv;
    }


}

if(donnees->debutpdv!=NULL)
{
    GtkWidget* lab;
    GtkWidget* sap;
    GtkWidget* is;

    lab=gtk_label_new("Plans de vols");                    //un label et deux boutons pour aider à la sélection
    sap=gtk_button_new_with_label("sélectionner/(D) tout");
    is=gtk_button_new_with_label("inverser la sélection");
    gtk_box_pack_start(GTK_BOX(boxpdv),lab,FALSE,FALSE,0);
    gtk_box_pack_start(GTK_BOX(boxpdv),sap,FALSE,FALSE,0);
    gtk_box_pack_start(GTK_BOX(boxpdv),is,FALSE,FALSE,0);
    g_signal_connect(G_OBJECT(sap), "clicked", G_CALLBACK(select_all_pdv), donnees);
    g_signal_connect(G_OBJECT(is), "clicked", G_CALLBACK(invert_selection_pdv), donnees);

    pdv* pt_current=donnees->debutpdv;
    while(pt_current!=NULL)                 //création et initialisation des checkbox
    {

        char label[100];

        sprintf(label,"%s\n", pt_current->nom);
        pt_current->coch=gtk_check_button_new_with_label(label);
        gtk_box_pack_start(GTK_BOX(boxpdv),pt_current->coch,FALSE,FALSE,0);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pt_current->coch), (pt_current->affichage));
        g_signal_connect(G_OBJECT(pt_current->coch), "toggled", G_CALLBACK(check_pdv), pt_current);
        g_signal_connect(G_OBJECT(pt_current->coch), "toggled", G_CALLBACK(redessiner_widget), donnees->carte);
        pt_current=pt_current->ptsuiv;
    }


}



if(donnees->debutaero==NULL && donnees->debutbalises==NULL && donnees->debutpdv==NULL)
{
    GtkWidget* lab;
    lab=gtk_label_new("Aucun fichier n'a été chargé !");
    gtk_box_pack_start(GTK_BOX(boxpdv),lab,FALSE,FALSE,0);

}
    gtk_widget_show_all(filw);  //afficher la fenètre



}

void select_all_aero(GtkWidget* button, file_opener* donnees)       //fonction pour sélectionner/désélectionner tous les aérodromes
{
    aerodrome* pt_current=donnees->debutaero;

    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pt_current->coch)))       //on se base sur l'état du 1er pour savoir si on coche ou on décoche
    {
        while(pt_current->ptsuiv!=NULL)
        {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pt_current->coch), (FALSE));
            pt_current->affichage=0;
            pt_current=pt_current->ptsuiv;
        }
    }
    else
    {
        while(pt_current->ptsuiv!=NULL)
            {
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pt_current->coch), (TRUE));
                pt_current->affichage=1;
                //printf("ok\n");
                pt_current=pt_current->ptsuiv;
            }
    }


}


void invert_selection_aero(GtkWidget* button, file_opener* donnees)     //inverser la sélection des aérodromes
{
    aerodrome* pt_current=donnees->debutaero;
    while(pt_current->ptsuiv!=NULL)
        {
            gboolean bEtat;
            bEtat = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pt_current->coch));
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pt_current->coch), (bEtat ^ TRUE));
            pt_current=pt_current->ptsuiv;
        }

}



void select_all_balises(GtkWidget* button, file_opener* donnees)       //fonction pour sélectionner/désélectionner toutes les balises
{
    balise* pt_current=donnees->debutbalises;

    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pt_current->coch)))       //on se base sur l'état du 1er pour savoir si on coche ou on décoche
    {
        while(pt_current->ptsuiv!=NULL)
        {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pt_current->coch), (FALSE));
            pt_current->affichage=0;
            pt_current=pt_current->ptsuiv;
        }
    }
    else
    {
        while(pt_current->ptsuiv!=NULL)
            {
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pt_current->coch), (TRUE));
                pt_current->affichage=1;
                pt_current=pt_current->ptsuiv;
            }
    }

}

void invert_selection_balise(GtkWidget* button, file_opener* donnees)     //inverser la sélection des balises
{
    balise* pt_current=donnees->debutbalises;
    while(pt_current->ptsuiv!=NULL)
        {
            gboolean bEtat;
            bEtat = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pt_current->coch));
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pt_current->coch), (bEtat ^ TRUE));
            pt_current=pt_current->ptsuiv;
        }

}

void select_all_pdv(GtkWidget* button, file_opener* donnees)       //fonction pour sélectionner/désélectionner tous les plans de vols
{
    pdv* pt_current=donnees->debutpdv;

    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pt_current->coch)))       //on se base sur l'état du 1er pour savoir si on coche ou on décoche
    {
        while(pt_current!=NULL)
        {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pt_current->coch), (FALSE));
            pt_current->affichage=0;
            pt_current=pt_current->ptsuiv;
        }
    }
    else
    {
        while(pt_current!=NULL)
            {
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pt_current->coch), (TRUE));
                pt_current->affichage=1;
                pt_current=pt_current->ptsuiv;
            }
    }

}

void invert_selection_pdv(GtkWidget* button, file_opener* donnees)     //inverser la sélection des plans de vols
{
    pdv* pt_current=donnees->debutpdv;
    while(pt_current!=NULL)
        {
            gboolean bEtat;
            bEtat = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pt_current->coch));
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pt_current->coch), (bEtat ^ TRUE));
            pt_current=pt_current->ptsuiv;
        }

}

void check_aero(GtkWidget *pToggle, aerodrome* pt_current)          //mise à jour de la variable affichage quand l'utilisateur coche/décoche (aérodromes)
{
    gboolean bEtat;
    bEtat = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pToggle));    // Récupération de l'état du bouton

    if(bEtat)
    {
        pt_current->affichage=1;
    }
    else
    {
        pt_current->affichage=0;
    }

}

void check_balise(GtkWidget *pToggle, balise* pt_current)          //mise à jour de la variable affichage quand l'utilisateur coche/décoche (balises)
{
    gboolean bEtat;
    bEtat = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pToggle));    // Récupération de l'état du bouton

    if(bEtat)
    {
        pt_current->affichage=1;
    }
    else
    {
        pt_current->affichage=0;
    }

}

void check_pdv(GtkWidget *pToggle, pdv* pt_current)          //mise à jour de la variable affichage quand l'utilisateur coche/décoche (plan de vols)
{
    gboolean bEtat;
    bEtat = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pToggle));    // Récupération de l'état du bouton

    if(bEtat)
    {
        pt_current->affichage=1;
    }
    else
    {
        pt_current->affichage=0;
    }

}



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
                for(t=0;t<TEMPS_SIMULATION;t+=donnees->deltat_conflits)
                {
                    get_position_avion(pos1,pdv1,t);
                    get_position_avion(pos2,pdv2,t);
                    double lat1=pos1->y;
                    double long1=pos1->x;

                    if(pos1->y>=0 && pos2->y>=0)  //si les avions sont en vols (etat=-1 si les avions n'ont pas décollés ou déja atterri)
                    {
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
                            g_print("Conflit entre %s et %s D = %lf, d sécu = %d,  t=%lf\n",pdv1->nom,pdv2->nom,D,donnees->distance_conflit,t);
                            conf=1;
                            if(conf!=memconflit)
                            {
//int h=t/60;
//int m=t-h*60;
                                conflit_current->D=D;
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
                            g_print("plus de conflit! t=%lf\n",t);
                            conflit_current->temps_fin=t;

                            conflit_current->ptsuiv=malloc(sizeof(conflit));
                            conflit_current=conflit_current->ptsuiv;
                            conflit_current->ptsuiv=NULL;
                        }
                        memconflit=conf;
                    }
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


       // double x1,x2,y1,y2;
        double lat1,lat2,long1,long2;

            if(pass_c->type_point)
            {
                balise* pt=pass_c->point;
                lat1=pt->latitude;
                long1=pt->longitude;
                //x1=pt->pos_x;
                //y1=pt->pos_y;
            }
            else
            {
                aerodrome* pt=pass_c->point;
                lat1=pt->latitude;
                long1=pt->longitude;
                //x1=pt->pos_x;
                //y1=pt->pos_y;
            }

            if(pass_c->ptsuiv->type_point)
            {
                balise* pt=pass_c->ptsuiv->point;
                lat2=pt->latitude;
                long2=pt->longitude;
                //x2=pt->pos_x;
                //y2=pt->pos_y;
            }
            else
            {
                aerodrome* pt=pass_c->ptsuiv->point;
                lat2=pt->latitude;
                long2=pt->longitude;
                //x2=pt->pos_x;
                //y2=pt->pos_y;
            }

            //double D=sqrt(pow((x2-x1)*680,2)+pow((y2-y1)*660,2));
            double dlat=3340*3.14/180*(lat2-lat1);                             //distance projeté sur un méridien en NM,  rayon de la terre = 6371km = 3340NM
            double latm=(lat1+lat2)/2;
            double r=3340*cos(latm*3.14/180);
            double dlong=r*3.14*(long2-long1)/180.0;               //distance projeté sur l'autre axe en NM
            double D=sqrt(pow(dlat,2)+pow(dlong,2));

        double d=pdv_c->vitesse/60*dt;

        pos->y=lat1+d/D*(lat2-lat1);
        pos->x=long1+d/D*(long2-long1);                     //pas vraiment ça mais c'est compliqué...
        //pos->x=x1+d/D*(x2-x1);
        //pos->y=y1+d/D*(y2-y1);

//g_print("%lf   %lf\n",pos->x,pos->y);
    }
}
