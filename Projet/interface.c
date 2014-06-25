#include "interface.h"
#include "ouverture_fichiers.h"
#include "cartographie.h"
#include "filtrage.h"
#include "ajouts_utilisateur.h"
#include "conflits.h"

void initialisation(int argc, char *argv[])
{
        // Initialisation de Gtk+
        g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, (GLogFunc) gtk_false, NULL);
        gtk_init (&argc, &argv);
        g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, g_log_default_handler, NULL);

        //initialisation des paramètres/donnees du programme
        file_opener *donnees=g_malloc(sizeof(file_opener));
        memset(donnees, 0, sizeof(file_opener));
        donnees->old = malloc(sizeof(position));
        donnees->bord = malloc(sizeof(position));
        donnees->start = malloc(sizeof(position));
        donnees->ptchemin=NULL;
        donnees->file_selection=NULL;
        donnees->debutaero=NULL;
        donnees->debutbalises=NULL;
        donnees->debutpdv=NULL;
        donnees->finpdv=NULL;
        donnees->finpdv=NULL;
        donnees->deb_conflits=NULL;
        donnees->temps=0;
        donnees->distance_conflit=10;
        donnees->deltat_conflits=3.0/36;
        donnees->latitude_max=51.75;
        donnees->dlat=11;
        donnees->longitude_min=-5.75;
        donnees->dlong=16;
        donnees->xcarte=550;//550
        donnees->ycarte=660;//660
        donnees->tag_lecture=0;
        donnees->clic_distance=0;

        form_pdv* formulaire=malloc(sizeof(form_pdv));
        formulaire->donnees=donnees;
        formulaire->nb_pt_int=0;
        formulaire->altitude=100;
        formulaire->nom[0]='\0';
        formulaire->vitesse=200;
        formulaire->pdv_edit=NULL;
        int i;
        for(i=0;i<40;i++)
        {
            formulaire->pass[i][0]='\0';
        }

    // Initialisation de la fenêtre
    donnees->Window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(donnees->Window), "GATI");
    gtk_window_set_default_size(GTK_WINDOW(donnees->Window), 32, 40);
    g_signal_connect(G_OBJECT(donnees->Window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Lancement de l'interface
    creer_interface(donnees,formulaire);
}


void creer_interface(file_opener* donnees,form_pdv* formulaire)
{

    GtkWidget *work_zone;
    GtkWidget *work_zl;
    GtkWidget *work_zr;

    GtkWidget *menu_bar;
    GtkWidget *Fichier_menu;
    GtkWidget *Edition_menu;
    GtkWidget *Aide_menu;
    GtkWidget *MI1_Fichier;
    GtkWidget *MI1_Edition;
    GtkWidget *MI1_Aide;
    GtkWidget *MI2_APropos;
    GtkWidget *MI2_Editer_pdv;
    GtkWidget *MI2_Nouveau;
    GtkWidget *MI2_Ouvrir;
    GtkWidget *MI2_Charger_default;
    GtkWidget *MI2_Enregistrer;
    GtkWidget *MI2_Enregistrer_conflits;
    GtkWidget *MI2_Enregistrer_plots;
    GtkWidget *MI2_parametres;
    GtkWidget *MI2_Quitter;

    GtkWidget *hbox_curseur;
    GtkWidget *label_curseur;
    GtkWidget *curseur;
    GtkWidget *filtre_button;
    GtkWidget *voir_pdv_button;
    GtkWidget *ajouter_pdv_button;

    GtkWidget *voir_conflits_button;
    GtkWidget *event_box;
    GtkWidget *Separateur;


// Création et ajout de la GtkBox mère verticale
    donnees->mother_box = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(donnees->Window), donnees->mother_box);


// Création et ajout de la barre de menu et de la zone de travail
    menu_bar = gtk_menu_bar_new();

    MI1_Fichier = gtk_menu_item_new_with_label("Fichier");
    Fichier_menu = gtk_menu_new();

        MI2_Nouveau = gtk_menu_item_new_with_label("Nouveau");
        gtk_menu_shell_append(GTK_MENU_SHELL(Fichier_menu), MI2_Nouveau);
            g_signal_connect(G_OBJECT(MI2_Nouveau), "activate", G_CALLBACK(liberer_memoire), donnees);

        MI2_Ouvrir = gtk_menu_item_new_with_label("Ouvrir");
        gtk_menu_shell_append(GTK_MENU_SHELL(Fichier_menu), MI2_Ouvrir);
                        g_signal_connect(G_OBJECT(MI2_Ouvrir), "activate", G_CALLBACK(lancer_boite), donnees);

        MI2_Charger_default = gtk_menu_item_new_with_label("Chargement par défaut");
        gtk_menu_shell_append(GTK_MENU_SHELL(Fichier_menu), MI2_Charger_default);
                        g_signal_connect(G_OBJECT(MI2_Charger_default), "activate", G_CALLBACK(rapide_file), donnees);

        MI2_Enregistrer = gtk_menu_item_new_with_label("Enregistrer les plan de vol");
        gtk_menu_shell_append(GTK_MENU_SHELL(Fichier_menu), MI2_Enregistrer);
                        g_signal_connect(G_OBJECT(MI2_Enregistrer), "activate", G_CALLBACK(creer_file_save_selection), donnees);

        MI2_Enregistrer_conflits = gtk_menu_item_new_with_label("Enregistrer les conflits");
        gtk_menu_shell_append(GTK_MENU_SHELL(Fichier_menu), MI2_Enregistrer_conflits);
                        g_signal_connect(G_OBJECT(MI2_Enregistrer_conflits), "activate", G_CALLBACK(creer_file_conflit_selection), donnees);

        MI2_Enregistrer_plots = gtk_menu_item_new_with_label("Enregistrer les plots");
        gtk_menu_shell_append(GTK_MENU_SHELL(Fichier_menu), MI2_Enregistrer_plots);
                        g_signal_connect(G_OBJECT(MI2_Enregistrer_plots), "activate", G_CALLBACK(creer_file_plots_selection), donnees);

        MI2_Quitter = gtk_menu_item_new_with_label("Quitter");
        gtk_menu_shell_append(GTK_MENU_SHELL(Fichier_menu), MI2_Quitter);
            g_signal_connect(G_OBJECT(MI2_Quitter), "activate", G_CALLBACK(gtk_main_quit), NULL);

        gtk_menu_item_set_submenu(GTK_MENU_ITEM(MI1_Fichier), Fichier_menu);

    MI1_Edition = gtk_menu_item_new_with_label("Edition");
    Edition_menu = gtk_menu_new();

        MI2_Editer_pdv = gtk_menu_item_new_with_label("Éditer les plans de vols");
        gtk_menu_shell_append(GTK_MENU_SHELL(Edition_menu), MI2_Editer_pdv);
                        g_signal_connect(G_OBJECT(MI2_Editer_pdv), "activate", G_CALLBACK(select_pdv_ed), formulaire);


        MI2_parametres = gtk_menu_item_new_with_label("Paramètres");
        gtk_menu_shell_append(GTK_MENU_SHELL(Edition_menu), MI2_parametres);
                        g_signal_connect(G_OBJECT(MI2_parametres), "activate", G_CALLBACK(parametres), formulaire);


         gtk_menu_item_set_submenu(GTK_MENU_ITEM(MI1_Edition), Edition_menu);

    MI1_Aide = gtk_menu_item_new_with_label("?");
    Aide_menu = gtk_menu_new();

        MI2_APropos = gtk_menu_item_new_with_label("À Propos");
        gtk_menu_shell_append(GTK_MENU_SHELL(Aide_menu), MI2_APropos);
            g_signal_connect(G_OBJECT(MI2_APropos), "activate", G_CALLBACK(APropos), NULL);

        gtk_menu_item_set_submenu(GTK_MENU_ITEM(MI1_Aide), Aide_menu);

    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), MI1_Fichier);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), MI1_Edition);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), MI1_Aide);
    gtk_box_pack_start(GTK_BOX(donnees->mother_box), menu_bar, FALSE, FALSE, 0);


