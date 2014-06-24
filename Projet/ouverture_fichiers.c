#include "interface.h"
#include "ouverture_fichiers.h"
#include "cartographie.h"
#include "filtrage.h"
#include "ajouts_utilisateur.h"
#include "conflits.h"


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


//créer une boite de dialogue demandant  l'utilisateur quel type de fichier il souhaite ouvrir.
void lancer_boite(GtkWidget *bouton, file_opener *donnees)
{
    GtkWidget* pBoite;
    pBoite = gtk_dialog_new_with_buttons("Sélection du fichier à charger",NULL,GTK_DIALOG_MODAL,GTK_STOCK_OK,GTK_RESPONSE_OK,NULL);


    GtkWidget *fixed;
    GtkWidget *combo;
    GtkWidget *label;


    label=gtk_label_new("Sélectionnez le type de fichier que vous souhaitez charger.");
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

//met donnees->what_file à 0,1 ou 2 suivant si l'utilisateur à choisi aérodrome, balises ou plan de vol. Utilisé ensuite dans la fonction charger_fichiers.
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
        FILE * fic=NULL;
        fic=fopen(donnees->ptchemin,"r");
        char ligne[100];
        if(fic!=NULL)
        {
            aerodrome * nouveau=malloc(sizeof(aerodrome));  //allocation en mémoire du premier élément de la liste chainée
            donnees->debutaero=nouveau;
            nouveau->ptsuiv=NULL;
            int cond;
            do
            {



                fscanf(fic,"%[^\n]",ligne);     //lecture de la ligne

                int k;
                for(k=0;k<strlen(ligne);k++) //remplacement du caractère '.' par ',' (sinon problème pour la lecture des flottants)
                {
                    if(ligne[k]=='.')
                    {
                        ligne[k]=',';
                    }
                }


                for(k=0;k<strlen(ligne)-1;k++) //remplacement de " (" par "|". (sinon problème pour la lecture du nom quand il y a des espaces)
                {
                    if(ligne[k]==' ' && ligne[k+1]=='(')
                    {
                        ligne[k]='|';
                    }
                }

                sscanf(ligne, "%lf, %lf, \"%[^|]|(%[^)]", &nouveau->longitude,&nouveau->latitude,nouveau->nom,nouveau->oaci);   //lecture de la ligne

                nouveau->affichage=0; //non affiché par défault

                if(fgetc(fic)!=EOF)     //on continue jusqu'a la fin du fichier
                {
                    cond=1;

                    nouveau->ptsuiv=malloc(sizeof(aerodrome));
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
//__________________________________________________________________________________________________________________________________________________________________________________________
//_____________________________________________________________________________balises______________________________________________________________________________________________________


    if(donnees->what_file==1)
    {
        FILE * fic=NULL;
        fic=fopen(donnees->ptchemin,"r");
        char ligne[100];
        if(fic!=NULL)
        {
            balise * nouveau=malloc(sizeof(balise));    //allocation en mémoire du premier élément de la liste chainée
            donnees->debutbalises=nouveau;
            nouveau->ptsuiv=NULL;

            int cond;
            do
            {

                fscanf(fic,"%[^\n]",ligne);

                double latdeg,latmin,latsec;
                double longdeg,longmin,longsec;
                char dirla,dirlo;
                int dirlat,dirlong;

                int i;
                for(i=0;i<strlen(ligne);i++)        //remplacement du caractère '.' par ',' (sinon problème pour la lecture des flottants)
                {
                    if(ligne[i]=='.')
                    {
                        ligne[i]=',';
                    }
                }

                sscanf(ligne, "%s %lf°%lf'%lf\" %c %lf°%lf'%lf\" %c", nouveau->nom,&latdeg,&latmin,&latsec,&dirla,&longdeg,&longmin,&longsec,&dirlo);   //lecture de tous les élements


                i=0;
                while(nouveau->nom[i]!='\0')
                {
                    nouveau->nom[i]=toupper(nouveau->nom[i]);
                    i++;
                }
                nouveau->nom[i]='\0';


                if(dirlo=='E')
                {
                    dirlong=1;
                }
                else
                {
                    dirlong=-1;
                }

                if(dirla=='N')
                {
                    dirlat=1;
                }
                else
                {
                    dirlat=-1;
                }


                nouveau->latitude=dirlat*(latdeg+latmin/60+latsec/3600);        //conversion en décimal, pour harmonisation des fonctions de conversion en (x,y), et calculs plus faciles
                nouveau->longitude=dirlong*(longdeg+longmin/60+longsec/3600);

            nouveau->affichage=0; //non affiché par défault

            if(fgetc(fic)!=EOF)     //on continue jusqu'a la fin du fichier
            {
                cond=1;
                nouveau->ptsuiv=malloc(sizeof(balise));
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
//__________________________________________________________________________________________________________________________________________________________________________________________
//_____________________________________________________________________________plans de vols________________________________________________________________________________________________

    if(donnees->what_file==2)
    {
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
                donnees->debutpdv=nouveau;      //instruction à changer pour pouvoir ouvrir plusieurs fichiers à la suite
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
                    while(ligne[j]!=' ' && ligne[j]!='\0')        //lecture du code OACI ou balise
                    {
                        chainetempo[j-j0]=ligne[j];
                        j++;
                    }
                    chainetempo[j-j0]='\0';
                    int i=0;
                    while(chainetempo[i]!='\0')
                    {
                        chainetempo[i]=toupper(chainetempo[i]);
                        i++;
                    }

            //on parcours les balises et aérodromes. Quand on en trouve un(e) avec le meme nom que dans le fichier, on le prend comme point de passage courant
                        if(donnees->debutbalises!=NULL)
                        {
                            balise* pt_current=donnees->debutbalises;

                            while(pt_current!=NULL)
                            {

                                if(!strcmp(chainetempo,pt_current->nom))
                                {
                                    pass_current->point=pt_current;
                                    pass_current->type_point=1;
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
                                }
                                pt_current=pt_current->ptsuiv;
                            }

                        }


                        if(pass_current->point==NULL)       //si le point n'as pas été trouvé, on incrémente la varible d'erreur (permet de savoir le nombre d'erreurs)
                        {
                            errpass++;
                        }

                        pass_current->ptsuiv=malloc(sizeof(pt_pass));
                        pass_current=pass_current->ptsuiv;
                        pass_current->ptsuiv=NULL;
                        pass_current->point=NULL;

                        j+=1;

                }

                nouveau->affichage=1; // affiché par défault


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
                else            //si l'importation du plan de vol à échoué, on libère la mémoire alloué
                {
                    pt_pass* pass=nouveau->pass_debut;
                    while(pass!=NULL)
                    {
                        pt_pass* pass2=pass->ptsuiv;
                        free(pass);
                        pass=pass2;
                    }
                }
                cond=1;

            }
            else
            {
                if(!errpass)
                {
                    donnees->finpdv=nouveau;
                }
                else
                {
                    pt_pass* pass=nouveau->pass_debut;
                    while(pass!=NULL)
                    {
                        pt_pass* pass2=pass->ptsuiv;
                        free(pass);
                        pass=pass2;
                    }
                    free(nouveau);
                }
                cond=0;
            }

            }while(cond);

        fclose(fic);
        }
        integrer_temps(donnees);
    }


    donnees->file_selection=NULL;
    redessiner(NULL,donnees->carte);

}





//on parcours toutes les listes chainée et on libère la mémoire allouée dynamiquement
void liberer_memoire(GtkWidget *bouton, file_opener *donnees)
{
    aerodrome* pt_current_aero= donnees->debutaero; //pour les aérodromes
    while(pt_current_aero!=NULL)
    {
        aerodrome* pt2=pt_current_aero->ptsuiv;
        free(pt_current_aero);
        pt_current_aero=pt2;
    }
    donnees->debutaero=NULL;

    balise* pt_current_balises= donnees->debutbalises;  //pour les balises
    while(pt_current_balises!=NULL)
    {
        balise* pt2=pt_current_balises->ptsuiv;
        free(pt_current_balises);
        pt_current_balises=pt2;
    }
    donnees->debutbalises=NULL;

    pdv* pt_current_pvd=donnees->debutpdv;          //pour les plans de vols
    while(pt_current_pvd!=NULL)
    {
        pdv* pt2=pt_current_pvd->ptsuiv;

        pt_pass* pass_current=pt_current_pvd->pass_debut;   //libération mémoire des points de passages
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

    conflit* pt_current_conflit=donnees->deb_conflits;      //pour les conflits
    while(pt_current_conflit!=NULL)
    {
        conflit* pt2=pt_current_conflit->ptsuiv;
        free(pt_current_conflit);
        pt_current_conflit=pt2;
    }
    donnees->deb_conflits=NULL;


    redessiner(NULL,donnees->carte);
}







void creer_file_save_selection(GtkWidget *bouton,file_opener *donnees)
{
//printf("azert\n");
    if(donnees->debutpdv!=NULL)
    {

        donnees->file_selection = gtk_file_selection_new( g_locale_to_utf8( "Sélectionnez un fichier", -1, NULL, NULL, NULL) );
        gtk_widget_show(donnees->file_selection);

        gtk_window_set_modal(GTK_WINDOW(donnees->file_selection), TRUE);

        g_signal_connect(G_OBJECT(GTK_FILE_SELECTION(donnees->file_selection)->ok_button), "clicked", G_CALLBACK(recuperer_save_chemin), donnees );

        g_signal_connect_swapped(G_OBJECT(GTK_FILE_SELECTION(donnees->file_selection)->cancel_button), "clicked", G_CALLBACK(gtk_widget_destroy), donnees->file_selection);
    }
    else
    {
    GtkWidget* erreur_box;
    erreur_box = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "Aucun plan de vol chargé.");
    gtk_dialog_run(GTK_DIALOG(erreur_box));
    gtk_widget_destroy(erreur_box);
    }

}


void recuperer_save_chemin(GtkWidget *bouton, file_opener *donnees)
{
    const gchar* chemin_fichier;
    chemin_fichier = gtk_file_selection_get_filename(GTK_FILE_SELECTION (donnees->file_selection) );

    donnees->ptchemin = malloc (sizeof *(donnees->ptchemin) * strlen (chemin_fichier) + 1);
    strcpy (donnees->ptchemin, chemin_fichier);

    FILE *f =fopen(donnees->ptchemin,"r");
    if(f)
    {
        gtk_widget_destroy(donnees->file_selection);

        GtkWidget *main_box;
        GtkWidget *label_alerte;

        main_box = gtk_dialog_new_with_buttons("Attention!",
        NULL,
        GTK_DIALOG_MODAL,
        GTK_STOCK_OK,GTK_RESPONSE_OK,
        GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,
        NULL);

        char label[300];
        sprintf(label,"Le fichier %s existe déjà voulez vous vraiment le supprimer ?",donnees->ptchemin);
        label_alerte = gtk_label_new(label);
        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(main_box)->vbox), label_alerte,FALSE,FALSE,0);
        gtk_widget_show_all(GTK_DIALOG(main_box)->vbox);

        switch (gtk_dialog_run(GTK_DIALOG(main_box)))
        {

            case GTK_RESPONSE_OK:
                sauver_fichiers(donnees);
                gtk_widget_destroy(main_box);
                break;

            case GTK_RESPONSE_CANCEL:
                creer_file_save_selection(NULL,donnees);
                gtk_widget_destroy(main_box);
                break;
            default:break;
        }
    }
    else
    {
    gtk_widget_destroy(donnees->file_selection);
    sauver_fichiers(donnees);
    }

}



//on sauvegarde les plans de vols sous le meme format que celui pour les ouvrirs
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









void creer_file_conflit_selection(GtkWidget *bouton,file_opener *donnees)
{
//printf("azert\n");
    if(donnees->debutpdv!=NULL)
    {

        donnees->file_selection = gtk_file_selection_new( g_locale_to_utf8( "Sélectionnez un fichier", -1, NULL, NULL, NULL) );
        gtk_widget_show(donnees->file_selection);

        gtk_window_set_modal(GTK_WINDOW(donnees->file_selection), TRUE);

        g_signal_connect(G_OBJECT(GTK_FILE_SELECTION(donnees->file_selection)->ok_button), "clicked", G_CALLBACK(recuperer_conflit_chemin), donnees );

        g_signal_connect_swapped(G_OBJECT(GTK_FILE_SELECTION(donnees->file_selection)->cancel_button), "clicked", G_CALLBACK(gtk_widget_destroy), donnees->file_selection);
    }
    else
    {
    GtkWidget* erreur_box;
    erreur_box = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "Aucun plan de vol chargé.");
    gtk_dialog_run(GTK_DIALOG(erreur_box));
    gtk_widget_destroy(erreur_box);
    }

}


