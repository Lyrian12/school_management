#ifndef EMPLOIE_DE_TEMPS_H
#define EMPLOIE_DE_TEMPS_H

#include <gtk/gtk.h>

// Déclare la fonction qui affiche la fenêtre Emploi du temps
void emploi_du_temps_window(void (*retour_cb)(GtkWidget*, gpointer), gpointer data);

#endif