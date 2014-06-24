#include "ajouts_utilisateur.h"
#include "ouverture_fichiers.h"
#include "cartographie.h"
#include "filtrage.h"
#include "conflits.h"

// Ajoute un point de passage sur le menu
void ajouter_pt_pass(GtkWidget* bouton,form_pdv* formulaire)
{
// On augmente le nombre de points de passage
    const gchar *text;
    formulaire->nb_pt_int++;
    text = gtk_entry_get_text(GTK_ENTRY(formulaire->nom_entry));
    strcpy(formulaire->nom,text);

// On récupère toutes les infos pour détruire et reconstruire la fenêtre
    formulaire->altitude = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(formulaire->spinfl));
    formulaire->vitesse = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(formulaire->spinvi));

// On récupère les points existants
int i;
    for(i=0;i<formulaire->nb_pt_int+1;i++)
    {
        text = gtk_entry_get_text(GTK_ENTRY(formulaire->pass_entry[i]));
        strcpy(formulaire->pass[i],text);
    }

    formulaire->heure=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(formulaire->spinh));
    formulaire->minutes=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(formulaire->spinm));

// On récréer la fenêtre
    gtk_widget_destroy(formulaire->wind);
    ajouter_plan_de_vol(NULL,formulaire);

}


// enleve un point de passage sur le menu
void enlever_pt_pass(GtkWidget* bouton,form_pdv* formulaire)
{
// On augmente le nombre de points de passage
    const gchar *text;
    formulaire->nb_pt_int--;
    text = gtk_entry_get_text(GTK_ENTRY(formulaire->nom_entry));
    strcpy(formulaire->nom,text);

// On récupère toutes les infos pour détruire et reconstruire la fenêtre
    formulaire->altitude = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(formulaire->spinfl));
    formulaire->vitesse = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(formulaire->spinvi));

// On récupère les points existants
int i;
    for(i=0;i<formulaire->nb_pt_int+1;i++)
    {
        text = gtk_entry_get_text(GTK_ENTRY(formulaire->pass_entry[i]));
        strcpy(formulaire->pass[i],text);
    }

    formulaire->heure=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(formulaire->spinh));
    formulaire->minutes=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(formulaire->spinm));

// On récréer la fenêtre
    gtk_widget_destroy(formulaire->wind);
    ajouter_plan_de_vol(NULL,formulaire);

}



// Fenêtre principale d'ajout d'un plan de vol
void ajouter_plan_de_vol(GtkWidget* bouton,form_pdv* formulaire)
{
    GtkWidget* box;
    GtkWidget* scrollbar;
    GtkWidget* aj_pass;
    GtkWidget* el_pass;
    GtkWidget* ok_button;
    formulaire->wind = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    // Initialisation de la fenêtre
    gtk_window_set_title(GTK_WINDOW(formulaire->wind), "Ajouter un Plan de Vol");
    int h=400+formulaire->nb_pt_int*42;
    h=MIN(h,600);
    gtk_window_set_default_size(GTK_WINDOW(formulaire->wind), 250, h);
    gtk_window_set_position(GTK_WINDOW(formulaire->wind),GTK_WIN_POS_CENTER);

    //Initialisation de la scrollbar
    scrollbar = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(formulaire->wind),scrollbar);

    //création et ajout d'une boite dans la fenètre scroll
    box=gtk_vbox_new(FALSE,0);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrollbar), box);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbar), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS); //Never: désactive la barre, Always, l'inverse



