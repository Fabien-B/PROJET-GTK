#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>


typedef struct{
GtkWidget *bt[3][3];
GtkWidget *labjoueur;
}bout;










void newgame(GtkWidget* newg, bout *data)
{
    int i=0;int j=0;
    for(i=0;i<3;i++)
    {
        for(j=0;j<3;j++)
        {
            gtk_button_set_label (GTK_BUTTON(data->bt[i][j])," ");
        }

    }
}





void infobox(GtkWidget* button)
{
    GtkWidget* boited;

    boited = gtk_message_dialog_new (NULL, GTK_DIALOG_MODAL,GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "le joueur %s à gagné !",gtk_button_get_label(GTK_BUTTON(button)));
    gtk_dialog_run(GTK_DIALOG(boited));

    gtk_widget_destroy(boited);
}

void winn(bout *dat, GtkWidget* button)
{
int gagn=0;


int i;
for(i=0;i<3;i++)
{
    int c1= !strcmp(gtk_button_get_label(GTK_BUTTON(dat->bt[i][0])),gtk_button_get_label(GTK_BUTTON(dat->bt[i][1]))) && strcmp(gtk_button_get_label(GTK_BUTTON(dat->bt[i][0]))," ");
    int c2= !strcmp(gtk_button_get_label(GTK_BUTTON(dat->bt[i][1])),gtk_button_get_label(GTK_BUTTON(dat->bt[i][2])));// && strcmp(gtk_button_get_label(GTK_BUTTON(dat->bt[i][0])),"-");

    int c3= !strcmp(gtk_button_get_label(GTK_BUTTON(dat->bt[0][i])),gtk_button_get_label(GTK_BUTTON(dat->bt[1][i]))) && strcmp(gtk_button_get_label(GTK_BUTTON(dat->bt[0][i]))," ");
    int c4= !strcmp(gtk_button_get_label(GTK_BUTTON(dat->bt[1][i])),gtk_button_get_label(GTK_BUTTON(dat->bt[2][i])));// && strcmp(gtk_button_get_label(GTK_BUTTON(dat->bt[0][i])),"-");

    if((c1 && c2) || (c3 && c4))
    {
        gagn=1;
    }
}

    int c5= !strcmp(gtk_button_get_label(GTK_BUTTON(dat->bt[0][0])),gtk_button_get_label(GTK_BUTTON(dat->bt[1][1]))) && strcmp(gtk_button_get_label(GTK_BUTTON(dat->bt[0][0]))," ");
    int c6= !strcmp(gtk_button_get_label(GTK_BUTTON(dat->bt[1][1])),gtk_button_get_label(GTK_BUTTON(dat->bt[2][2])));// && strcmp(gtk_button_get_label(GTK_BUTTON(dat->bt[0][0])),"-");

    int c7= !strcmp(gtk_button_get_label(GTK_BUTTON(dat->bt[0][2])),gtk_button_get_label(GTK_BUTTON(dat->bt[1][1]))) && strcmp(gtk_button_get_label(GTK_BUTTON(dat->bt[2][0]))," ");
    int c8= !strcmp(gtk_button_get_label(GTK_BUTTON(dat->bt[1][1])),gtk_button_get_label(GTK_BUTTON(dat->bt[2][0])));// && strcmp(gtk_button_get_label(GTK_BUTTON(dat->bt[0][0])),"-");

    if((c5 && c6) || (c7 && c8))
    {
        gagn=1;
    }

if(gagn)
{
    const gchar *az=gtk_button_get_label(GTK_BUTTON(button));
    g_print("le joueur %s à gagné!\n",az);
    infobox(button);
}


}




void change_player_button(GtkWidget* button, bout *dat)
{
    if((strcmp(gtk_button_get_label(GTK_BUTTON(button)),"X") && strcmp(gtk_button_get_label(GTK_BUTTON(button)),"O")))
    {
        g_print("change player\n");
        if(strcmp(gtk_label_get_label(GTK_LABEL(dat->labjoueur)),"joueur: X"))
        {
            gtk_label_set_label(GTK_LABEL(dat->labjoueur), "joueur: X");
        }
        else
        {
            gtk_label_set_label(GTK_LABEL(dat->labjoueur), "joueur: O");
        }
    }



    if((strcmp(gtk_button_get_label(GTK_BUTTON(button)),"X") && strcmp(gtk_button_get_label(GTK_BUTTON(button)),"O")))
    {
        if(strcmp(gtk_label_get_label(GTK_LABEL(dat->labjoueur)),"joueur: X"))
        {
            gtk_button_set_label (GTK_BUTTON(button), "X");
        }
        else
        {
            gtk_button_set_label (GTK_BUTTON(button), "O");
        }
        winn(dat,button);
    }

//winn(dat,button);
}