//----------------------------------------------------------------------------------------------------


// Création et ajout des espaces de travails gauche et droite
    work_zone = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(donnees->mother_box), work_zone, FALSE, FALSE, 0);


// Zone de gauche
    work_zl = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(work_zone), work_zl, TRUE, TRUE, 0);


// Zone de droite
    work_zr = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(work_zone), work_zr, FALSE, FALSE, 0);


// Création de la carte
    donnees->carte = gtk_drawing_area_new ();
    gtk_drawing_area_size (GTK_DRAWING_AREA(donnees->carte), donnees->xcarte,donnees->ycarte);


//  Ajout event box pour gérer les évenements scroll/drag
    event_box = gtk_event_box_new();
    gtk_box_pack_start (GTK_BOX (work_zl), event_box, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(event_box), donnees->carte);
    gtk_widget_set_events(event_box,GDK_SCROLL_UP);
    gtk_signal_connect(GTK_OBJECT(event_box), "scroll_event",GTK_SIGNAL_FUNC(scroll_event), donnees);
    gtk_widget_set_events(event_box,GDK_BUTTON_PRESS);
    gtk_signal_connect(GTK_OBJECT(event_box), "button_press_event",GTK_SIGNAL_FUNC(press_event), donnees);
    gtk_widget_set_events(event_box,GDK_MOTION_NOTIFY);
    gtk_signal_connect(GTK_OBJECT(event_box), "motion_notify_event",GTK_SIGNAL_FUNC(drag_event), donnees);
    gtk_widget_set_events(event_box,GDK_POINTER_MOTION_MASK);
    gtk_signal_connect(GTK_OBJECT(event_box), "motion_notify_event",GTK_SIGNAL_FUNC(move_event), donnees);