// Construction du formulaire
    formulaire->nom_label=gtk_label_new("Indicatif de l'avion:"); //label et gtkEntry pour l'indicatif avion
    formulaire->nom_entry=gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box),formulaire->nom_label,FALSE,FALSE,0);
    gtk_box_pack_start(GTK_BOX(box),formulaire->nom_entry,FALSE,FALSE,0);
    gtk_entry_set_text(GTK_ENTRY(formulaire->nom_entry), formulaire->nom);


    formulaire->hour_box=gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(box), formulaire->hour_box,FALSE,FALSE,FALSE);

    formulaire->heure_label = gtk_frame_new("Heures");
    formulaire->spinh = gtk_spin_button_new_with_range(0, 23, 1);
    gtk_container_add(GTK_CONTAINER(formulaire->heure_label), formulaire->spinh);
    gtk_box_pack_start(GTK_BOX(formulaire->hour_box), formulaire->heure_label, FALSE, FALSE, 0);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(formulaire->spinh), formulaire->heure);

    formulaire->heure_label = gtk_frame_new("Minutes");
    formulaire->spinm = gtk_spin_button_new_with_range(0, 59, 1);
    gtk_container_add(GTK_CONTAINER(formulaire->heure_label), formulaire->spinm);
    gtk_box_pack_start(GTK_BOX(formulaire->hour_box), formulaire->heure_label, FALSE, FALSE, 0);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(formulaire->spinm), formulaire->minutes);

    formulaire->altitude_label = gtk_frame_new("Niveau de vol (FL)");
    formulaire->spinfl = gtk_spin_button_new_with_range(0, 700, 5);
    gtk_container_add(GTK_CONTAINER(formulaire->altitude_label), formulaire->spinfl);
    gtk_box_pack_start(GTK_BOX(box), formulaire->altitude_label, FALSE, FALSE, 0);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(formulaire->spinfl), formulaire->altitude);

    formulaire->vitesse_label = gtk_frame_new("Vitesse de vol (kt)");
    formulaire->spinvi = gtk_spin_button_new_with_range(0, 700, 10);
    gtk_container_add(GTK_CONTAINER(formulaire->vitesse_label), formulaire->spinvi);
    gtk_box_pack_start(GTK_BOX(box), formulaire->vitesse_label, FALSE, FALSE, 0);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(formulaire->spinvi), formulaire->vitesse);

    formulaire->pass_label[0]=gtk_label_new("Point de départ:"); //label et gtkEntry pour le pt de départ
    formulaire->pass_entry[0]=gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box),formulaire->pass_label[0],FALSE,FALSE,0);
    gtk_box_pack_start(GTK_BOX(box),formulaire->pass_entry[0],FALSE,FALSE,0);
    gtk_entry_set_text(GTK_ENTRY(formulaire->pass_entry[0]), formulaire->pass[0]);

    int i=1;
    while(i<formulaire->nb_pt_int+1) //label et gtkEntry pour les pts intermédiaires
    {
        formulaire->pass_label[i]=gtk_label_new("Point de passage:");
        formulaire->pass_entry[i]=gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(box),formulaire->pass_label[i],FALSE,FALSE,0);
        gtk_box_pack_start(GTK_BOX(box),formulaire->pass_entry[i],FALSE,FALSE,0);
        gtk_entry_set_text(GTK_ENTRY(formulaire->pass_entry[i]), formulaire->pass[i]);
        i++;
    }

    formulaire->pass_label[i]=gtk_label_new("Point d'arrivée:"); //label et gtkEntry pour le pt d'arrivée
    formulaire->pass_entry[i]=gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box),formulaire->pass_label[i],FALSE,FALSE,0);
    gtk_box_pack_start(GTK_BOX(box),formulaire->pass_entry[i],FALSE,FALSE,0);
    gtk_entry_set_text(GTK_ENTRY(formulaire->pass_entry[i]), formulaire->pass[i]);

    aj_pass=gtk_button_new_with_label("Ajouter un point de passage"); //bouton pour ajouter un pt de passage
    gtk_box_pack_start(GTK_BOX(box),aj_pass,FALSE,FALSE,0);
    g_signal_connect(GTK_BUTTON(aj_pass),"clicked",G_CALLBACK(ajouter_pt_pass),formulaire);

    el_pass=gtk_button_new_with_label("Retirer un point de passage"); //bouton pour ajouter un pt de passage
    gtk_box_pack_start(GTK_BOX(box),el_pass,FALSE,FALSE,0);
    g_signal_connect(GTK_BUTTON(el_pass),"clicked",G_CALLBACK(enlever_pt_pass),formulaire);

    ok_button=gtk_button_new_with_label("Confirmer"); //label et gtkEntry pour ajouter le plan de vol à la base de données
    gtk_box_pack_start(GTK_BOX(box),ok_button,FALSE,FALSE,10);
    g_signal_connect(GTK_BUTTON(ok_button),"clicked",G_CALLBACK(ajouter_pdv),formulaire);



    gtk_widget_show_all(formulaire->wind); //afficher la fenètre

}

