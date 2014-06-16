#include "interface.h"
#include "ouverture_fichiers.h"
#include "cartographie.h"
#include "filtrage.h"
#include "ajouts_utilisateur.h"


void initialisation(int argc, char *argv[])
{
        // Initialisation de Gtk+
        g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, (GLogFunc) gtk_false, NULL);
        gtk_init (&argc, &argv);
        g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, g_log_default_handler, NULL);

        //initialisation des paramètres/donnees du programme
        file_opener *donnees=g_malloc(sizeof(file_opener));
        donnees->ptchemin=NULL;
        donnees->file_selection=NULL;
        donnees->debutaero=NULL;
        donnees->debutbalises=NULL;
        donnees->debutpdv=NULL;
        donnees->finpdv=NULL;
        donnees->finpdv=NULL;
        donnees->temps=0;
        donnees->distance_conflit=10;
        donnees->deltat_conflits=3;
        donnees->latitude_max=51.75;
        donnees->dlat=11;
        donnees->longitude_min=-5.75;
        donnees->dlong=16;
        donnees->xcarte=550;//550
        donnees->ycarte=660;//660


        form_pdv* formulaire=malloc(sizeof(form_pdv));
        formulaire->donnees=donnees;
        formulaire->nb_pt_int=0;
        formulaire->altitude[0]='\0';
        formulaire->nom[0]='\0';
        formulaire->vitesse[0]='\0';
        int i;
        for(i=0;i<40;i++)
        {
            formulaire->pass[i][0]='\0';
        }

        donnees->Window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(donnees->Window), "GATI");
    gtk_window_set_default_size(GTK_WINDOW(donnees->Window), 32, 40);
    g_signal_connect(G_OBJECT(donnees->Window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

        creer_interface(donnees,formulaire);
}


void creer_interface(file_opener* donnees,form_pdv* formulaire)
{

    GtkWidget *work_zone;
    GtkWidget *work_zl;
    GtkWidget *work_zr;

    GtkWidget *menu_bar;
    GtkWidget *Fichier_menu;
    GtkWidget *Aide_menu;
    GtkWidget *MI1_Fichier;
    GtkWidget *MI1_Aide;
    GtkWidget *MI2_APropos;
    GtkWidget *MI2_Nouveau;
    GtkWidget *MI2_Ouvrir;
    GtkWidget *MI2_Enregistrer;
    GtkWidget *MI2_Quitter;

    GtkWidget *hbox_curseur;
    GtkWidget *label_curseur;
    GtkWidget *curseur;
    GtkWidget *filtre_button;
    GtkWidget *voir_pdv_button;
    GtkWidget *ajouter_pdv_button;
    GtkWidget *detect_conflits_button;
    GtkWidget *parametres_button;
    GtkWidget *voir_conflits_button;
    GtkObject *adj2;
    GtkWidget* event_box;


// Creation et ajout de la GtkBox mère verticale
    donnees->mother_box = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(donnees->Window), donnees->mother_box);

//creation et ajout de la barre de menu et la zone de travail------------------------------------------------------------------------------------------------------------------------
    menu_bar = gtk_menu_bar_new();

    MI1_Fichier = gtk_menu_item_new_with_label("Fichier");
    Fichier_menu = gtk_menu_new();
        MI2_Nouveau = gtk_menu_item_new_with_label("Nouveau");
        gtk_menu_shell_append(GTK_MENU_SHELL(Fichier_menu), MI2_Nouveau);
            g_signal_connect(G_OBJECT(MI2_Nouveau), "activate", G_CALLBACK(liberer_memoire), donnees);

        MI2_Ouvrir = gtk_menu_item_new_with_label("Ouvrir");
        gtk_menu_shell_append(GTK_MENU_SHELL(Fichier_menu), MI2_Ouvrir);
                        g_signal_connect(G_OBJECT(MI2_Ouvrir), "activate", G_CALLBACK(lancer_boite), donnees);

        MI2_Enregistrer = gtk_menu_item_new_with_label("Enregistrer");
        gtk_menu_shell_append(GTK_MENU_SHELL(Fichier_menu), MI2_Enregistrer);
                        g_signal_connect(G_OBJECT(MI2_Enregistrer), "activate", G_CALLBACK(creer_file_save_selection), donnees);

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
    gtk_box_pack_start(GTK_BOX(donnees->mother_box), menu_bar, FALSE, FALSE, 0);
    //------------------------------------------------------------------------------------------------------------------------------------------------------------------------------







    work_zone = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(donnees->mother_box), work_zone, FALSE, FALSE, 0);
