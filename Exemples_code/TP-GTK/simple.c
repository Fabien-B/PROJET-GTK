#include <gtk/gtk.h>

void affiche(GtkWidget *widget, char *data) {
	g_print("1\n %s",data);
}
int main(int argc,char *argv[]){
	GtkWidget *fenetre,*bouton;

	g_print("2\n");

	gtk_init(&argc,&argv);
	g_print("2\n");
	fenetre=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_print("2\n");
	gtk_window_set_title(GTK_WINDOW(fenetre),"flot execution");

	bouton = gtk_button_new_with_label("press me");
	gtk_container_add(GTK_CONTAINER(fenetre),bouton);
    char chaine[]="azerty";
	g_signal_connect( G_OBJECT(bouton),"clicked",G_CALLBACK(affiche),chaine);
	g_signal_connect(G_OBJECT(fenetre),"delete-event",G_CALLBACK(gtk_main_quit),NULL);

	g_print("3\n");

	gtk_widget_show_all(fenetre);
	gtk_main();
	g_print("4\n");

	return 0;
}