// Fonction de validation sert aussi à l'édition des pdv
void ajouter_pdv(GtkWidget* bouton,form_pdv* formulaire)
{
    pdv* pdv_current;
    pt_pass* passdeb;
    int cas;
    if(formulaire->pdv_edit==NULL)
    {
        // Premier plan de vol sauvegardé
        if(formulaire->donnees->finpdv!=NULL)
        {
        // Allocation mémoire et création de la liste chainée
            formulaire->donnees->finpdv->ptsuiv=malloc(sizeof(pdv));
            pdv_current=formulaire->donnees->finpdv->ptsuiv;
            pdv_current->ptsuiv=NULL;
            cas=0;
        }
        else
        {
        // Ajout à la liste chainée
            formulaire->donnees->debutpdv=malloc(sizeof(pdv));
            pdv_current=formulaire->donnees->debutpdv;
            formulaire->donnees->finpdv=pdv_current;
            pdv_current->ptsuiv=NULL;
            cas=1;
        }
    }
    else
    {
        pdv_current=formulaire->pdv_edit;
        passdeb=pdv_current->pass_debut;
    }




// Récupération des données du formulaire
    const gchar *text;
    text = gtk_entry_get_text(GTK_ENTRY(formulaire->nom_entry));
    strcpy(pdv_current->nom,text);

    pdv_current->heure=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(formulaire->spinh));
    pdv_current->minute=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(formulaire->spinm));


    pdv_current->altitude = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(formulaire->spinfl));
    pdv_current->vitesse = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(formulaire->spinvi));


    pdv_current->pass_debut=malloc(sizeof(pt_pass));
    pt_pass* pass_current=pdv_current->pass_debut;
    pass_current->point=NULL;
    pass_current->ptsuiv=NULL;
    int i;
    int er=0;

    // Sauvegarde dans la structure
    for(i=0;i<formulaire->nb_pt_int+2;i++)
    {
        text = gtk_entry_get_text(GTK_ENTRY(formulaire->pass_entry[i]));
        char text2[40];
        strcpy(text2,text);
        int i=0;
        while(text2[i]!='\0')
        {
            text2[i]=toupper(text2[i]);
            i++;
        }
                if(formulaire->donnees->debutbalises!=NULL)
                {
                    balise* pt_current=formulaire->donnees->debutbalises;
                    // Recherche du point de passage parmis les balises
                    while(pt_current!=NULL)
                    {

                        if(!strcmp(text2,pt_current->nom))
                        {
                            pass_current->point=pt_current;
                            pass_current->type_point=1;
                        }
                        pt_current=pt_current->ptsuiv;
                    }
                }


                if(formulaire->donnees->debutaero!=NULL)
                {
                    aerodrome* pt_current=formulaire->donnees->debutaero;
                    // Recherche du point de passage parmis les aérodromes
                    while(pt_current!=NULL)
                    {
                        if(!strcmp(text2,pt_current->oaci))
                        {
                            pass_current->point=pt_current;
                            pass_current->type_point=0;
                        }
                        pt_current=pt_current->ptsuiv;
                    }

                }
// Si il y a une erreur
        if(pass_current->point==NULL)
        {
            er++;
        }

    pass_current->ptsuiv=malloc(sizeof(pt_pass));
    pass_current=pass_current->ptsuiv;
    pass_current->ptsuiv=NULL;
    pass_current->point=NULL;



    }

// En cas d'erreur destruction de la saisie
    if(er)
    {
        pass_current=pdv_current->pass_debut;
        while(pass_current!=NULL)
        {
            pt_pass* pass2=pass_current->ptsuiv;
            free(pass_current);
            pass_current=pass2;
        }

        if(formulaire->pdv_edit==NULL)
        {
            free(pdv_current);
        }
        else
        {
            pdv_current->pass_debut=passdeb;
        }

        if(formulaire->pdv_edit==NULL)
        {
            // Si le plan de vol n'est pas le premier
            if(cas)
            {
                formulaire->donnees->debutpdv=NULL;
            }
            else
            {
                formulaire->donnees->finpdv->ptsuiv=NULL;
            }
        }

    }