//Création et ajout des espaces de travails gauche et droite
    work_zl = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(work_zone), work_zl, TRUE, TRUE, 0);
    work_zr = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(work_zone), work_zr, FALSE, FALSE, 0);

//Création de la carte

    donnees->carte = gtk_drawing_area_new ();
    gtk_drawing_area_size (GTK_DRAWING_AREA(donnees->carte), donnees->xcarte,donnees->ycarte);
    //gtk_box_pack_start (GTK_BOX (event_box), donnees->carte, TRUE, TRUE, 0);
//g_signal_connect(donnees->carte, "size-allocate", G_CALLBACK(my_getsizecarte), NULL);


//    Ajout event box pour gérer le scroll/zoom
    event_box = gtk_event_box_new();
    gtk_box_pack_start (GTK_BOX (work_zl), event_box, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(event_box), donnees->carte);
    gtk_widget_set_events(event_box,GDK_SCROLL_UP);
    gtk_signal_connect(GTK_OBJECT(event_box), "scroll_event",GTK_SIGNAL_FUNC(scroll_event), donnees);
    gtk_widget_set_events(event_box,GDK_BUTTON_PRESS);
    g_print("adresse initiale : %p\n",donnees);
    gtk_signal_connect(GTK_OBJECT(event_box), "button_press_event",GTK_SIGNAL_FUNC(press_event), donnees);
    gtk_widget_set_events(event_box,GDK_MOTION_NOTIFY);
    gtk_signal_connect(GTK_OBJECT(event_box), "motion_notify_event",GTK_SIGNAL_FUNC(drag_event), donnees);

// Création du curseur temp (adjustment) et de son label
    hbox_curseur = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(work_zl), hbox_curseur, FALSE, FALSE, 0);
    label_curseur=gtk_label_new("Temps   ");
    gtk_box_pack_start(GTK_BOX(hbox_curseur), label_curseur, FALSE, FALSE, 0);

//  ( depart , min , max , incrément scroll x2 , incrémentation clic en dehors du curseur , ? )
    adj2 = gtk_adjustment_new (donnees->temps, 0.0, 1440.0, 0.5, 5.0, 0.0);
    gtk_signal_connect (GTK_OBJECT (adj2), "value_changed", GTK_SIGNAL_FUNC (recup_temps), donnees);
    curseur = gtk_hscale_new (GTK_ADJUSTMENT (adj2));
    gtk_scale_set_digits (GTK_SCALE (curseur), 0);
    gtk_scale_set_draw_value(GTK_SCALE(curseur),FALSE);
    gtk_box_pack_start (GTK_BOX (hbox_curseur), curseur, TRUE, TRUE, 0);
//g_signal_connect(hbox_curseur, "size-allocate", G_CALLBACK(my_getsizetemps), NULL);

    donnees->heure_label=gtk_label_new("   00:00");
    gtk_box_pack_start(GTK_BOX(hbox_curseur),donnees->heure_label,FALSE,FALSE,0);