void recuperer_conflit_chemin(GtkWidget *bouton, file_opener *donnees)
{
    const gchar* chemin_fichier;
    chemin_fichier = gtk_file_selection_get_filename(GTK_FILE_SELECTION (donnees->file_selection) );

    donnees->ptchemin = malloc (sizeof *(donnees->ptchemin) * strlen (chemin_fichier) + 1);
    strcpy (donnees->ptchemin, chemin_fichier);

    FILE *f =fopen(donnees->ptchemin,"r");
    if(f)
    {
        gtk_widget_destroy(donnees->file_selection);

        GtkWidget *main_box;
        GtkWidget *label_alerte;

        main_box = gtk_dialog_new_with_buttons("Attention",
        NULL,
        GTK_DIALOG_MODAL,
        GTK_STOCK_OK,GTK_RESPONSE_OK,
        GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,
        NULL);

        char label[300];
        sprintf(label,"Le fichier %s existe déjà voulez vous vraiment le supprimer ?",donnees->ptchemin);
        label_alerte = gtk_label_new(label);
        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(main_box)->vbox), label_alerte,FALSE,FALSE,0);
        gtk_widget_show_all(GTK_DIALOG(main_box)->vbox);

        switch (gtk_dialog_run(GTK_DIALOG(main_box)))
        {

            case GTK_RESPONSE_OK:
                sauver_conflits(donnees);
                gtk_widget_destroy(main_box);
                break;

            case GTK_RESPONSE_CANCEL:
                creer_file_conflit_selection(NULL,donnees);
                gtk_widget_destroy(main_box);
                break;
            default:break;
        }
    }
    else
    {
    gtk_widget_destroy(donnees->file_selection);
    sauver_conflits(donnees);
    }

}