// Mise en place du séparateur entre la carte et la barre temps
    Separateur = gtk_hseparator_new();
    gtk_box_pack_start (GTK_BOX (work_zl), Separateur, FALSE, TRUE, 5);


// Création du curseur temp (adjustment) et de son label
    hbox_curseur = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(work_zl), hbox_curseur, FALSE, FALSE, 0);
    label_curseur=gtk_label_new("Temps   ");
    gtk_box_pack_start(GTK_BOX(hbox_curseur), label_curseur, FALSE, FALSE, 0);

// Barre de temps
//  ( depart , min , max , incrément scroll x2 , incrémentation clic en dehors du curseur , zone visible )
    donnees->adj2 = gtk_adjustment_new (donnees->temps, 0.0, 1440.0, 0.5, 5.0, 0.0);
    gtk_signal_connect (GTK_OBJECT (donnees->adj2), "value_changed", GTK_SIGNAL_FUNC (recup_temps), donnees);
    curseur = gtk_hscale_new (GTK_ADJUSTMENT (donnees->adj2));
    gtk_scale_set_digits (GTK_SCALE (curseur), 0);
    gtk_scale_set_draw_value(GTK_SCALE(curseur),FALSE);
    gtk_box_pack_start (GTK_BOX (hbox_curseur), curseur, TRUE, TRUE, 0);

// Label temps à gauche de la barre de temps
    char heure[10];
    int h=donnees->temps/60;
    int m=donnees->temps-h*60;
    sprintf(heure,"   %02d:%02d",h,m);
    donnees->heure_label=gtk_label_new(heure);
    gtk_box_pack_start(GTK_BOX(hbox_curseur),donnees->heure_label,FALSE,FALSE,0);


// Boutons sur le menu de gauche
    filtre_button=gtk_button_new_with_mnemonic("_Filtrer l'affichage");
    gtk_box_pack_start(GTK_BOX(work_zr),filtre_button,FALSE,FALSE,0);
    g_signal_connect(GTK_BUTTON(filtre_button),"clicked",G_CALLBACK(filtres),donnees);

    voir_pdv_button=gtk_button_new_with_mnemonic("_Voir les plans de vols");
    gtk_box_pack_start(GTK_BOX(work_zr),voir_pdv_button,FALSE,FALSE,0);
    g_signal_connect(GTK_BUTTON(voir_pdv_button),"clicked",G_CALLBACK(voir_pdv),donnees);

    ajouter_pdv_button=gtk_button_new_with_mnemonic("_Ajouter un plan de vol");
    gtk_box_pack_start(GTK_BOX(work_zr),ajouter_pdv_button,FALSE,FALSE,0);
    g_signal_connect(GTK_BUTTON(ajouter_pdv_button),"clicked",G_CALLBACK(ajouter_plan_de_vol),formulaire);

    voir_conflits_button=gtk_button_new_with_mnemonic("Voir les _conflits");
    gtk_box_pack_start(GTK_BOX(work_zr),voir_conflits_button,FALSE,FALSE,0);
    g_signal_connect(GTK_BUTTON(voir_conflits_button),"clicked",G_CALLBACK(voir_conflits),donnees);