//        gtk_range_set_update_policy (GTK_RANGE (curseur), GTK_UPDATE_DISCONTINUOUS);


    filtre_button=gtk_button_new_with_label("Filtres");
    gtk_box_pack_start(GTK_BOX(work_zr),filtre_button,FALSE,FALSE,0);
    g_signal_connect(GTK_BUTTON(filtre_button),"clicked",G_CALLBACK(filtres),donnees);

    voir_pdv_button=gtk_button_new_with_label("voir les plans de vols");
    gtk_box_pack_start(GTK_BOX(work_zr),voir_pdv_button,FALSE,FALSE,0);
    g_signal_connect(GTK_BUTTON(voir_pdv_button),"clicked",G_CALLBACK(voir_pdv),donnees);

    ajouter_pdv_button=gtk_button_new_with_label("Ajouter un plan de vol");
    gtk_box_pack_start(GTK_BOX(work_zr),ajouter_pdv_button,FALSE,FALSE,0);
    g_signal_connect(GTK_BUTTON(ajouter_pdv_button),"clicked",G_CALLBACK(ajouter_plan_de_vol),formulaire);

    detect_conflits_button=gtk_button_new_with_label("Détection des conflits");
    gtk_box_pack_start(GTK_BOX(work_zr),detect_conflits_button,FALSE,FALSE,0);
    g_signal_connect(GTK_BUTTON(detect_conflits_button),"clicked",G_CALLBACK(detection_conflits),donnees);

    parametres_button=gtk_button_new_with_label("Paramètres");
    gtk_box_pack_start(GTK_BOX(work_zr),parametres_button,FALSE,FALSE,0);
    g_signal_connect(GTK_BUTTON(parametres_button),"clicked",G_CALLBACK(parametres),formulaire);

    voir_conflits_button=gtk_button_new_with_label("Voir les conflits");
    gtk_box_pack_start(GTK_BOX(work_zr),voir_conflits_button,FALSE,FALSE,0);
    g_signal_connect(GTK_BUTTON(voir_conflits_button),"clicked",G_CALLBACK(voir_conflits),donnees);


// Gère le rafraichissement
    g_signal_connect (G_OBJECT (donnees->carte), "expose-event", G_CALLBACK (expose_cb), donnees);

// Debug rapide
     GtkWidget * rapide_file_button;

    rapide_file_button=gtk_button_new_with_label("Chargement rapide");
    gtk_box_pack_start(GTK_BOX(work_zr),rapide_file_button,FALSE,FALSE,0);
    g_signal_connect(GTK_BUTTON(rapide_file_button),"clicked",G_CALLBACK(rapide_file),donnees);



g_signal_connect(donnees->Window, "size-allocate", G_CALLBACK(my_getsize), formulaire);
    gtk_widget_show_all(donnees->Window);
}

void press_event(GtkWidget* carte, GdkEventButton* event, file_opener* donnees)
{
donnees->bord = malloc(sizeof(position));
donnees->start = malloc(sizeof(position));

//g_print("toujours vivant 0 avec adresse : %p , bord : %p, start : %p \n",donnees,donnees->bord,donnees->start);
donnees->bord->x = donnees->longitude_min;
donnees->bord->y = donnees->latitude_max;


donnees->start->x = event->x;
donnees->start->y = event->y;


}

void drag_event(GtkWidget* carte, GdkEventMotion* event, file_opener* donnees)
{
//g_print("ok\n");
 int x, y;
  GdkModifierType state;

  if (event->is_hint)
  {
//    g_print("quand ? \n\n");
    gdk_window_get_pointer (event->window, &x, &y, &state);
  }
  else
    {
      state = event->state;
    }



  if (state & GDK_BUTTON1_MASK )
  {
  donnees->longitude_min = donnees->bord->x - ((event->x - donnees->start->x) * donnees->dlong / donnees->xcarte);
        donnees->latitude_max = donnees->bord->y + ((event->y - donnees->start->y) * donnees->dlat / donnees->ycarte);
      x = event->x;
      y = event->y;
            redessiner(donnees->carte);
//  g_print("condition bizarre !\n");
  }
//  g_print("1 = %d, 2 = %d, 3 = %d, 4 = %d, 5 = %d",GDK_BUTTON1_MASK,GDK_BUTTON2_MASK,GDK_BUTTON3_MASK,GDK_BUTTON1_MASK,GDK_BUTTON1_MOTION_MASK);
//    g_print("delta x = %lf",((event->x - donnees->start->x) * donnees->dlat / donnees->xcarte) / 10000);



//g_print("x = %lf, axes = %lf, boutton = %d\n",event->x,event->axes,event->button);
//g_print("send_event = %d, state = %d, time = %d\n",event->send_event,event->state,event->time);
//g_print("type : %d\n",event->type);
}