//  S'il n'y a pas d'erreur
    else
    {
        // Si le plan de vol n'est pas le premier
        if(cas)
        {
            formulaire->donnees->finpdv=pdv_current;
        }
        else
        {
            formulaire->donnees->finpdv=formulaire->donnees->finpdv->ptsuiv;
        }
    }

    pdv_current->affichage=1;

// Destruction et réinitialisation du formulaire
    gtk_widget_destroy(formulaire->wind);


    formulaire->nom[0]='\0'; //réinitialisation du formulaire
    formulaire->heure=0;
    formulaire->minutes=0;
    for(i=0;i<10;i++)
    {
    formulaire->pass[i][0]='\0';
    }
    formulaire->nb_pt_int=0;


    integrer_temps(formulaire->donnees);

}



void select_pdv_ed(GtkWidget *bouton, form_pdv* formulaire)
{
    GtkWidget* pBoite;
    pBoite = gtk_dialog_new_with_buttons("Sélection du plan de vol à éditer",NULL,GTK_DIALOG_MODAL,GTK_STOCK_OK,GTK_RESPONSE_OK,NULL);


    GtkWidget *fixed;
    GtkWidget *combo;
    GtkWidget *label;

    if(formulaire->donnees->debutpdv!=NULL)
    {
        label=gtk_label_new("Sélectionnez le plan de vol à éditer.");
        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(pBoite)->vbox), label, TRUE, TRUE, 10);

      fixed = gtk_fixed_new();

      combo = gtk_combo_box_new_text();

      pdv* current=formulaire->donnees->debutpdv;
      while(current!=NULL)
      {
        gtk_combo_box_append_text(GTK_COMBO_BOX(combo), current->nom);
        current=current->ptsuiv;
      }

      gtk_fixed_put(GTK_FIXED(fixed), combo, 50, 50);
      gtk_container_add(GTK_CONTAINER(GTK_DIALOG(pBoite)->vbox), fixed);


      g_signal_connect(G_OBJECT(combo), "changed",G_CALLBACK(combo_selected_pdv), formulaire);

        gtk_combo_box_set_active(GTK_COMBO_BOX(combo),0); //mettre le 1er en choix par défault.


    }
    else
    {
        label=gtk_label_new("    Aucun plan de vol n'a été chargé!    ");
        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(pBoite)->vbox), label, TRUE, TRUE, 10);
    }


    gtk_widget_show_all(GTK_DIALOG(pBoite)->vbox);


    if (gtk_dialog_run(GTK_DIALOG(pBoite))==GTK_RESPONSE_OK) //on n'édite le pdv que si l'utilisateur clique sur OK.
    {
        if(formulaire->donnees->debutpdv!=NULL)
        {
            ajouter_plan_de_vol(NULL,formulaire);
        }
    }
    gtk_widget_destroy(pBoite);
}



void combo_selected_pdv(GtkWidget *widget,form_pdv* formulaire)
{
  gchar *text =  gtk_combo_box_get_active_text(GTK_COMBO_BOX(widget));

  pdv* current=formulaire->donnees->debutpdv;
  while(current!=NULL)
  {
    if(!strcmp(text,current->nom))
    {
        formulaire->pdv_edit=current;
    }

    current=current->ptsuiv;
  }


    // On augmente le nombre de points de passage
    strcpy(formulaire->nom,formulaire->pdv_edit->nom);

    // On récupère toutes les infos pour détruire et reconstruire la fenêtre
    formulaire->altitude = formulaire->pdv_edit->altitude;
    formulaire->vitesse = formulaire->pdv_edit->vitesse;

    pt_pass* pass=formulaire->pdv_edit->pass_debut;
    int i=0;
    while(pass->ptsuiv!=NULL)
    {
        if(pass->type_point)    //balise
        {
            balise* bal=pass->point;
            strcpy(formulaire->pass[i],bal->nom);
            i++;
        }
        else        //aérodrome
        {
            aerodrome* aero=pass->point;
            strcpy(formulaire->pass[i],aero->oaci);
            i++;
        }
        pass=pass->ptsuiv;
    }
    formulaire->nb_pt_int=i-2;


    formulaire->heure=formulaire->pdv_edit->heure;
    formulaire->minutes=formulaire->pdv_edit->minute;






}

