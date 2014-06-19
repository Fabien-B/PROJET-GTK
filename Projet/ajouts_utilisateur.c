#include "interface.h"
#include "ouverture_fichiers.h"
#include "cartographie.h"
#include "filtrage.h"
#include "ajouts_utilisateur.h"

void ajouter_pt_pass(GtkWidget* bouton,form_pdv* formulaire)
{
    const gchar *text;
    formulaire->nb_pt_int++;
    text = gtk_entry_get_text(GTK_ENTRY(formulaire->nom_entry));
    strcpy(formulaire->nom,text);
g_print("%s\n",text);

     text = gtk_entry_get_text(GTK_ENTRY(formulaire->altitude_entry));
    strcpy(formulaire->altitude,text);
g_print("%s\n",text);

     text = gtk_entry_get_text(GTK_ENTRY(formulaire->vitesse_entry));
    strcpy(formulaire->vitesse,text);
g_print("%s\n",text);

int i;
    for(i=0;i<formulaire->nb_pt_int+1;i++)
    {
        text = gtk_entry_get_text(GTK_ENTRY(formulaire->pass_entry[i]));
        strcpy(formulaire->pass[i],text);
    g_print("%s\n",text);

    }

    formulaire->heure=gtk_spin_button_get_value(GTK_SPIN_BUTTON(formulaire->spinh));
    formulaire->minutes=gtk_spin_button_get_value(GTK_SPIN_BUTTON(formulaire->spinm));


    gtk_widget_destroy(formulaire->wind);
    ajouter_plan_de_vol(NULL,formulaire);

}




void ajouter_plan_de_vol(GtkWidget* bouton,form_pdv* formulaire)
{
    GtkWidget* box;
    GtkWidget* scrollbar;
    GtkWidget* aj_pass;
    GtkWidget* ok_button;
    formulaire->wind = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(GTK_WINDOW(formulaire->wind), "Ajouter un Plan de Vol");
    gtk_window_set_default_size(GTK_WINDOW(formulaire->wind), 200, 400);
    gtk_window_set_position(GTK_WINDOW(formulaire->wind),GTK_WIN_POS_CENTER);

    //init scrollbar
    scrollbar = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(formulaire->wind),scrollbar);

    //création et ajout d'une boite dans la fenètre scroll
    box=gtk_vbox_new(FALSE,0);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrollbar), box);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbar), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS); //Never: désactive la barre, Always, l'inverse




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


    formulaire->altitude_label=gtk_label_new("Niveau de vol:"); //label et gtkEntry pour le niveau de vol
    formulaire->altitude_entry=gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box),formulaire->altitude_label,FALSE,FALSE,0);
    gtk_box_pack_start(GTK_BOX(box),formulaire->altitude_entry,FALSE,FALSE,0);
    gtk_entry_set_text(GTK_ENTRY(formulaire->altitude_entry), formulaire->altitude);

    formulaire->vitesse_label=gtk_label_new("Vitesse de vol:"); //label et gtkEntry pour la vitesse
    formulaire->vitesse_entry=gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box),formulaire->vitesse_label,FALSE,FALSE,0);
    gtk_box_pack_start(GTK_BOX(box),formulaire->vitesse_entry,FALSE,FALSE,0);
    gtk_entry_set_text(GTK_ENTRY(formulaire->vitesse_entry), formulaire->vitesse);

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
    gtk_box_pack_start(GTK_BOX(box),aj_pass,FALSE,FALSE,5);
    g_signal_connect(GTK_BUTTON(aj_pass),"clicked",G_CALLBACK(ajouter_pt_pass),formulaire);

    ok_button=gtk_button_new_with_label("Confirmer"); //label et gtkEntry pour ajouter le plan de vol à la base de données
    gtk_box_pack_start(GTK_BOX(box),ok_button,FALSE,FALSE,0);
    g_signal_connect(GTK_BUTTON(ok_button),"clicked",G_CALLBACK(ajouter_pdv),formulaire);

    gtk_widget_show_all(formulaire->wind); //afficher la fenètre

}