void scroll_event(GtkWidget* carte,GdkEventScroll* event,file_opener* donnees)
{

// NOTE :
//- Limiter le zoom
//- Le rendre paramétrable
//- recentrer le zoom en cas de dézoom quasi max


// x = position x sur le programme, x_root = position sur l'écran (globale)
//double lat,lon;
//
//lon = donnees->longitude_min + ((event->x/donnees->xcarte) * donnees->dlong);
//lat = donnees->latitude_max - ((event->y/donnees->ycarte) * donnees->dlat);
//g_print("Scroll en x = %lf , y = %lf \n ce qui donne lat = %lf , long = %lf\n",event->x,event->y,lat,lon);



if(event->direction==0)
{
//donnees->latitude_max = donnees->latitude_max - 0.1;
//donnees->longitude_min = donnees->longitude_min + (0.1 *16/11);

donnees->dlat = donnees->dlat / 2;
donnees->dlong = donnees->dlat *16/11;

donnees->latitude_max = (donnees->latitude_max - donnees->dlat * event->y/donnees->ycarte);
donnees->longitude_min = (donnees->longitude_min + donnees->dlong * event->x/donnees->xcarte);

}
if(event->direction==1)
{
//donnees->latitude_max = donnees->latitude_max + 0.1;
//donnees->longitude_min = donnees->longitude_min - (0.1 *16/11);
donnees->latitude_max = (donnees->latitude_max + donnees->dlat * event->y/donnees->ycarte);
donnees->longitude_min = (donnees->longitude_min - donnees->dlong * event->x/donnees->xcarte);

donnees->dlat = donnees->dlat * 2;
donnees->dlong = donnees->dlat *16/11;



}

//donnees->latitude_max = (donnees->latitude_max - donnees->dlat * event->x/donnees->xcarte);
//donnees->longitude_min = (donnees->longitude_min + donnees->dlong * event->y/donnees->ycarte);

//donnees->latitude_max = (donnees->latitude_max + (lat + donnees->dlat/2) ) / 2;
//donnees->longitude_min = (donnees->longitude_min + (lon - donnees->dlong/2) ) /2;

//g_print("x = %lf, x_root = %lf, y = %lf, y_root = %lf\n",event->x,event->x_root,event->y,event->y_root);

//donnees->latitude_max=gtk_spin_button_get_value(GTK_SPIN_BUTTON(lat_coin_spin))+donnees->dlat/2;
//donnees->longitude_min=gtk_spin_button_get_value(GTK_SPIN_BUTTON(long_coin_spin))-donnees->dlong/2;
//
//double conversion_lat(double latitude, file_opener *donnees)
//{
//    double r=(donnees->latitude_max-latitude)/donnees->dlat;
//    return r;
//}
//
//double conversion_longitude(double longitude, file_opener *donnees)
//{
//    double r=(-donnees->longitude_min+longitude)/donnees->dlong;
//    return r;
//}

//g_print("test adresse event : %p \n\n",event);
//g_print("test adresse donnees fin : %p \n\n",donnees);
//
//
//g_print("dlat = %lf, dlong = %lf, lat max = %lf, long min = %lf\n\n",donnees->dlat,donnees->dlong,donnees->latitude_max,donnees->longitude_min);
redessiner(donnees->carte);

}

void recup_temps(GtkAdjustment *adj, file_opener *donnees)
{
donnees->temps = adj->value;
//printf("adj : %lf",adj->value);
//printf("donnees temps : %lf\n", donnees->temps);
redessiner(donnees->carte);

    int h=donnees->temps/60;
    int m=donnees->temps-h*60;
    char lab_heure[10];
    sprintf(lab_heure,"   %02d:%02d",h,m);
    gtk_label_set_text(GTK_LABEL(donnees->heure_label),lab_heure);
//    donnees->heure_label=gtk_label_new(lab_heure);
//    gtk_box_pack_start(GTK_BOX(hbox_curseur),donnees->heure_label,TRUE,TRUE,0);
}


