#include "interface.h"
#include "ouverture_fichiers.h"

void creer_file_selection(file_opener *donnees)
{

    donnees->file_selection = gtk_file_selection_new( g_locale_to_utf8( "Sélectionnez un fichier", -1, NULL, NULL, NULL) );
    gtk_widget_show(donnees->file_selection);

    gtk_window_set_modal(GTK_WINDOW(donnees->file_selection), TRUE);

    g_signal_connect(G_OBJECT(GTK_FILE_SELECTION(donnees->file_selection)->ok_button), "clicked", G_CALLBACK(recuperer_chemin), donnees );

    g_signal_connect_swapped(G_OBJECT(GTK_FILE_SELECTION(donnees->file_selection)->cancel_button), "clicked", G_CALLBACK(gtk_widget_destroy), donnees->file_selection);
}

void recuperer_chemin(GtkWidget *bouton, file_opener *donnees)
{
    const gchar* chemin_fichier;
    chemin_fichier = gtk_file_selection_get_filename(GTK_FILE_SELECTION (donnees->file_selection) );

    donnees->ptchemin = malloc (sizeof *(donnees->ptchemin) * strlen (chemin_fichier) + 1);
    strcpy (donnees->ptchemin, chemin_fichier);

    gtk_widget_destroy(donnees->file_selection);

     charger_fichiers(donnees);
}

void lancer_boite(GtkWidget *bouton, file_opener *donnees)
{
    GtkWidget* pBoite;
    pBoite = gtk_dialog_new_with_buttons("sélection du fichier à charger",NULL,GTK_DIALOG_MODAL,GTK_STOCK_OK,GTK_RESPONSE_OK,NULL);


  GtkWidget *fixed;
  GtkWidget *combo;
  GtkWidget *label;


    label=gtk_label_new("sélectionnez quel fichier vous voulez charger.");
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(pBoite)->vbox), label, TRUE, TRUE, 0);

  fixed = gtk_fixed_new();

  combo = gtk_combo_box_new_text();
  gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "Aérodromes");
  gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "Balises");
  gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "Plans de vols");

  gtk_fixed_put(GTK_FIXED(fixed), combo, 50, 50);
  gtk_container_add(GTK_CONTAINER(GTK_DIALOG(pBoite)->vbox), fixed);


  g_signal_connect(G_OBJECT(combo), "changed",G_CALLBACK(combo_selected), donnees);

    gtk_combo_box_set_active(GTK_COMBO_BOX(combo),0); //mettre aérodrome en choix par défault.
    gtk_widget_show_all(GTK_DIALOG(pBoite)->vbox);


    if (gtk_dialog_run(GTK_DIALOG(pBoite))==GTK_RESPONSE_OK) //on ne sélectionne le fichier que si l'utilisateur clique sur OK.
    {
            creer_file_selection(donnees);

    }
    gtk_widget_destroy(pBoite);
}


void combo_selected(GtkWidget *widget,file_opener *donnees)
{
  gchar *text =  gtk_combo_box_get_active_text(GTK_COMBO_BOX(widget));

  if(!strcmp(text,"Aérodromes"))
  {
    donnees->what_file=0;
  }
  if(!strcmp(text,"Balises"))
  {
    donnees->what_file=1;
  }
  if(!strcmp(text,"Plans de vols"))
  {
    donnees->what_file=2;
  }
    //g_print("%d\n",donnees->what_file);

  g_free(text);
}







