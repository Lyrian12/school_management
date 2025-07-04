#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include "responsable_filiere.h"
#include "responsable_filiere/enregistrer_etudiant.h"
#include "responsable_filiere/enregistrer_enseignant.h"
#include "responsable_filiere/annonces.h"
#include "responsable_filiere/Emploie_de_temps.h"
static void on_enregistrer_etudiant(GtkWidget *widget, gpointer data) {
    g_print("Enregistrer Élève sélectionné\n");
      gtk_widget_destroy(GTK_WIDGET(data));
    enregistrer_etudiant_window(NULL, NULL);
}

static void on_enregistrer_enseignant(GtkWidget *widget, gpointer data) {
    g_print("Enregistrer Enseignant sélectionné\n");
     gtk_widget_destroy(GTK_WIDGET(data));
    enregistrer_enseignant_window(NULL, NULL);
}

static void on_annonces(GtkWidget *widget, gpointer data) {
    g_print("Annonces sélectionné\n");
    gtk_widget_destroy(GTK_WIDGET(data));
    annonces_window(NULL, NULL);
}

static void on_emploi_du_temps(GtkWidget *widget, gpointer data) {
    g_print("Emploi du temps sélectionné\n");
     gtk_widget_destroy(GTK_WIDGET(data));
    emploi_du_temps_window(NULL, NULL);
}

void show_responsable_filiere(){
    

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Responsable de filière");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 25);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 40);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Bouton Enregistrer Élève
    GtkWidget *btn_eleve = gtk_button_new_with_label("Enregistrer Élève");
    gtk_widget_set_name(btn_eleve, "btn_rf");
    g_signal_connect(btn_eleve, "clicked", G_CALLBACK(on_enregistrer_etudiant), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), btn_eleve, TRUE, TRUE, 0);

    // Bouton Enregistrer Enseignants
    GtkWidget *btn_enseignant = gtk_button_new_with_label("Enregistrer Enseignants");
    gtk_widget_set_name(btn_enseignant, "btn_rf");
    g_signal_connect(btn_enseignant, "clicked", G_CALLBACK(on_enregistrer_enseignant), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), btn_enseignant, TRUE, TRUE, 0);

    // Bouton Annonces
    GtkWidget *btn_annonces = gtk_button_new_with_label("Annonces");
    gtk_widget_set_name(btn_annonces, "btn_rf");
    g_signal_connect(btn_annonces, "clicked", G_CALLBACK(on_annonces), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), btn_annonces, TRUE, TRUE, 0);

    // Bouton Emploi du temps
    GtkWidget *btn_emploi = gtk_button_new_with_label("Emploi du temps");
    gtk_widget_set_name(btn_emploi, "btn_rf");
    g_signal_connect(btn_emploi, "clicked", G_CALLBACK(on_emploi_du_temps), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), btn_emploi, TRUE, TRUE, 0);

    // CSS pour les boutons
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider,
        "#btn_rf {"
        "  background:rgb(169, 211, 211);"
        "  color: white;"
        "  font-weight: bold;"
        "  border-radius: 8px;"
        "  font-size: 16px;"
        "  transition: background 200ms;"
        "  padding: 12px 0;"
        "}"
        "#btn_rf:hover {"
        "  background:rgb(27, 214, 73);"
        "}", -1, NULL);

    GtkStyleContext *context;
    context = gtk_widget_get_style_context(btn_eleve);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    context = gtk_widget_get_style_context(btn_enseignant);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    context = gtk_widget_get_style_context(btn_annonces);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    context = gtk_widget_get_style_context(btn_emploi);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return ;
}