int main(int argc, char **argv)
{
    GtkWidget *pWindow;
    GtkWidget *mother_box;

    GtkWidget *menu;
    GtkWidget *plateau;
    GtkWidget *etat;
    GtkWidget *pButton_menu[2];
    GtkWidget *pButton_plateau[3][3];
    GtkWidget *labjoueur;


    gtk_init(&argc,&argv);
    pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(pWindow), "Jeu de Morpion");
    gtk_window_set_default_size(GTK_WINDOW(pWindow), 320, 400);
    g_signal_connect(G_OBJECT(pWindow), "destroy", G_CALLBACK(gtk_main_quit), NULL);


    /* Creation de la GtkBox mère verticale */
    mother_box = gtk_vbox_new(FALSE, 0);

    /* Ajout de la GtkVBox mère dans la fenetre */
    gtk_container_add(GTK_CONTAINER(pWindow), mother_box);

    /* Création sous boite menu, plateau et etat */
    menu = gtk_hbox_new(TRUE, 0);

    plateau = gtk_table_new(3, 3, TRUE);

    //plateau = gtk_hbox_new(TRUE, 0);
    etat = gtk_hbox_new(TRUE, 0);

    /*ajout des boites précédentes*/
    gtk_box_pack_start(GTK_BOX(mother_box), menu, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(mother_box), plateau, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(mother_box), etat, FALSE, FALSE, 0);

    /*création boutons menu*/
    pButton_menu[0] = gtk_button_new_with_label("nouvelle partie");
    pButton_menu[1] = gtk_button_new_with_label("quitter");

    /*ajout boutons menu dans le menu*/
    gtk_box_pack_start(GTK_BOX(menu), pButton_menu[0], TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(menu), pButton_menu[1], TRUE, TRUE, 0);



        /*création et ajout du label joueur*/
    labjoueur=gtk_label_new("joueur: X");
    gtk_container_add(GTK_CONTAINER(etat), labjoueur);




    /* Creation et ajout des boutons plateau*/

    int i=0; int j=0;
    for(i=0;i<3;i++)
    {
        for(j=0;j<3;j++)
        {
            pButton_plateau[i][j] = gtk_button_new_with_label(" ");
        }

    }



    i=0;j=0;
    for(i=0;i<3;i++)
    {
        for(j=0;j<3;j++)
        {
            gtk_table_attach_defaults(GTK_TABLE(plateau), pButton_plateau[i][j], j, j+1, i, i+1);
        }

    }
/*----------------------------------------------------------------------------------------------------------*/





    /*création de la structure de données*/
    bout *data=NULL;
    data=malloc(sizeof(bout));

        i=0;j=0;
        for(i=0;i<3;i++)
        {
            for(j=0;j<3;j++)
            {

                data->bt[i][j]=pButton_plateau[i][j];
            }

        }
    data->labjoueur=labjoueur;
/*----------------------------------------------------------------------------------------------------------*/






    /*faire marcher le bouton "quitter"*/
    g_signal_connect(G_OBJECT(pButton_menu[1]), "clicked", G_CALLBACK(gtk_main_quit), NULL);

    g_signal_connect(G_OBJECT(pButton_menu[0]), "clicked", G_CALLBACK(newgame), data);




/*changer le joueur et le bouton à l'appui sur un bouton*/
    i=0;j=0;
    for(i=0;i<3;i++)
    {
        for(j=0;j<3;j++)
        {
            g_signal_connect(G_OBJECT(pButton_plateau[i][j]), "clicked", G_CALLBACK(change_player_button), data);
        }

    }

/*----------------------------------------------------------------------------------------------------------*/






    gtk_widget_show_all(pWindow);
    gtk_main();
    return EXIT_SUCCESS;
}
