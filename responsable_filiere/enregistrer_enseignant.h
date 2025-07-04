#ifndef ENREGISTRER_ENSEIGNANT_H
#define ENREGISTRER_ENSEIGNANT_H

#include <gtk/gtk.h>

// Déclare la fonction qui affiche la fenêtre Enregistrer Enseignant
void enregistrer_enseignant_window(void (*retour_cb)(GtkWidget*, gpointer), gpointer data);

#endif