#ifndef GESTIONDEPARTEMENT_H
#define GESTIONDEPARTEMENT_H

#include <gtk/gtk.h>

// Déclare la fonction qui affiche la fenêtre Gestion Département
void gestion_departement_window(void (*retour_cb)(GtkWidget*, gpointer), gpointer data);

#endif