#include "interface.h"
//structure pour les fichiers de donnees



void init_interface(int argc, char *argv[])
{
    GtkWidget *Window;
    GtkWidget *mother_box;

    GtkWidget *work_zone;
    GtkWidget *work_zl;
    GtkWidget *work_zr;
    GtkWidget *carte;

    GtkWidget *menu_bar;
    GtkWidget *Fichier_menu;
    GtkWidget *Aide_menu;
    GtkWidget *MI1_Fichier;
    GtkWidget *MI1_Aide;
    GtkWidget *MI2_APropos;
    GtkWidget *MI2_Nouveau;
    GtkWidget *MI2_Ouvrir;
    GtkWidget *MI2_Quitter;
//
    GtkWidget *hbox_curseur;
    GtkWidget *label_curseur;
    GtkWidget *curseur;

    file_opener *donnees=g_malloc(sizeof(file_opener));
        donnees->ptchemin=NULL;
        donnees->file_selection=NULL;


    gtk_init(&argc,&argv);

    Window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(Window), "GATI");
    gtk_window_set_default_size(GTK_WINDOW(Window), 320, 400);
    g_signal_connect(G_OBJECT(Window), "destroy", G_CALLBACK(gtk_main_quit), NULL);


// Creation et ajout de la GtkBox mère verticale
    mother_box = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(Window), mother_box);
//creation et ajout de la barre de menu et la zone de travail
    menu_bar = gtk_menu_bar_new();

    MI1_Fichier = gtk_menu_item_new_with_label("Fichier");
    Fichier_menu = gtk_menu_new();
        MI2_Nouveau = gtk_menu_item_new_with_label("Nouveau");
        gtk_menu_shell_append(GTK_MENU_SHELL(Fichier_menu), MI2_Nouveau);
        MI2_Ouvrir = gtk_menu_item_new_with_label("Ouvrir");
        gtk_menu_shell_append(GTK_MENU_SHELL(Fichier_menu), MI2_Ouvrir);
                //g_signal_connect(G_OBJECT(MI2_Ouvrir), "activate", G_CALLBACK(creer_file_selection), donnees);
                        g_signal_connect(G_OBJECT(MI2_Ouvrir), "activate", G_CALLBACK(lancer_boite), donnees);
        MI2_Quitter = gtk_menu_item_new_with_label("Quitter");
        gtk_menu_shell_append(GTK_MENU_SHELL(Fichier_menu), MI2_Quitter);
            g_signal_connect(G_OBJECT(MI2_Quitter), "activate", G_CALLBACK(gtk_main_quit), NULL);
        gtk_menu_item_set_submenu(GTK_MENU_ITEM(MI1_Fichier), Fichier_menu);

    MI1_Aide = gtk_menu_item_new_with_label("?");
    Aide_menu = gtk_menu_new();
        MI2_APropos = gtk_menu_item_new_with_label("À Propos");
        gtk_menu_shell_append(GTK_MENU_SHELL(Aide_menu), MI2_APropos);
            g_signal_connect(G_OBJECT(MI2_APropos), "activate", G_CALLBACK(APropos), NULL);
        gtk_menu_item_set_submenu(GTK_MENU_ITEM(MI1_Aide), Aide_menu);

    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), MI1_Fichier);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), MI1_Aide);
    gtk_box_pack_start(GTK_BOX(mother_box), menu_bar, FALSE, FALSE, 0);
    //menu...







    work_zone = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(mother_box), work_zone, FALSE, FALSE, 0);
