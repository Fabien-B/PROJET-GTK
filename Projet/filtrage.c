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

    //création et ajout d'une boite dans la boite scrollable
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
        saa=gtk_button_new_with_label("Sélectionner/(D) tout");
        is=gtk_button_new_with_label("Inverser la sélection");
        gtk_box_pack_start(GTK_BOX(boxaero),lab,FALSE,FALSE,0);
        gtk_box_pack_start(GTK_BOX(boxaero),saa,FALSE,FALSE,0);
        gtk_box_pack_start(GTK_BOX(boxaero),is,FALSE,FALSE,0);
        g_signal_connect(G_OBJECT(saa), "clicked", G_CALLBACK(select_all_aero), donnees);
        g_signal_connect(G_OBJECT(is), "clicked", G_CALLBACK(invert_selection_aero), donnees);

        aerodrome* pt_current=donnees->debutaero;
        while(pt_current->ptsuiv!=NULL)             //création et initialisation des checkbox
        {
            char label[100];
            sprintf(label,"%s :  %s\n",pt_current->oaci,pt_current->nom);
            pt_current->coch=gtk_check_button_new_with_label(label);
            gtk_box_pack_start(GTK_BOX(boxaero),pt_current->coch,FALSE,FALSE,0);
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pt_current->coch), (pt_current->affichage));
            g_signal_connect(G_OBJECT(pt_current->coch), "toggled", G_CALLBACK(check_aero), pt_current);
            g_signal_connect(G_OBJECT(pt_current->coch), "toggled", G_CALLBACK(redessiner), donnees->carte);
            pt_current=pt_current->ptsuiv;
        }


    }

    if(donnees->debutbalises!=NULL)
    {
        GtkWidget* lab;
        GtkWidget* sab;
        GtkWidget* is;

        lab=gtk_label_new("Balises");                    //un label et deux boutons pour aider à la sélection
        sab=gtk_button_new_with_label("Sélectionner/(D) tout");
        is=gtk_button_new_with_label("Inverser la sélection");
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
            g_signal_connect(G_OBJECT(pt_current->coch), "toggled", G_CALLBACK(redessiner), donnees->carte);
            pt_current=pt_current->ptsuiv;
        }


    }

    if(donnees->debutpdv!=NULL)
    {
        GtkWidget* lab;
        GtkWidget* sap;
        GtkWidget* is;

        lab=gtk_label_new("Plans de vols");                    //un label et deux boutons pour aider à la sélection
        sap=gtk_button_new_with_label("Sélectionner/(D) tout");
        is=gtk_button_new_with_label("Inverser la sélection");
        gtk_box_pack_start(GTK_BOX(boxpdv),lab,FALSE,FALSE,0);
        gtk_box_pack_start(GTK_BOX(boxpdv),sap,FALSE,FALSE,0);
        gtk_box_pack_start(GTK_BOX(boxpdv),is,FALSE,FALSE,0);
        g_signal_connect(G_OBJECT(sap), "clicked", G_CALLBACK(select_all_pdv), donnees);
        g_signal_connect(G_OBJECT(is), "clicked", G_CALLBACK(invert_selection_pdv), donnees);

        pdv* pt_current=donnees->debutpdv;
        while(pt_current!=NULL)                 //création et initialisation des checkbox
        {

            char label[100];

            sprintf(label,"%s\n",pt_current->nom);
            pt_current->coch=gtk_check_button_new_with_label(label);
            gtk_box_pack_start(GTK_BOX(boxpdv),pt_current->coch,FALSE,FALSE,0);
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pt_current->coch), (pt_current->affichage));
            g_signal_connect(G_OBJECT(pt_current->coch), "toggled", G_CALLBACK(check_pdv), pt_current);
            g_signal_connect(G_OBJECT(pt_current->coch), "toggled", G_CALLBACK(redessiner), donnees->carte);
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

