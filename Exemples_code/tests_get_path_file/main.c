#include "interface.h"
//structure pour les fichiers de donnees



void init_interface(int argc, char *argv[])
{
    GtkWidget *Window;
    GtkWidget *mother_box;

    GtkWidget *work_zone;
    GtkWidget *work_zl;
    GtkWidget *work_zr;
//    GtkWidget *carte;

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

    file_pack *donnees=g_malloc(sizeof(file_pack));
        donnees->chemin_aerodromes=NULL;
        donnees->chemin_balises=NULL;
        donnees->file_selection=NULL;
        donnees->vols=NULL;
//    GtkWidget *menu_item;
//

    gtk_init(&argc,&argv);

    Window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(Window), "Le projet de ouf!!!");
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
            //g_signal_connect(G_OBJECT(MI2_Ouvrir), "activate", G_CALLBACK(file_selection), donnees);
                        g_signal_connect(G_OBJECT(MI2_Ouvrir), "activate", G_CALLBACK(creer_file_selection), donnees);
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

    //carte=gtk_drawing_area_new ();
    //gtk_box_pack_start(GTK_BOX(work_zl), carte, FALSE, FALSE, 0);



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
    APropos_box = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Coded by Fabien, called 'The Master'...");
    gtk_dialog_run(GTK_DIALOG(APropos_box));
    gtk_widget_destroy(APropos_box);
}




void creer_file_selection(GtkWidget *bouton,file_pack *donnees)
{

    donnees->file_selection = gtk_file_selection_new( g_locale_to_utf8( "Sélectionnez un fichier", -1, NULL, NULL, NULL) );
    gtk_widget_show(donnees->file_selection);

    gtk_window_set_modal(GTK_WINDOW(donnees->file_selection), TRUE);

    g_signal_connect(G_OBJECT(GTK_FILE_SELECTION(donnees->file_selection)->ok_button), "clicked", G_CALLBACK(recuperer_chemin), donnees );

    g_signal_connect_swapped(G_OBJECT(GTK_FILE_SELECTION(donnees->file_selection)->cancel_button), "clicked", G_CALLBACK(gtk_widget_destroy), donnees->file_selection);
}

void recuperer_chemin(GtkWidget *bouton, file_pack *donnees)
{
    const gchar* chemin_fichier;
    chemin_fichier = gtk_file_selection_get_filename(GTK_FILE_SELECTION (donnees->file_selection) );

    donnees->chemin_aerodromes = malloc (sizeof *(donnees->chemin_aerodromes) * strlen (chemin_fichier) + 1);
    strcpy (donnees->chemin_aerodromes, chemin_fichier);

    gtk_widget_destroy(donnees->file_selection);

     charger_fichiers(donnees); //à remplacer par la fonction de Xavier.
}

void charger_fichiers(file_pack *donnees)
{
    g_print("%s",donnees->chemin_aerodromes);
    donnees->file_selection=NULL;
}































void combo_selected(GtkWidget *widget, gpointer window)
{
  gchar *text =  gtk_combo_box_get_active_text(GTK_COMBO_BOX(widget));
  gtk_label_set_text(GTK_LABEL(window), text);
  g_free(text);
}


int main( int argc, char *argv[])
{

  GtkWidget *window;
  GtkWidget *fixed;
  GtkWidget *combo;
  GtkWidget *label;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "GtkCombo");
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 230, 150);

  fixed = gtk_fixed_new();

  combo = gtk_combo_box_new_text();
  gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "Ubuntu");
  gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "Mandriva");
  gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "Fedora");
  gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "Mint");
  gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "Gentoo");
  gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "Debian");

  gtk_fixed_put(GTK_FIXED(fixed), combo, 50, 50);
  gtk_container_add(GTK_CONTAINER(window), fixed);

  label = gtk_label_new("-");
  gtk_fixed_put(GTK_FIXED(fixed), label, 50, 110);

  g_signal_connect_swapped(G_OBJECT(window), "destroy",
        G_CALLBACK(gtk_main_quit), G_OBJECT(window));

  g_signal_connect(G_OBJECT(combo), "changed",
        G_CALLBACK(combo_selected), (gpointer) label);

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}