void charger_fichiers(file_opener *donnees)
{
//__________________________________________________________________________________________________________________________________________________________________________________________
//_____________________________________________________________________________aérodromes___________________________________________________________________________________________________

    if(donnees->what_file==0)
    {
    //g_print("aérodrome: %s\n",donnees->ptchemin);

        int j;
        FILE * fic=NULL;
        fic=fopen(donnees->ptchemin,"r");
        char ligne[50];
        if(fic!=NULL)
        {
            aerodrome * nouveau=malloc(sizeof(aerodrome));
            donnees->debutaero=nouveau;
            nouveau->ptsuiv=NULL;
            int cond;
            do
            {
                char chainetempo[60]; //initialisation d'une chaine de caractères temporaire

                j=0;
                int j0=0;
                fscanf(fic,"%[^\n]",ligne);
        //printf("%s\n",ligne);

                while(ligne[j]!=',')        //------------------------ lecture de la longitude ---------------------------//
                {
                    chainetempo[j-j0]=ligne[j];
                    j++;
                }
                chainetempo[j-j0]='\0';

                int k;
                for(k=0;k<strlen(chainetempo);k++) //remplacement du caractère '.' par ',' ...
                {
                    if(chainetempo[k]=='.')
                    {
                        chainetempo[k]=',';
                    }
                }
                sscanf(chainetempo, "%lf", &nouveau->longitude); //et conversion de la chaine de caractère en double
        //printf("latitude=%lf\n",nouveau->latitude);


                j+=2;                      //------------------------ lecture de la latitude ---------------------------//
                j0=j;
                while(ligne[j]!=',')
                {
                    chainetempo[j-j0]=ligne[j];
                    j++;
                }
                chainetempo[j-j0]='\0';

                for(k=0;k<strlen(chainetempo);k++)
                {
                    if(chainetempo[k]=='.')
                    {
                        chainetempo[k]=',';
                    }
                }
                sscanf(chainetempo, "%lf", &nouveau->latitude);
        //printf("longitude=%lf\n",nouveau->longitude);


                j+=3;                      //------------------------ lecture du nom de l'aérodrome ---------------------------//
                j0=j;
                while(ligne[j]!='(')
                {
                    nouveau->nom[j-j0]=ligne[j];
                    j++;
                }
                nouveau->nom[j-j0-1]='\0';
        //printf("nom=%s\n",nouveau->nom);

                j+=1;                      //------------------------ lecture du code OACI ---------------------------//
                j0=j;
                while(ligne[j]!=')')
                {
                    nouveau->oaci[j-j0]=ligne[j];
                    j++;
                }
                nouveau->oaci[j-j0]='\0';
//        printf("oaci=%s\n",nouveau->oaci);

        //printf("\n\n\n\n\n");


            nouveau->affichage=0; //non affiché par défault

            if(fgetc(fic)!=EOF)
            {
                cond=1;

                nouveau->ptsuiv=malloc(sizeof(aerodrome));
                nouveau=nouveau->ptsuiv;
                nouveau->ptsuiv=NULL;
            }
            else
            {
            cond=0;


           /* printf("\n\n\n\n\n\n\n"); /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            aerodrome *ptcurr=donnees->debutaero;
            while(ptcurr->ptsuiv!=NULL)
            {
                printf("%s\n",ptcurr->nom);
                ptcurr=ptcurr->ptsuiv;
            }/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/
            }

            }while(cond);
        fclose(fic);
        }
    }
//__________________________________________________________________________________________________________________________________________________________________________________________
//_____________________________________________________________________________balises______________________________________________________________________________________________________


    if(donnees->what_file==1)
    {
    //g_print("balises: %s\n",donnees->ptchemin);

        int j;
        FILE * fic=NULL;
        fic=fopen(donnees->ptchemin,"r");
        char ligne[50];
        if(fic!=NULL)
        {
            balise * nouveau=malloc(sizeof(balise));
            donnees->debutbalises=nouveau;
            nouveau->ptsuiv=NULL;

            int cond;
            do
            {
                nouveau->affichage=0;
                char chainetempo[20]; //initialisation d'une chaine de caractères temporaire

                j=0;
                int j0=0;
                fscanf(fic,"%[^\n]",ligne);
       //printf("%s\n",ligne);

                while(ligne[j]!=' ')                        //Nom
                {
                    nouveau->nom[j-j0]=ligne[j];
                    j++;
                }
                nouveau->nom[j-j0]='\0';

                j++;                                        //degres latitude
                j0=j;
                while(j<j0+2)
                {
                    chainetempo[j-j0]=ligne[j];
                    j++;
                }
                chainetempo[j-j0]='\0';
                sscanf(chainetempo, "%lf", &nouveau->latdeg);

                j+=2;                                        //minutes latitude
                j0=j;
                while(j<j0+2)
                {
                    chainetempo[j-j0]=ligne[j];
                    j++;
                }
                chainetempo[j-j0]='\0';
                sscanf(chainetempo, "%lf", &nouveau->latmin);

                j+=1;                                         //secondes latitude
                j0=j;
                while(j<j0+4)
                {
                    chainetempo[j-j0]=ligne[j];
                    j++;
                }
                chainetempo[j-j0]='\0';
                int k;
                for(k=0;k<strlen(chainetempo);k++)
                {
                    if(chainetempo[k]=='.')
                    {
                        chainetempo[k]=',';
                    }
                }
                sscanf(chainetempo, "%lf", &nouveau->latsec);

                if(ligne[j+2]=='N')                                        //latitude nord ou sud
                {
                    nouveau->dirlat=1;
                }
                else
                {
                    nouveau->dirlat=-1;
                }

                j+=3;
                j0=j;

                while(j<j0+4)                                        //longitude degres
                {
                    chainetempo[j-j0]=ligne[j];
                    j++;
                }
                chainetempo[j-j0]='\0';
                sscanf(chainetempo, "%lf", &nouveau->longdeg);

                j+=2;                                        //longitude minutes
                j0=j;
                while(j<j0+2)
                {
                    chainetempo[j-j0]=ligne[j];
                    j++;
                }
                chainetempo[j-j0]='\0';
                sscanf(chainetempo, "%lf", &nouveau->longmin);

                j+=1;                                        //longitude secondes
                j0=j;
                while(j<j0+4)
                {
                    chainetempo[j-j0]=ligne[j];
                    j++;
                }
                chainetempo[j-j0]='\0';
                for(k=0;k<strlen(chainetempo);k++)
                {
                    if(chainetempo[k]=='.')
                    {
                        chainetempo[k]=',';
                    }
                }
                sscanf(chainetempo, "%lf", &nouveau->longsec);

                if(ligne[j+2]=='E')                                        //longitude est ou ouest
                {
                    nouveau->dirlong=1;
                }
                else
                {
                    nouveau->dirlong=-1;
                }

                nouveau->latitude=nouveau->dirlat*(nouveau->latdeg+nouveau->latmin/60+nouveau->latsec/3600);
                nouveau->longitude=nouveau->dirlong*(nouveau->longdeg+nouveau->longmin/60+nouveau->longsec/3600);
                //printf("%lf     %lf\n",nouveau->latitude,nouveau->longitude);
       // printf("%s %d°%d'%lf [%d] %d°%d'%lf [%d]",nouveau->nom,nouveau->latdeg,nouveau->latmin,nouveau->latsec,nouveau->dirlat,nouveau->longdeg,nouveau->longmin,nouveau->longsec,nouveau->dirlong);
       // printf("\n\n\n\n\n");

            nouveau->affichage=0; //non affiché par défault
            if(fgetc(fic)!=EOF)
            {
                cond=1;
                nouveau->ptsuiv=malloc(sizeof(balise));
                nouveau=nouveau->ptsuiv;
                nouveau->ptsuiv=NULL;
            }
            else
            {
            cond=0;

           /* printf("\n\n\n\n\n\n\n");/////////////////////////////////////////////
                                                                                //
            balise *ptcurr=donnees->debutbalises;                               //
            while(ptcurr->ptsuiv!=NULL)                                         // sert à rien, c'est pour vérifier que le chainage marche correctement.
            {                                                                   //
                printf("%s\n",ptcurr->nom);                                     //
                ptcurr=ptcurr->ptsuiv;                                          //
            }/////////////////////////////////////////////////////////////////////
*/
            }

            }while(cond);
        fclose(fic);
        }
//__________________________________________________________________________________________________________________________________________________________________________________________
//_____________________________________________________________________________plan de vols_________________________________________________________________________________________________

    }
//__________________________________________________________________________________________________________________________________________________________________________________________
//_____________________________________________________________________________plans de vols________________________________________________________________________________________________
    if(donnees->what_file==2)
    {
    //g_print("PDV: %s\n",donnees->ptchemin);

        int j;
        FILE * fic=NULL;
        fic=fopen(donnees->ptchemin,"r");
        //char ligne[200];
        if(fic!=NULL)
        {
            pdv* nouveau=malloc(sizeof(pdv));

            if(donnees->debutpdv==NULL)
            {
                donnees->debutpdv=nouveau;
            }
            else
            {
                donnees->debutpdv=nouveau;
            }

            nouveau->ptsuiv=NULL;
            nouveau->pass_debut=NULL;
            int cond;
            char ligne[200];
            fscanf(fic,"%[^\n]",ligne);
            fgetc(fic);
            do
            {
                int errpass=0; //pour détecter les erreurs d'importations des PDV
                char chainetempo[200];

                j=0;
                int j0=0;

        //printf("%s\n",ligne);

                while(ligne[j]!=' ')        //------------------------ lecture de l'indicatif avion ---------------------------//
                {
                    nouveau->nom[j-j0]=ligne[j];
                    j++;
                }
                nouveau->nom[j-j0]='\0';
        //printf("nom: %s\n",nouveau->nom);

                j+=1;                      //------------------------ lecture de l'heure ---------------------------//
                j0=j;
                while(ligne[j]!=':')
                {
                    chainetempo[j-j0]=ligne[j];
                    j++;
                }
                chainetempo[j-j0]='\0';
                sscanf(chainetempo,"%d",&nouveau->heure);

                j++;                      //------------------------ lecture des minutes ---------------------------//
                j0=j;
                while(ligne[j]!=' ')
                {
                    chainetempo[j-j0]=ligne[j];
                    j++;
                }
                chainetempo[j-j0]='\0';
                sscanf(chainetempo,"%d",&nouveau->minute);
    //printf("heure: %d:%d\n",nouveau->heure,nouveau->minute);

                j+=1;                      //------------------------ lecture du niveau de vol ---------------------------//
                j0=j;
                while(ligne[j]!=' ')
                {
                    chainetempo[j-j0]=ligne[j];
                    j++;
                }
                chainetempo[j-j0]='\0';
                sscanf(chainetempo,"%d",&nouveau->altitude);
    //printf("alt: %d\n",nouveau->altitude);


                j+=1;                      //------------------------ lecture de la vitesse ---------------------------//
                j0=j;
                while(ligne[j]!=' ')
                {
                    chainetempo[j-j0]=ligne[j];
                    j++;
                }
                chainetempo[j-j0]='\0';
                sscanf(chainetempo,"%d",&nouveau->vitesse);
    //printf("vitesse: %d\n",nouveau->vitesse);




                j++;                      //------------------------ lecture des pt de passage ---------------------------//
                j0=j;
                nouveau->pass_debut=malloc(sizeof(pt_pass));
                pt_pass* pass_current=nouveau->pass_debut;
                pass_current->point=NULL;
                pass_current->ptsuiv=NULL;

                while(j<strlen(ligne))
                {
                    j0=j;
                    while(ligne[j]!=' ' && ligne[j]!='\0')        //lecture du code
                    {
                        chainetempo[j-j0]=ligne[j];
                        j++;
                    }
                    chainetempo[j-j0]='\0';


                        if(donnees->debutbalises!=NULL)
                        {
                            balise* pt_current=donnees->debutbalises;

                            while(pt_current!=NULL)
                            {

                                if(!strcmp(chainetempo,pt_current->nom))
                                {
                                    pass_current->point=pt_current;
                                    pass_current->type_point=1;
//printf("%s sélectionné\n",pt_current->nom);
                                }
                                pt_current=pt_current->ptsuiv;
                            }
                        }


                        if(donnees->debutaero!=NULL)
                        {
                            aerodrome* pt_current=donnees->debutaero;

                            while(pt_current!=NULL)
                            {

                                if(!strcmp(chainetempo,pt_current->oaci))
                                {
                                    pass_current->point=pt_current;
                                    pass_current->type_point=0;
//printf("%s sélectionné\n",pt_current->oaci);
                                }
                                pt_current=pt_current->ptsuiv;
                            }

                        }

                        if(pass_current->point==NULL)
                        {
                            errpass++;
                        }

                        pass_current->ptsuiv=malloc(sizeof(pt_pass));
                        pass_current=pass_current->ptsuiv;
                        pass_current->ptsuiv=NULL;
                        pass_current->point=NULL;

                        j+=1;

                }

                nouveau->affichage=0; //non affiché par défault


            fscanf(fic,"%[^\n]",ligne);

            if(fgetc(fic)!=EOF)
            {
                if(!errpass)
                {
                    donnees->finpdv=nouveau;
                    nouveau->ptsuiv=malloc(sizeof(pdv));
                    nouveau=nouveau->ptsuiv;
                    nouveau->ptsuiv=NULL;
                }
                cond=1;

            }
            else
            {
                if(!errpass)
                {
                    donnees->finpdv=nouveau;
                }
                /*else
                {
                    pt_pass* pass=nouveau->pass_debut;
                    while(pass!=NULL)
                    {
                        pt_pass* pass2=pass->ptsuiv;
                        free(pass);
                        pass=pass2;
                    }
                    free(nouveau);
                }*/
                cond=0;
            }

            }while(cond);

        fclose(fic);
        }
        integrer_temps(donnees->debutpdv);
    }


    donnees->file_selection=NULL;

    conversion(donnees);
}


