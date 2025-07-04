#ifndef ENREGISTRER_ETUDIANT_H
#define ENREGISTRER_ETUDIANT_H

#include <gtk/gtk.h>

// Déclare la fonction qui affiche la fenêtre Enregistrer Étudiant
void enregistrer_etudiant_window(void (*retour_cb)(GtkWidget*, gpointer), gpointer data);

#endif