void ajouter_pdv(GtkWidget* bouton,form_pdv* formulaire)
{


//g_print("%p %p %p\n",formulaire->donnees->debutaero,formulaire->donnees->debutbalises,formulaire->donnees->debutpdv);
int cas;
    pdv* pdv_current;
    if(formulaire->donnees->finpdv!=NULL)
    {
//g_print("SUITE\n");
        formulaire->donnees->finpdv->ptsuiv=malloc(sizeof(pdv));
        //formulaire->donnees->finpdv=formulaire->donnees->finpdv->ptsuiv;
        //pdv_current=formulaire->donnees->finpdv;
        pdv_current=formulaire->donnees->finpdv->ptsuiv;
        pdv_current->ptsuiv=NULL;
        cas=0;
    }
    else
    {
//g_print("DEBUT PDV\n");fflush(stdout);
        formulaire->donnees->debutpdv=malloc(sizeof(pdv));
        pdv_current=formulaire->donnees->debutpdv;
        formulaire->donnees->finpdv=pdv_current;
        pdv_current->ptsuiv=NULL;
        cas=1;
    }


    const gchar *text;
    text = gtk_entry_get_text(GTK_ENTRY(formulaire->nom_entry));
    strcpy(pdv_current->nom,text);
//g_print("%s\n",pdv_current->nom);

    pdv_current->heure=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(formulaire->spinh));
    pdv_current->minute=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(formulaire->spinm));
//g_print("%d:%d\n",pdv_current->heure,pdv_current->minute);


    text = gtk_entry_get_text(GTK_ENTRY(formulaire->altitude_entry));
    sscanf(text, "%d", &pdv_current->altitude);
//g_print("%d\n",pdv_current->altitude);

     text = gtk_entry_get_text(GTK_ENTRY(formulaire->vitesse_entry));
    sscanf(text, "%d", &pdv_current->vitesse);
//g_print("%d\n",pdv_current->vitesse);


    pdv_current->pass_debut=malloc(sizeof(pt_pass));
    pt_pass* pass_current=pdv_current->pass_debut;
    pass_current->point=NULL;
    pass_current->ptsuiv=NULL;
    int i;
    int er=0;
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
        printf("%s\n",text2);
                if(formulaire->donnees->debutbalises!=NULL)
                {
                    balise* pt_current=formulaire->donnees->debutbalises;
                    while(pt_current!=NULL)
                    {

                        if(!strcmp(text2,pt_current->nom))
                        {
                            pass_current->point=pt_current;
                            pass_current->type_point=1;
//printf("%s sélectionné\n",pt_current->nom);
                        }
                        pt_current=pt_current->ptsuiv;
                    }
                }


                if(formulaire->donnees->debutaero!=NULL)
                {
                    aerodrome* pt_current=formulaire->donnees->debutaero;
                    while(pt_current!=NULL)
                    {
                        if(!strcmp(text2,pt_current->oaci))
                        {
                            pass_current->point=pt_current;
                            pass_current->type_point=0;
//printf("%s sélectionné\n",pt_current->nom);
                        }
                        pt_current=pt_current->ptsuiv;
                    }

                }

        if(pass_current->point==NULL)
        {
            er++;
            printf("er=%d\n",er);
        }

    pass_current->ptsuiv=malloc(sizeof(pt_pass));
    pass_current=pass_current->ptsuiv;
    pass_current->ptsuiv=NULL;
    pass_current->point=NULL;



    }

    if(er)
    {
        pass_current=pdv_current->pass_debut;
        while(pass_current!=NULL)
        {
            pt_pass* pass2=pass_current->ptsuiv;
            free(pass_current);
            pass_current=pass2;
        }
        free(pdv_current);
        if(cas)
        {
            formulaire->donnees->debutpdv=NULL;
        }
        else
        {
            formulaire->donnees->finpdv->ptsuiv=NULL;
        }

    }
    else
    {
        formulaire->donnees->finpdv=formulaire->donnees->finpdv->ptsuiv;
    }

    pdv_current->affichage=0;

    gtk_widget_destroy(formulaire->wind);


    formulaire->nom[0]='\0'; //réinitialisation du formulaire
    formulaire->altitude[0]='\0';
    formulaire->vitesse[0]='\0';
    formulaire->heure=0;
    formulaire->minutes=0;
    for(i=0;i<10;i++)
    {
    formulaire->pass[i][0]='\0';
    }
    formulaire->nb_pt_int=0;


    integrer_temps(formulaire->donnees);

}