// Boîte pour l'animation
    GtkWidget* media_box;
    media_box=gtk_hbox_new(TRUE,0);
    gtk_box_pack_start(GTK_BOX(work_zr), media_box, FALSE, FALSE, 0);

    GtkWidget* play_b;
    GtkWidget* img;
    play_b=gtk_button_new();
    img=gtk_image_new_from_file("play");
    gtk_button_set_image(GTK_BUTTON(play_b), img);
    gtk_box_pack_start(GTK_BOX(media_box),play_b,TRUE,TRUE,0);
    g_signal_connect(GTK_BUTTON(play_b),"clicked",G_CALLBACK(play),donnees);

    GtkWidget* pause_b;
    GtkWidget* img2;
    pause_b=gtk_button_new();
    img2=gtk_image_new_from_file("pause");
    gtk_button_set_image(GTK_BUTTON(pause_b), img2);
    gtk_box_pack_start(GTK_BOX(media_box),pause_b,TRUE,TRUE,0);
    g_signal_connect(GTK_BUTTON(pause_b),"clicked",G_CALLBACK(stop),donnees);


// Initialisation du label de conflit
    donnees->Msg_conflit=gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(work_zr),donnees->Msg_conflit,FALSE,FALSE,10);

// Initialisation du label de distance clic
    donnees->Distance_clic = gtk_label_new("Distance clic :\n    0 NM");
    gtk_box_pack_start(GTK_BOX(work_zr),donnees->Distance_clic,FALSE,FALSE,10);

// Initialisation du label de position du curseur
    donnees->Position_curseur = gtk_label_new("Position du curseur : \nLatitude : \nLongitude");
    gtk_box_pack_start(GTK_BOX(work_zr),donnees->Position_curseur,FALSE,FALSE,10);

// Gère le rafraichissement
    g_signal_connect (G_OBJECT (donnees->carte), "expose-event", G_CALLBACK (expose_cb), donnees);

// Gère le redimensionnement
    g_signal_connect(donnees->Window, "size-allocate", G_CALLBACK(my_getsize), formulaire);

// Affiche des Widgets
    gtk_widget_show_all(donnees->Window);

}


// Donne la position du curseur
void move_event(GtkWidget* carte, GdkEventMotion* event, file_opener* donnees)
{
double x,y;
x = donnees->longitude_min + donnees->dlong * (event->x/donnees->xcarte);
y = donnees->latitude_max - donnees->dlat * (event->y/donnees->ycarte);

//printf("test x = %lf, y = %lf\n",x,y);
//fflush(stdout);

char label_position[100];
sprintf(label_position,"Position du curseur :\n Latitude : %lf \n Longitude : %lf",y,x);
gtk_label_set_text(GTK_LABEL(donnees->Position_curseur),label_position);
}


// Premier évenement du déplacement de la carte, on y récupère la position du curseur et le coordonnées de l'angle haut gauche
void press_event(GtkWidget* carte, GdkEventButton* event, file_opener* donnees)
{
// On sauvegarde le coin haut gauche
donnees->bord->x = donnees->longitude_min;
donnees->bord->y = donnees->latitude_max;

// On mémorise la position du curseur au moment du clic initiale
donnees->start->x = event->x;
donnees->start->y = event->y;

// On transforme la position en longitude/latitude
position clic;
clic.x = donnees->longitude_min + donnees->dlong * (event->x/donnees->xcarte);
clic.y = donnees->latitude_max - donnees->dlat * (event->y/donnees->ycarte);

// Utilisé pour le calcul des longueurs
donnees->old->x = clic.x;
donnees->old->y = clic.y;

//g_print("Clic en : lat = %lf, long = %lf\n",clic.y,clic.x);

}