//on sauvegarde les conflits
void sauver_conflits(file_opener *donnees)
{
g_print("conflit");
printf("%s",donnees->ptchemin);

    FILE *fic=NULL;
    fic=fopen(donnees->ptchemin,"w");
    if(fic!=NULL)
    {

        if(donnees->deb_conflits!=NULL)
        {
            conflit* current=donnees->deb_conflits;

            while(current->ptsuiv!=NULL)
            {
                int h=current->temps_deb/60;
                int m=current->temps_deb-60*h;
                fprintf(fic,"Conflit etre %s et %s à %02d:%02d, localisation approximative: %lf ; %lf",current->pdv1->nom,current->pdv2->nom,h,m,current->latitude,current->longitude);

                fprintf(fic,"\n");

                current=current->ptsuiv;
            }
        }
        fclose(fic);
    }



}









void creer_file_plots_selection(GtkWidget *bouton,file_opener *donnees)
{
printf("azert\n");
    if(donnees->debutpdv!=NULL)
    {

        donnees->file_selection = gtk_file_selection_new( g_locale_to_utf8( "Sélectionnez un fichier", -1, NULL, NULL, NULL) );
        gtk_widget_show(donnees->file_selection);

        gtk_window_set_modal(GTK_WINDOW(donnees->file_selection), TRUE);

        g_signal_connect(G_OBJECT(GTK_FILE_SELECTION(donnees->file_selection)->ok_button), "clicked", G_CALLBACK(recuperer_plots_chemin), donnees );

        g_signal_connect_swapped(G_OBJECT(GTK_FILE_SELECTION(donnees->file_selection)->cancel_button), "clicked", G_CALLBACK(gtk_widget_destroy), donnees->file_selection);
    }
    else
    {
    GtkWidget* erreur_box;
    erreur_box = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "Aucun plan de vol chargé.");
    gtk_dialog_run(GTK_DIALOG(erreur_box));
    gtk_widget_destroy(erreur_box);
    }

}