//Création et ajout des espaces de travails gauche et droite
    work_zl = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(work_zone), work_zl, TRUE, TRUE, 0);
    work_zr = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(work_zone), work_zr, FALSE, FALSE, 0);

    carte=gtk_drawing_area_new ();
    gtk_widget_set_size_request (carte, 100, 200);
    gtk_box_pack_start(GTK_BOX(work_zl), carte, FALSE, FALSE, 0);



    hbox_curseur = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(work_zl), hbox_curseur, TRUE, TRUE, 0);
    label_curseur=gtk_label_new("temps");
    gtk_box_pack_start(GTK_BOX(hbox_curseur), label_curseur, FALSE, FALSE, 0);
    curseur=gtk_hscale_new_with_range (5,200 ,100);
    gtk_container_add(GTK_CONTAINER(hbox_curseur), curseur);


    gtk_widget_show_all(Window);
}




void APropos(GtkWidget* widget)
{
    GtkWidget* APropos_box;
    APropos_box = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "The GATIP, 'The GNU Air Traffic Indicator' est un software devellopé dans le cadre d'un projet étudiant.");
    gtk_dialog_run(GTK_DIALOG(APropos_box));
    gtk_widget_destroy(APropos_box);
}




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

     charger_fichiers(donnees); //à remplacer par la fonction de Xavier.
}

void charger_fichiers(file_opener *donnees)
{

    if(donnees->what_file==0)
    {
        g_print("aérodrome: %s\n",donnees->ptchemin);

        int i,j;
        FILE * fic=NULL;
        fic=fopen(donnees->ptchemin,"r");
        char ligne[50];
        if(fic!=NULL)
        {

            i=0;
            do
            {
                char chainetempo[20]; //initialisation d'une chaine de caractères temporaire

                j=0;
                int j0=0;
                aerodrome * nouveau=malloc(sizeof(aerodrome));
                fscanf(fic,"%[^\n]",ligne);
       // printf("%s\n",ligne);
                //fgetc(fic);

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
        printf("latitude=%lf\n",nouveau->latitude);


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
        printf("longitude=%lf\n",nouveau->longitude);


                j+=3;                      //------------------------ lecture du nom de l'aérodrome ---------------------------//
                j0=j;
                while(ligne[j]!='(')
                {
                    nouveau->nom[j-j0]=ligne[j];
                    j++;
                }
                nouveau->nom[j-j0-1]='\0';
                printf("nom=%s\n",nouveau->nom);

                j+=1;                      //------------------------ lecture du code OACI ---------------------------//
                j0=j;
                while(ligne[j]!=')')
                {
                    nouveau->oaci[j-j0]=ligne[j];
                    j++;
                }
                nouveau->oaci[j-j0]='\0';
                printf("oaci=%s\n",nouveau->oaci);

        printf("\n\n\n\n\n");



            i++;
            }while(fgetc(fic)!=EOF);
        fclose(fic);
        }
    }


    if(donnees->what_file==1)
    {
    g_print("balises: %s\n",donnees->ptchemin);

        int i,j;
        FILE * fic=NULL;
        fic=fopen(donnees->ptchemin,"r");
        char ligne[50];
        if(fic!=NULL)
        {

            i=0;
            do
            {
                char chainetempo[20]; //initialisation d'une chaine de caractères temporaire

                j=0;
                int j0=0;
                balise * nouveau=malloc(sizeof(balise));
                fscanf(fic,"%[^\n]",ligne);
       printf("%s\n",ligne);

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

        printf("%s %d°%d'%lf [%d] %d°%d'%lf [%d]",nouveau->nom,nouveau->latdeg,nouveau->latmin,nouveau->latsec,nouveau->dirlat,nouveau->longdeg,nouveau->longmin,nouveau->longsec,nouveau->dirlong);
        printf("\n\n\n\n\n");

            i++;
            }while(fgetc(fic)!=EOF);
        fclose(fic);
        }


























    }
    if(donnees->what_file==2)
    {
        g_print("plan de vols: %s\n",donnees->ptchemin);
    }



    donnees->file_selection=NULL;
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


  //g_signal_connect(G_OBJECT(combo), "changed",G_CALLBACK(combo_selected), (gpointer) label);
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
    g_print("%d\n",donnees->what_file);

  g_free(text);
}