// Second évènement de déplacement à partir de la différence entre la position actuelle du curseur et celle enregistrer par la fonction précédante on calcule le déplacement de la fenêtre
void drag_event(GtkWidget* carte, GdkEventMotion* event, file_opener* donnees)
{
GdkModifierType state;
state = event->state;

// Si le bouton cliqué est le clic gauche
  if (state & GDK_BUTTON1_MASK)
  {
    double longi = donnees->bord->x - ((event->x - donnees->start->x) * donnees->dlong / donnees->xcarte);
    double lati = donnees->bord->y + ((event->y - donnees->start->y) * donnees->dlat / donnees->ycarte);

// Protection contre les déplacement trop lointain, les bords de la carte sont fixés
    donnees->longitude_min = MAX(longi,-13.75);
    donnees->latitude_max = MIN(lati,57.25);

    if(donnees->longitude_min + donnees->dlong > 18.25)
    {
    donnees->longitude_min = MIN(longi + donnees->dlong,18.25) - donnees->dlong;
    }

    if(donnees->latitude_max - donnees->dlat < 35.25)
    {
    donnees->latitude_max = MAX(lati - donnees->dlat,35.25) + donnees->dlat;
    }
// On réactualise l'affichage
    redessiner(NULL,donnees->carte);
  }
// Calcul de la distance depuis le dernier clic droit
  else if(state & GDK_BUTTON3_MASK)
  {
    double longi = donnees->longitude_min + ((event->x) * donnees->dlong / donnees->xcarte);
    double lati = donnees->latitude_max - ((event->y) * donnees->dlat / donnees->ycarte);

    double dlat=3340*3.14/180*(lati - donnees->old->y);
    double latm=(lati + donnees->old->y)/2;
    double r=3340*cos(latm*3.14/180);
    double dlong=r*3.14*(donnees->old->x - longi)/180.0;
    donnees->clic_distance =sqrt(pow(dlat,2)+pow(dlong,2));

    char label_clic;
    sprintf(&label_clic,"Distance clic :\n    %0.1lf NM",donnees->clic_distance);
    gtk_label_set_text(GTK_LABEL(donnees->Distance_clic),&label_clic);

    //printf("test x = %lf, y = %lf\n",x,y);
    //fflush(stdout);

    char label_position[100];
    sprintf(label_position,"Position du curseur :\n Latitude : %lf \n Longitude : %lf",lati,longi);
    gtk_label_set_text(GTK_LABEL(donnees->Position_curseur),label_position);
  }
}

// Gère le zoom avec le scroll molette
void scroll_event(GtkWidget* carte,GdkEventScroll* event,file_opener* donnees)
{


// Facteur de zoom
double Z = 2;

// Protection dézoom
if(11/donnees->dlat <= 1 && event->direction)
{
// Recentrage et blocage du zoom
donnees->latitude_max = 57.25;
donnees->longitude_min = -13.75;

donnees->dlat = 22;
donnees->dlong = 32;
//g_print("Zoom min ! \n");
}
// Protection zoom
else if(11/donnees->dlat >= 48 && event->direction==0)
{
// Blocage du zoom

//g_print("Zoom max !\n");
}
else
{
    // Scroll up zoom
    if(event->direction==0)
    {
        // Nouvelle largeur et hauteur
        donnees->dlat = donnees->dlat / Z;
        donnees->dlong = donnees->dlat *16/11;

        // Repositionnement du bord de la carte
        donnees->latitude_max = (donnees->latitude_max - donnees->dlat * (Z -1) * event->y/donnees->ycarte);
        donnees->longitude_min = (donnees->longitude_min + donnees->dlong * (Z -1) * event->x/donnees->xcarte);

    }
    // Scroll down dézoom
    if(event->direction==1)
    {
        donnees->dlat = donnees->dlat * Z;
        donnees->dlong = donnees->dlat *16/11;

        donnees->latitude_max = (donnees->latitude_max + donnees->dlat * (1 - 1/Z) * event->y/donnees->ycarte);
        donnees->longitude_min = (donnees->longitude_min - donnees->dlong * (1 - 1/Z) * event->x/donnees->xcarte);
    }
}
redessiner(NULL,donnees->carte);
}


// Appelé lors de la modification du curseur temps
void recup_temps(GtkAdjustment *adj, file_opener *donnees)
{
donnees->temps = adj->value;

redessiner(NULL,donnees->carte);

// Modification du label à gauche de la barre
    int h=donnees->temps/60;
    int m=donnees->temps-h*60;
    char lab_heure[10];
    sprintf(lab_heure,"   %02d:%02d",h,m);
    gtk_label_set_text(GTK_LABEL(donnees->heure_label),lab_heure);

}