void APropos(GtkWidget* widget)
{
    GtkWidget* APropos_box;
    APropos_box = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "The GATIP, 'The GNU Air Traffic Indicator' est un software devellopé dans le cadre d'un projet étudiant.");
    gtk_dialog_run(GTK_DIALOG(APropos_box));
    gtk_widget_destroy(APropos_box);
}

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
    box=gtk_vbox_new(FALSE,50);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrollbar), box);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbar), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS); //Never: désactive la barre, Always, l'inverse

    if(donnees->debutpdv!=NULL)
    {


        pdv *pdv_current=donnees->debutpdv;
//->ptsuiv
        while(pdv_current!=NULL)
        {
            char texte[3000];
            texte[0]='\0';
            sprintf(texte,"%s\n\tHeure de départ: %02d:%02d\n\tNiveau de vol: FL%d\n\tvitesse: %d kt    \n",pdv_current->nom,pdv_current->heure,pdv_current->minute,pdv_current->altitude,pdv_current->vitesse);
            pt_pass* passage=pdv_current->pass_debut;
            while(passage->ptsuiv!=NULL)
            {
                if(passage->type_point==0)
                {
                    if(passage->point!=NULL)
                    {
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
             sprintf(texte,"%s\n- - - - - - - - - - - - - -\n",texte);

g_print("%s",texte);

    GtkWidget* label;
    label=gtk_label_new(texte);
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);
    //gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);

            pdv_current=pdv_current->ptsuiv;
        }

    }
    else
    {
        GtkWidget* lab;
        lab=gtk_label_new("  Aucun plan de vol n'a été chargé !");
        gtk_box_pack_start(GTK_BOX(box),lab,FALSE,FALSE,0);
    }

        gtk_widget_show_all(pdvw);  //afficher la fenètre

}


void rapide_file(GtkWidget * widget, file_opener * donnees)
{
donnees->what_file=0;
donnees->ptchemin="aerodromes_fr.txt";

charger_fichiers(donnees);

donnees->what_file=1;
donnees->ptchemin="balises_fr.txt";

charger_fichiers(donnees);


donnees->what_file=2;
donnees->ptchemin="PVD_bon.txt";

charger_fichiers(donnees);

}


void redessiner(GtkWidget * carte) // force le rafraichissement
{
gtk_widget_queue_draw(carte);
}

void redessiner_widget(GtkWidget* button, GtkWidget * carte) // force le rafraichissement
{
gtk_widget_queue_draw(carte);
}





void parametres(GtkWidget* bouton, form_pdv* formulaire)
{
    //GtkWidget* boite;
    GtkWidget* dist_conflits_label;
    GtkWidget* dist_conflits_spin;
    GtkWidget* delt_conflits_label;
    GtkWidget* delt_conflits_spin;
    GtkWidget* dim_carte_label;
    GtkWidget* dim_carte_spin;
    GtkWidget* zoom_carte_label;
    GtkWidget* zoom_carte_spin;
    GtkWidget* lat_coin_label;
    GtkWidget* long_coin_label;
    GtkWidget* lat_coin_spin;
    GtkWidget* long_coin_spin;
    GtkWidget* pos_centre;
    GtkWidget* default_button;
    file_opener* donnees=formulaire->donnees;

    donnees->boite = gtk_dialog_new_with_buttons("Paramètres",
        NULL,
        GTK_DIALOG_MODAL,
        GTK_STOCK_OK,GTK_RESPONSE_OK,
        GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,
        NULL);

    default_button=gtk_button_new_with_label("Paramètres par défault");
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(donnees->boite)->vbox),default_button,FALSE,FALSE,0);
    g_signal_connect(GTK_BUTTON(default_button),"clicked",G_CALLBACK(visu_carte_default),formulaire);

    dist_conflits_label = gtk_frame_new("Distance de détection des conflits (NM)");
    dist_conflits_spin = gtk_spin_button_new_with_range(0, 100, 1);
    gtk_container_add(GTK_CONTAINER(dist_conflits_label), dist_conflits_spin);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(donnees->boite)->vbox), dist_conflits_label, FALSE, FALSE, 0);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(dist_conflits_spin), donnees->distance_conflit);

    delt_conflits_label = gtk_frame_new("granularité temporelle de détection des conflits (min)");
    delt_conflits_spin = gtk_spin_button_new_with_range(0.0, 30, 0.5);
    gtk_container_add(GTK_CONTAINER(delt_conflits_label), delt_conflits_spin);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(donnees->boite)->vbox), delt_conflits_label, FALSE, FALSE, 0);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(delt_conflits_spin), donnees->deltat_conflits);

    dim_carte_label = gtk_frame_new("hauteur de la carte");
    dim_carte_spin = gtk_spin_button_new_with_range(0, 1000, 25);
    gtk_container_add(GTK_CONTAINER(dim_carte_label), dim_carte_spin);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(donnees->boite)->vbox), dim_carte_label, FALSE, FALSE, 0);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(dim_carte_spin), donnees->xcarte);

    zoom_carte_label = gtk_frame_new("Zoom");
    zoom_carte_spin = gtk_spin_button_new_with_range(0, 11, 0.1);
    gtk_container_add(GTK_CONTAINER(zoom_carte_label), zoom_carte_spin);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(donnees->boite)->vbox), zoom_carte_label, FALSE, FALSE, 0);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(zoom_carte_spin), 11/donnees->dlat);


    pos_centre=gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(donnees->boite)->vbox), pos_centre,FALSE,FALSE,FALSE);

    lat_coin_label = gtk_frame_new("latitude du centre");
    lat_coin_spin = gtk_spin_button_new_with_range(40, 60, 0.2);
    gtk_container_add(GTK_CONTAINER(lat_coin_label), lat_coin_spin);
    gtk_box_pack_start(GTK_BOX(pos_centre), lat_coin_label, FALSE, FALSE, 0);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(lat_coin_spin), donnees->latitude_max-donnees->dlat/2);

    long_coin_label = gtk_frame_new("longitude du centre");
    long_coin_spin = gtk_spin_button_new_with_range(-10, 20, 0.2);
    gtk_container_add(GTK_CONTAINER(long_coin_label), long_coin_spin);
    gtk_box_pack_start(GTK_BOX(pos_centre), long_coin_label, FALSE, FALSE, 0);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(long_coin_spin), donnees->longitude_min+donnees->dlong/2);
    gtk_widget_show_all(GTK_DIALOG(donnees->boite)->vbox);


    switch (gtk_dialog_run(GTK_DIALOG(donnees->boite)))
    {

        case GTK_RESPONSE_OK:
            donnees->distance_conflit=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(dist_conflits_spin));
            donnees->deltat_conflits=gtk_spin_button_get_value(GTK_SPIN_BUTTON(delt_conflits_spin));
            donnees->xcarte=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(dim_carte_spin));
            donnees->ycarte=donnees->xcarte*1.2;
            donnees->dlat=11/gtk_spin_button_get_value(GTK_SPIN_BUTTON(zoom_carte_spin));
            donnees->dlong=donnees->dlat*16/11;
            donnees->latitude_max=gtk_spin_button_get_value(GTK_SPIN_BUTTON(lat_coin_spin))+donnees->dlat/2;
            donnees->longitude_min=gtk_spin_button_get_value(GTK_SPIN_BUTTON(long_coin_spin))-donnees->dlong/2;

            gtk_widget_destroy(donnees->mother_box);
            creer_interface(donnees,formulaire);
            gtk_window_resize(GTK_WINDOW(donnees->Window), 10, 10);
            break;

        case GTK_RESPONSE_CANCEL:
        case GTK_RESPONSE_NONE:
        default:

            break;
    }
    gtk_widget_destroy(donnees->boite);
}


