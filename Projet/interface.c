#include "interface.h"
#include "ouverture_fichiers.h"
#include "cartographie.h"
#include "filtrage.h"

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
    GtkWidget *filtre_button;
    GtkWidget *voir_pdv_button;

    file_opener *donnees=g_malloc(sizeof(file_opener));
        donnees->ptchemin=NULL;
        donnees->file_selection=NULL;
        donnees->debutaero=NULL;
        donnees->debutbalises=NULL;
        donnees->debutpdv=NULL;


 // Initialisation de Gtk+
    g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, (GLogFunc) gtk_false, NULL);
    gtk_init (&argc, &argv);
    g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, g_log_default_handler, NULL);


    Window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(Window), "GATI");
    gtk_window_set_default_size(GTK_WINDOW(Window), 320, 400);
    g_signal_connect(G_OBJECT(Window), "destroy", G_CALLBACK(gtk_main_quit), NULL);


// Creation et ajout de la GtkBox mère verticale
    mother_box = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(Window), mother_box);

//creation et ajout de la barre de menu et la zone de travail------------------------------------------------------------------------------------------------------------------------
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
    //------------------------------------------------------------------------------------------------------------------------------------------------------------------------------







    work_zone = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(mother_box), work_zone, FALSE, FALSE, 0);
//Création et ajout des espaces de travails gauche et droite
    work_zl = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(work_zone), work_zl, TRUE, TRUE, 0);
    work_zr = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(work_zone), work_zr, FALSE, FALSE, 0);

//Création de la carte

    carte = gtk_drawing_area_new ();
    gtk_drawing_area_size (GTK_DRAWING_AREA(carte), 400,400);
    gtk_box_pack_start (GTK_BOX (work_zl), carte, TRUE, TRUE, 0);



    hbox_curseur = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(work_zl), hbox_curseur, TRUE, TRUE, 0);
    label_curseur=gtk_label_new("temps");
    gtk_box_pack_start(GTK_BOX(hbox_curseur), label_curseur, FALSE, FALSE, 0);
    curseur=gtk_hscale_new_with_range (5,200 ,100);
    gtk_container_add(GTK_CONTAINER(hbox_curseur), curseur);

    filtre_button=gtk_button_new_with_label("Filtres");
    gtk_box_pack_start(GTK_BOX(work_zr),filtre_button,FALSE,FALSE,0);
    g_signal_connect(GTK_BUTTON(filtre_button),"clicked",G_CALLBACK(filtres),donnees);

    voir_pdv_button=gtk_button_new_with_label("voir les plans de vols");
    gtk_box_pack_start(GTK_BOX(work_zr),voir_pdv_button,FALSE,FALSE,0);
    g_signal_connect(GTK_BUTTON(voir_pdv_button),"clicked",G_CALLBACK(voir_pdv),donnees);

// Gère le rafraichissement
    g_signal_connect (G_OBJECT (carte), "expose-event", G_CALLBACK (expose_cb), donnees);

    gtk_widget_show_all(Window);
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
    gtk_window_set_default_size(GTK_WINDOW(pdvw), 10, 500);
    gtk_window_set_position(GTK_WINDOW(pdvw),GTK_WIN_POS_CENTER);

    //init scrollbar
    scrollbar = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(pdvw),scrollbar);

    //création et ajout d'une boite dans la fenètre scroll
    box=gtk_hbox_new(FALSE,0);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrollbar), box);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbar), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS); //Never: désactive la barre, Always, l'inverse

    if(donnees->debutpdv!=NULL)
    {
        char texte[3000];
        texte[0]='\0';

        pdv *pdv_current=donnees->debutpdv;

        while(pdv_current!=NULL)
        {
            sprintf(texte,"%s\n\n%s\n\tNiveau de vol:FL%d\n",texte,pdv_current->nom,pdv_current->altitude);
            pt_pass* passage=pdv_current->pass_debut;

            while(passage->ptsuiv!=NULL)
            {
                if(passage->type_point==0)
                {
                    aerodrome* pdvae=passage->point;
                    sprintf(texte,"%s\n\t%s",texte,pdvae->nom);
                    //printf("aero:  %s\n",pdvae->nom);
                }
                if(passage->type_point==1)
                {
                    balise* pdvbal=passage->point;
                    sprintf(texte,"%s\n\t%s",texte,pdvbal->nom);
                    //printf("bali:  %s\n",pdvbal->nom);
                }

                passage=passage->ptsuiv;

            }
             sprintf(texte,"%s\n---------------",texte);




            pdv_current=pdv_current->ptsuiv;
        }







    GtkWidget* label;
    label=gtk_label_new(texte);
    gtk_container_add(GTK_CONTAINER(box), label);
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);


    }


        gtk_widget_show_all(pdvw);  //afficher la fenètre

}