void APropos(GtkWidget* widget)
{
    GtkWidget* APropos_box;
    APropos_box = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "The GATI, 'The GNU Air Traffic Indicator' est un software devellopé dans le cadre d'un projet étudiant.\nCopyright (C) 2014  Fabien Bonneval, Xavier Durand.\n\nThis program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.\nThis program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.\nYou should have received a copy of the GNU General Public License along with this program.  If not, see http://www.gnu.org/licenses/.\n\nLe contour de la france est extrait des données sous licence Creative Commons mises à disposition par Bjorn Sandvik à l'adresse: http://thematicmapping.org/downloads/world_borders.php");

    gtk_dialog_run(GTK_DIALOG(APropos_box));
    gtk_widget_destroy(APropos_box);
}

// Affichage des plans de vol
void voir_pdv(GtkWidget *bouton, file_opener* donnees)
{
    GtkWidget *pdvw;
    GtkWidget* box;
    GtkWidget *scrollbar;

    //init fenêtre
    pdvw = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(GTK_WINDOW(pdvw), "Plans de vols");
    gtk_window_set_default_size(GTK_WINDOW(pdvw), 100, 500);
    gtk_window_set_position(GTK_WINDOW(pdvw),GTK_WIN_POS_CENTER);

    //init scrollbar
    scrollbar = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(pdvw),scrollbar);

 //création et ajout d'une boite dans la fenètre scroll
    box=gtk_vbox_new(FALSE,0);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrollbar), box);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbar), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS); //Never: désactive la barre, Always, l'inverse

// Si il y a des plans de vol
    if(donnees->debutpdv!=NULL)
    {

        pdv *pdv_current=donnees->debutpdv;
//->ptsuiv
        while(pdv_current!=NULL)
        {
        //  On affiche le premier texte de base
            char texte[3000];
            texte[0]='\0';
            sprintf(texte,"\n%s\n\tHeure de départ: %02d:%02d\n\tNiveau de vol: FL%d\n\tvitesse: %d kt    \n",pdv_current->nom,pdv_current->heure,pdv_current->minute,pdv_current->altitude,pdv_current->vitesse);
            pt_pass* passage=pdv_current->pass_debut;
            while(passage->ptsuiv!=NULL)
            {
                // Si le point de passage est un aérodrome
                if(passage->type_point==0)
                {
                    if(passage->point!=NULL)
                    {
                    // On remplie la ligne du point de passage.
                        aerodrome* pdvae=passage->point;
                        int k=(passage->temps/60);
                        int h=k%24;
                        int m=passage->temps-k*60;
                    sprintf(texte,"%s\n\t%s  %02d:%02d    ",texte,pdvae->nom,h,m);
//printf("aero:  %s\n",pdvae->nom);
                    }
                    else
                    {
                        sprintf(texte,"%s\n\tPoint introuvable  ",texte);
                    }



                }
                // Balise
                if(passage->type_point==1)
                {

                    if(passage->point!=NULL)
                    {
                        balise* pdvbal=passage->point;
                        int k=(passage->temps/60);
                        int h=k%24;
                        int m=passage->temps-k*60;
                        sprintf(texte,"%s\n\t%s  %02d:%02d    ",texte,pdvbal->nom,h,m);
//printf("bali:  %s\n",pdvbal->nom);
                    }
                    else
                    {
                        sprintf(texte,"%s\nPoint introuvable  ",texte);
                    }

                }

                passage=passage->ptsuiv;

            }
             sprintf(texte,"%s\n-  -  -  -  -  -  -  -  -  -  -  -  -  -\n",texte);

//g_print("%s",texte);

// Ajout label
    GtkWidget* label;
    label=gtk_label_new(texte);
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);

            pdv_current=pdv_current->ptsuiv;
        }

    }
    else
    {
    // Si aucun plan de vol
        GtkWidget* lab;
        lab=gtk_label_new("  Aucun plan de vol n'a été chargé !");
        gtk_box_pack_start(GTK_BOX(box),lab,FALSE,FALSE,0);
    }

        gtk_widget_show_all(pdvw);  //afficher la fenètre

}