void visu_carte_default(GtkWidget* button, form_pdv* formulaire)
{
        file_opener* donnees=formulaire->donnees;
        donnees->distance_conflit=10;
        donnees->deltat_conflits=3;
        donnees->latitude_max=51.75;
        donnees->dlat=11;
        donnees->longitude_min=-5.75;
        donnees->dlong=16;
        donnees->xcarte=550;//550
        donnees->ycarte=660;//660

        gtk_widget_destroy(donnees->boite);
        gtk_widget_destroy(donnees->mother_box);
        creer_interface(donnees,formulaire);
        gtk_window_resize(GTK_WINDOW(donnees->Window), 10, 10);
}





void voir_conflits(GtkWidget *bouton, file_opener* donnees)
{
    GtkWidget *pdvw;
    GtkWidget* box;
    GtkWidget *scrollbar;

    //init fenêtre
    pdvw = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(GTK_WINDOW(pdvw), "Conflits");
    gtk_window_set_default_size(GTK_WINDOW(pdvw), 10, 500);
    gtk_window_set_position(GTK_WINDOW(pdvw),GTK_WIN_POS_CENTER);

    //init scrollbar
    scrollbar = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(pdvw),scrollbar);

 //création et ajout d'une boite dans la fenètre scroll
    box=gtk_vbox_new(FALSE,0);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrollbar), box);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbar), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS); //Never: désactive la barre, Always, l'inverse

        conflit* conflit_current=donnees->deb_conflits;
        while(conflit_current->ptsuiv!=NULL)
        {
            char texte[300];
            texte[0]='\0';
            int k=(conflit_current->temps/60);
            int h=k%24;
            int m=conflit_current->temps-k*60;
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
            sprintf(texte,"CONFLIT entre %s et %s à %02d:%02d\t\n\t\tPosition: %lf°N  %lf°%c\t\n\t\tDistance=%.2lf NM\t\n  -  -  -  -  -  -  -  -  -  -  -  \n",conflit_current->pdv1->nom,conflit_current->pdv2->nom,h,m,conflit_current->latitude,direc*conflit_current->longitude,dir,conflit_current->D);

//g_print("%s",texte);

    GtkWidget* label;
    label=gtk_label_new(texte);
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);
    //gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);

            conflit_current=conflit_current->ptsuiv;
        }
        GtkWidget* params;
        char texte[300];texte[0]='\0';
        sprintf(texte,"\tParamètres:\n\t\tDistance de détection des conflits: %d NM\t\n\t\tTemps entre deux détections: %.2lf minutes\t",donnees->distance_conflit,donnees->deltat_conflits);
        params=gtk_label_new(texte);
        gtk_box_pack_start(GTK_BOX(box), params, FALSE, FALSE, 0);


        gtk_widget_show_all(pdvw);  //afficher la fenètre

}




void my_getsize(GtkWidget *widget, GtkAllocation *allocation, form_pdv* formulaire) {
//    printf("W: width = %d, height = %d\n", allocation->width, allocation->height);
    file_opener* donnees=formulaire->donnees;
    gtk_widget_set_size_request(donnees->carte, allocation->width-180, allocation->height-40);
    if((allocation->height)<(allocation->width))
    {
        donnees->ycarte=allocation->height-27;
        donnees->xcarte=donnees->ycarte/1.2;

    }
    else
    {
        donnees->xcarte=allocation->width-160;
        donnees->ycarte=donnees->xcarte*1.2;
    }

/*
    gtk_widget_destroy(donnees->mother_box);
            creer_interface(donnees,formulaire);
            gtk_window_resize(GTK_WINDOW(donnees->Window), 10, 10);
    */

}

/*
void my_getsizecarte(GtkWidget *widget, GtkAllocation *allocation, void *data) {
//    printf("Carte: width = %d, height = %d\n", allocation->width, allocation->height);

}

void my_getsizetemps(GtkWidget *widget, GtkAllocation *allocation, void *data) {
//    printf("Temps: width = %d, height = %d\n", allocation->width, allocation->height);
}
*/