void recuperer_plots_chemin(GtkWidget *bouton, file_opener *donnees)
{printf("tooacrd!");
    const gchar* chemin_fichier;
    chemin_fichier = gtk_file_selection_get_filename(GTK_FILE_SELECTION (donnees->file_selection) );

    donnees->ptchemin = malloc (sizeof *(donnees->ptchemin) * strlen (chemin_fichier) + 1);
    strcpy (donnees->ptchemin, chemin_fichier);

    FILE *f=fopen(donnees->ptchemin,"r");
    if(f)
    {
        gtk_widget_destroy(donnees->file_selection);

        GtkWidget *main_box;
        GtkWidget *label_alerte;

        main_box = gtk_dialog_new_with_buttons("Attention",
        NULL,
        GTK_DIALOG_MODAL,
        GTK_STOCK_OK,GTK_RESPONSE_OK,
        GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,
        NULL);

        char label[300];
        sprintf(label,"Le fichier %s existe déjà voulez vous vraiment le supprimer ?",donnees->ptchemin);
        label_alerte = gtk_label_new(label);
        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(main_box)->vbox), label_alerte,FALSE,FALSE,0);
        gtk_widget_show_all(GTK_DIALOG(main_box)->vbox);

        switch (gtk_dialog_run(GTK_DIALOG(main_box)))
        {

            case GTK_RESPONSE_OK:
                sauver_plots(donnees);
                gtk_widget_destroy(main_box);
                break;

            case GTK_RESPONSE_CANCEL:
                creer_file_plots_selection(NULL,donnees);
                gtk_widget_destroy(main_box);
                break;
            default:break;
        }
    }
    else
    {
    g_print("ggggggggggggga");
    gtk_widget_destroy(donnees->file_selection);
    sauver_plots(donnees);
    }

}


//on sauvegarde les plots
void sauver_plots(file_opener *donnees)
{
    FILE *fic=NULL;
    fic=fopen(donnees->ptchemin,"w");
    if(fic!=NULL)
    {

        if(donnees->debutpdv!=NULL)
        {
            pdv* current=donnees->debutpdv;

            while(current->ptsuiv!=NULL)
            {
                fprintf(fic,"%s %d kt FL%d\n",current->nom,current->vitesse,current->altitude);

                double td=current->temps_depart;
                double ta=current->temps_arrivee;
                double t;
                for(t=td;t<=ta;t+=donnees->deltat_conflits)
                {
                    position* pos=malloc(sizeof(position));
                    get_position_avion(pos,current,t);
                    int h=t/60;
                    int m=t-60*h;
                    int s=(t-60*h-m)*60;
                    fprintf(fic,"%02d:%02d:%02d\t%lf;%lf\n",h,m,s,pos->x,pos->y);


                }
                fprintf(fic,"\n\n");

                current=current->ptsuiv;
            }
        }
        fclose(fic);
    }



}