// Chargement par défaut
void rapide_file(GtkWidget * widget, file_opener * donnees)
{
donnees->what_file=0;
donnees->ptchemin="aerodromes_fr.txt";

charger_fichiers(donnees);

donnees->what_file=1;
donnees->ptchemin="balises_fr.txt";

charger_fichiers(donnees);


donnees->what_file=2;
donnees->ptchemin="plan_de_vol.txt";

charger_fichiers(donnees);

}

// Force le rafraichissement
void redessiner(GtkWidget* button, GtkWidget * carte)
{
gtk_widget_queue_draw(carte);
}

// Boîte des paramètres
void parametres(GtkWidget* bouton, form_pdv* formulaire)
{
    GtkWidget* dist_conflits_label;
    GtkWidget* dist_conflits_spin;
    GtkWidget* delt_conflits_label;
    GtkWidget* delt_conflits_spin;

    file_opener* donnees=formulaire->donnees;

// Initialisation de la boîte de dialogue
    donnees->boite = gtk_dialog_new_with_buttons("Paramètres",
        NULL,
        GTK_DIALOG_MODAL,
        GTK_STOCK_OK,GTK_RESPONSE_OK,
        GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,
        NULL);


// Création des 2 paramètres
    dist_conflits_label = gtk_frame_new("Distance de détection des conflits (NM)");
    dist_conflits_spin = gtk_spin_button_new_with_range(0, 100, 1);
    gtk_container_add(GTK_CONTAINER(dist_conflits_label), dist_conflits_spin);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(donnees->boite)->vbox), dist_conflits_label, FALSE, FALSE, 0);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(dist_conflits_spin), donnees->distance_conflit);

    delt_conflits_label = gtk_frame_new("Temps entre deux détections de conflits et sauvegarde des points (s)");
    delt_conflits_spin = gtk_spin_button_new_with_range(1, 300, 1);
    gtk_container_add(GTK_CONTAINER(delt_conflits_label), delt_conflits_spin);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(donnees->boite)->vbox), delt_conflits_label, FALSE, FALSE, 0);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(delt_conflits_spin), donnees->deltat_conflits*60);


    gtk_widget_show_all(GTK_DIALOG(donnees->boite)->vbox);

// Choix
    switch (gtk_dialog_run(GTK_DIALOG(donnees->boite)))
    {

        case GTK_RESPONSE_OK:
            donnees->distance_conflit=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(dist_conflits_spin));
            donnees->deltat_conflits=gtk_spin_button_get_value(GTK_SPIN_BUTTON(delt_conflits_spin))/60;
            detection_conflits(NULL,donnees);

            gtk_widget_destroy(donnees->boite);
            break;

        case GTK_RESPONSE_CANCEL : gtk_widget_destroy(donnees->boite);break;
        default : break;
    }
}


// Affichage des conflits
void voir_conflits(GtkWidget *bouton, file_opener* donnees)
{
    GtkWidget *pdvw;
    GtkWidget* box;
    GtkWidget *scrollbar;

    //init fenêtre
    pdvw = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(GTK_WINDOW(pdvw), "Conflits");
    gtk_window_set_default_size(GTK_WINDOW(pdvw), 150, 500);
    gtk_window_set_position(GTK_WINDOW(pdvw),GTK_WIN_POS_CENTER);

    //init scrollbar
    scrollbar = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(pdvw),scrollbar);

 //création et ajout d'une boite dans la fenètre scroll
    box=gtk_vbox_new(FALSE,0);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrollbar), box);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbar), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS); //Never: désactive la barre, Always, l'inverse

    detection_conflits(NULL,donnees);