void conversion(file_opener *donnees)
{

    if(donnees->what_file==0)
    {
        aerodrome* pt_current=donnees->debutaero;

        while(pt_current->ptsuiv!=NULL)             //création et initialisation des checkbox
        {
        pt_current->pos_y = (donnees->latitude_max - pt_current->latitude) / donnees->dlat; // Conversion et transformation de la position en pourcentage de la zone affichable
        //printf("%s , latitude = %lf, y = %lf\n",pt_current->nom,pt_current->latitude,pt_current->pos_y);
        pt_current->pos_x = (-donnees->longitude_min + pt_current->longitude) / donnees->dlong;
       // printf("%s , longitude = %lf, x = %lf\n\n",pt_current->nom,pt_current->longitude,pt_current->pos_x);
        pt_current=pt_current->ptsuiv;
        }

    }

    if(donnees->what_file==1)
    {
        balise* pt_balise=donnees->debutbalises;

        while(pt_balise->ptsuiv!=NULL)
        {
//        pt_balise->pos_x = (pt_balise->latmin) / 60;
//        printf("minutes : %lf\n",pt_balise->pos_x);
//        printf("minutes réels : %lf\n",pt_balise->latmin);
//        pt_balise->pos_x = (pt_balise->latsec) / 3600;
//        printf("secondes : %lf\n",pt_balise->pos_x);
//        printf("secondes réels : %lf\n",pt_balise->latsec);
            pt_balise->pos_y = (donnees->latitude_max - pt_balise->latitude) / donnees->dlat;
            pt_balise->pos_x = (-donnees->longitude_min + pt_balise->longitude) / donnees->dlong;

//        pt_balise->pos_y = (donnees->latitude_max - (pt_balise->latdeg) - (pt_balise->latmin) / 60 - (pt_balise->latsec) / 3600) / donnees->dlat;
       // printf("%s , latitude = %lf°,%lf',%lf\" , y=%lf\n",pt_balise->nom,pt_balise->latdeg,(pt_balise->latmin) / 60,(pt_balise->latsec) / 3600,pt_balise->pos_y);
//        pt_balise->pos_x = (-donnees->longitude_min + (pt_balise->dirlong) * (pt_balise->longdeg + (pt_balise->longmin) / 60 + (pt_balise->longsec) / 3600)) / donnees->dlong;
       // printf("%s , %d longitude = %lf°,%lf',%lf\" , x=%lf\n\n",pt_balise->nom,pt_balise->dirlong,pt_balise->longdeg,pt_balise->longmin,pt_balise->longsec,pt_balise->pos_x);
            pt_balise=pt_balise->ptsuiv;
        }

    }

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

void liberer_memoire(GtkWidget *bouton, file_opener *donnees)
{
    aerodrome* pt_current_aero= donnees->debutaero;
    while(pt_current_aero!=NULL)
    {
        aerodrome* pt2=pt_current_aero->ptsuiv;
        free(pt_current_aero);
        pt_current_aero=pt2;
    }
    donnees->debutaero=NULL;

    balise* pt_current_balises= donnees->debutbalises;
    while(pt_current_balises!=NULL)
    {
        balise* pt2=pt_current_balises->ptsuiv;
        free(pt_current_balises);
        pt_current_balises=pt2;
    }
    donnees->debutbalises=NULL;

    pdv* pt_current_pvd=donnees->debutpdv;
    while(pt_current_pvd!=NULL)
    {
        pdv* pt2=pt_current_pvd->ptsuiv;

        pt_pass* pass_current=pt_current_pvd->pass_debut;
        while(pass_current!=NULL)
        {
            pt_pass* pass2=pass_current->ptsuiv;
            free(pass_current);
            pass_current=pass2;
        }

        free(pt_current_pvd);
        pt_current_pvd=pt2;

    }
    donnees->debutpdv=NULL;

    conflit* pt_current_conflit=donnees->deb_conflits;
    while(pt_current_conflit!=NULL)
    {
        conflit* pt2=pt_current_conflit->ptsuiv;
        free(pt_current_conflit);
        pt_current_conflit=pt2;
    }
    donnees->deb_conflits=NULL;


    redessiner(donnees->carte);
}









void creer_file_save_selection(GtkWidget *bouton,file_opener *donnees)
{
//printf("azert\n");
    donnees->file_selection = gtk_file_selection_new( g_locale_to_utf8( "Sélectionnez un fichier", -1, NULL, NULL, NULL) );
    gtk_widget_show(donnees->file_selection);

    gtk_window_set_modal(GTK_WINDOW(donnees->file_selection), TRUE);

    g_signal_connect(G_OBJECT(GTK_FILE_SELECTION(donnees->file_selection)->ok_button), "clicked", G_CALLBACK(recuperer_save_chemin), donnees );

    g_signal_connect_swapped(G_OBJECT(GTK_FILE_SELECTION(donnees->file_selection)->cancel_button), "clicked", G_CALLBACK(gtk_widget_destroy), donnees->file_selection);
}

void recuperer_save_chemin(GtkWidget *bouton, file_opener *donnees)
{
    const gchar* chemin_fichier;
    chemin_fichier = gtk_file_selection_get_filename(GTK_FILE_SELECTION (donnees->file_selection) );

    donnees->ptchemin = malloc (sizeof *(donnees->ptchemin) * strlen (chemin_fichier) + 1);
    strcpy (donnees->ptchemin, chemin_fichier);

    gtk_widget_destroy(donnees->file_selection);

     sauver_fichiers(donnees);
}




void sauver_fichiers(file_opener *donnees)
{
printf("%s",donnees->ptchemin);

    FILE *fic=NULL;
    fic=fopen(donnees->ptchemin,"w");
    if(fic!=NULL)
    {

        if(donnees->debutpdv!=NULL)
        {
            pdv* current=donnees->debutpdv;

            while(current!=NULL)
            {
                fprintf(fic,"%s %02d:%02d %d %d",current->nom,current->heure,current->minute,current->altitude,current->vitesse);

                pt_pass* pass_current=current->pass_debut;
                while(pass_current->ptsuiv!=NULL)
                {
                    if(pass_current->type_point==0)
                    {
                        aerodrome* aero=pass_current->point;
                        fprintf(fic," %s",aero->oaci);
                    }
                    else
                    {
                        balise* bal=pass_current->point;
                        fprintf(fic," %s",bal->nom);
                    }
                    pass_current=pass_current->ptsuiv;

                }
                fprintf(fic,"\n");

                current=current->ptsuiv;
            }
        }
        fclose(fic);
    }



}




void integrer_temps(pdv* pdv_deb)
{
    pdv* pdv_current=pdv_deb;
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


}
