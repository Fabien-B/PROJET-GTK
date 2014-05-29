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

                while(ligne[j]!=',')        //------------------------ lecture de la latitude ---------------------------//
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
                sscanf(chainetempo, "%lf", &nouveau->latitude); //et conversion de la chaine de caractère en double
        //printf("latitude=%lf\n",nouveau->latitude);


                j+=2;                      //------------------------ lecture de la longitude ---------------------------//
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
                sscanf(chainetempo, "%lf", &nouveau->longitude);
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
        //printf("oaci=%s\n",nouveau->oaci);

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
                sscanf(chainetempo, "%d", &nouveau->latdeg);

                j+=2;                                        //minutes latitude
                j0=j;
                while(j<j0+2)
                {
                    chainetempo[j-j0]=ligne[j];
                    j++;
                }
                chainetempo[j-j0]='\0';
                sscanf(chainetempo, "%d", &nouveau->latmin);

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
                sscanf(chainetempo, "%d", &nouveau->longdeg);

                j+=2;                                        //longitude minutes
                j0=j;
                while(j<j0+2)
                {
                    chainetempo[j-j0]=ligne[j];
                    j++;
                }
                chainetempo[j-j0]='\0';
                sscanf(chainetempo, "%d", &nouveau->longmin);

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

        int j=0;
        FILE * fic=NULL;
        fic=fopen(donnees->ptchemin,"r");
        char ligne[50];
        if(fic!=NULL)
        {

            int j0=0;
            pdv * nouveau=malloc(sizeof(pdv));
            donnees->debutpdv=nouveau;
            nouveau->ptsuiv=NULL;
            nouveau->pass_debut=NULL;
            nouveau->pass_fin=NULL;
            int cond;
            do      //tant qu'on est pas arrivé à la fin du fichier.
            {

                char chainetempo[60]; //initialisation d'une chaine de caractères temporaire
                char type[60];

                fscanf(fic,"%[^\n]",nouveau->nom);  //récupération du nom du PDV
//printf("%s\n",nouveau->nom);
                fgetc(fic);      //pour passer à la ligne suivante
                fscanf(fic,"%d",&nouveau->altitude);
//printf("%d\n",nouveau->altitude);
                fgetc(fic);

                fscanf(fic,"%[^\n]",ligne);



                nouveau->pass_debut=malloc(sizeof(pt_pass));
                pt_pass* pass_current=nouveau->pass_debut;
                pass_current->point=NULL;


                do
                {
                    fgetc(fic);
                    j=0;
                    while(ligne[j]!=' ')        //------------------------ lecture du type de point (aérodrome ou balise) ---------------------------//
                    {
                        type[j]=ligne[j];
                        j++;

                    }
                    type[j]='\0';
//printf("%s\n",type);




                    if(!strcmp(type,"OACI"))        //------------------------ si le point suivant est un aérodrome ---------------------------//
                    {
//g_print("Aero   :");



                        pass_current->type_point=0;     //indique que c'est un aérodrome
                        j+=2;
                        j0=j;

                        while(ligne[j]!='"')
                        {
                            chainetempo[j-j0]=ligne[j];
                            j++;
                        }
                        chainetempo[j-j0]='\0';
//printf("%s    ===    ",chainetempo);


                        if(donnees->debutaero!=NULL)
                        {
                            aerodrome* pt_current=donnees->debutaero;

                            while(pt_current!=NULL)
                            {

                                if(!strcmp(chainetempo,pt_current->oaci))
                                {
                                    pass_current->point=pt_current;
//printf("%s sélectionné\n",pt_current->oaci);
                                }
                                pt_current=pt_current->ptsuiv;
                            }

                        }

                    }


                    if(!strcmp(type,"balise"))            //------------------------ si le point suivant est une balise---------------------------//
                    {
//g_print("Balise :");


                        pass_current->type_point=1;     //indique que c'est une balise
                        j+=2;
                        j0=j;

                        while(ligne[j]!='"')
                        {
                            chainetempo[j-j0]=ligne[j];
                            j++;
                        }
                        chainetempo[j-j0]='\0';
//printf("%s    ===    ",chainetempo);


                        if(donnees->debutbalises!=NULL)
                        {
                            balise* pt_current=donnees->debutbalises;

                            while(pt_current!=NULL)
                            {

                                if(!strcmp(chainetempo,pt_current->nom))
                                {
                                    pass_current->point=pt_current;
//printf("%s sélectionné\n",pt_current->nom);
                                }
                                pt_current=pt_current->ptsuiv;
                            }
                        }
                    }

                    nouveau->affichage=0; //non affiché par défault



                    pass_current->ptsuiv=malloc(sizeof(pt_pass));
                    pass_current=pass_current->ptsuiv;
                    pass_current->ptsuiv=NULL;
                    pass_current->point=NULL;





                    fscanf(fic,"%[^\n]",ligne);



                }while(strcmp(ligne,";"));
                fgetc(fic);

//printf("\n\n\n\n\n");


            if(fgetc(fic)!=EOF)
            {
                cond=1;
                nouveau->ptsuiv=malloc(sizeof(pdv));
                nouveau=nouveau->ptsuiv;
                nouveau->ptsuiv=NULL;
            }
            else
            {
            cond=0;
            }
            }while(cond);
        fclose(fic);




        }

    }


    donnees->file_selection=NULL;
}