// On parcours les conflits en testant au préalable s'il y a des plans de vol et des conflits
    if(donnees->debutpdv!=NULL)
    {
        if(donnees->deb_conflits!=NULL)
        {

        conflit* conflit_current=donnees->deb_conflits;
            while(conflit_current->ptsuiv!=NULL)
            {
            // Construction du label à afficher
                char texte[300];
                texte[0]='\0';
                int k=(conflit_current->temps_deb/60);
                int h=k%24;
                int m=conflit_current->temps_deb-k*60;
                int direc;
                char dir;
                if(conflit_current->longitude<0)
                {
                    dir='W';
                    direc=-1;
                }
                else
                {
                dir='E';
                direc=1;
                }
                sprintf(texte,"CONFLIT entre %s et %s à %02d:%02d\t\n\t\tPosition: %lf°N  %lf°%c\t\n  -  -  -  -  -  -  -  -  -  -  -  \n",conflit_current->pdv1->nom,conflit_current->pdv2->nom,h,m,conflit_current->latitude,direc*conflit_current->longitude,dir);

    //g_print("%s",texte);

        GtkWidget* label;
        label=gtk_label_new(texte);
        gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);

                conflit_current=conflit_current->ptsuiv;
            }

    //      On rappel les paramètres de détection
            GtkWidget* params;
            char texte[300];texte[0]='\0';
            sprintf(texte,"\tParamètres:\n\t\tDistance de détection des conflits: %d NM\t\n\t\tTemps entre deux détections: %.0lf secondes\t",donnees->distance_conflit,donnees->deltat_conflits*60);
            params=gtk_label_new(texte);
            gtk_box_pack_start(GTK_BOX(box), params, FALSE, FALSE, 0);

        }
        else
        {
            GtkWidget* lab;
            lab=gtk_label_new("  Pas de conflits!   ");
            gtk_box_pack_start(GTK_BOX(box), lab, FALSE, FALSE, 0);
        }
    }
    else
    {
        GtkWidget* lab;
        lab=gtk_label_new("  Pas de plans de vol chargé!   ");
        gtk_box_pack_start(GTK_BOX(box), lab, FALSE, FALSE, 0);
    }

        gtk_widget_show_all(pdvw);  //afficher la fenètre

}

// Redéimensionnement des dimensions de la carte en fonction de la taille de la fenêtre
void my_getsize(GtkWidget *widget, GtkAllocation *allocation, form_pdv* formulaire)
{
//    printf("W: width = %d, height = %d\n", allocation->width, allocation->height);
    file_opener* donnees=formulaire->donnees;
    gtk_widget_set_size_request(donnees->carte, allocation->width-180, allocation->height-50);
    if((allocation->height)<(allocation->width))
    {
        donnees->ycarte=allocation->height-30;
        donnees->xcarte=donnees->ycarte/1.2;
    }
    else
    {
        donnees->xcarte=allocation->width-160;
        donnees->ycarte=donnees->xcarte*1.2;
    }
}



gboolean animation(file_opener* donnees)
{
    if(donnees->temps<24*60-1)
    {
        int c=0;
        conflit* conflit_current=donnees->deb_conflits;

        while(conflit_current!=NULL)
        {
            int dt1=abs(donnees->temps-conflit_current->temps_deb);
            int dt2=abs(donnees->temps-conflit_current->temps_fin);

            // Si il y a moins de 2 minutes avant le début d'un conflit ou la fin du conflit actuel
            if(dt1<2 || dt2<2)
            {
               c=1;
            }

            conflit_current=conflit_current->ptsuiv;
        }
        // Si le test précédant est vérifié on ralentie l'incrémentation du temps de l'animation, sinon le temps s'écoule normalement
        if(c)
        {
            donnees->temps+=0.2;
        }
        else
        {
        int a=0;
        pdv* pdv_current = donnees->debutpdv;

            while(pdv_current!=NULL)
            {
                if(pdv_current->temps_depart < donnees->temps + 10 && pdv_current->temps_arrivee > donnees->temps - 10)
                {
                    a=1;
                }
            pdv_current = pdv_current->ptsuiv;
            }
        if(a)
        {
            donnees->temps+=1;
        }
        else
        {
            donnees->temps+=5;
        }
        }

//      On réatribut la valeur du temps
        gtk_adjustment_set_value(GTK_ADJUSTMENT(donnees->adj2), donnees->temps);

    }


    return TRUE;
}

// Lance l'animation
void play(GtkWidget* bouton,file_opener* donnees)
{
    if(donnees->tag_lecture==0)
    {
        //gtk_timeout_remove (donnees->tag_lecture);
        donnees->tag_lecture=gtk_timeout_add(250,(GtkFunction) animation,donnees);
    }

}

// Arrete l'animation
void stop(GtkWidget* bouton,file_opener* donnees)
{
    if(donnees->tag_lecture!=0)
    {
        gtk_timeout_remove (donnees->tag_lecture);
        donnees->tag_lecture=0;
    }

}


