#ifndef ANNONCES_H
#define ANNONCES_H

#include <gtk/gtk.h>

// Déclare la fonction qui affiche la fenêtre Annonces
void annonces_window(void (*retour_cb)(GtkWidget*, gpointer), gpointer data);

#endif