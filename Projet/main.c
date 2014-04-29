#include <stdlib.h>
#include <gtk/gtk.h>
#include "interface.h"

int main (int argc, char *argv[])
{

    init_interface(argc,argv);


    gtk_main ();
    return 0;
}
