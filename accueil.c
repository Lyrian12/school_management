#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include "accueil.h"
#include "responsable_filiere.h"
#include "departement_3iac.h"
static void on_responsable_clicked(GtkWidget *widget, gpointer data) {
    g_print("Responsable de filière sélectionné\n");
     gtk_widget_destroy(GTK_WIDGET(data));
    show_responsable_filiere();
}

static void on_gestion_clicked(GtkWidget *widget, gpointer data) {
    g_print("Gestion_Departement_3iac sélectionné\n");
    gtk_widget_destroy(GTK_WIDGET(data));
    show_departement_3iac();
}

    void show_accueil(){
    

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Accueil");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 30);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 40);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Bouton Responsable de filière
    GtkWidget *btn_responsable = gtk_button_new_with_label("Responsable de filière");
    gtk_widget_set_name(btn_responsable, "btn_accueil");
    g_signal_connect(btn_responsable, "clicked", G_CALLBACK(on_responsable_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), btn_responsable, TRUE, TRUE, 0);

    // Bouton Gestion_Departement_3iac
    GtkWidget *btn_gestion = gtk_button_new_with_label("Gestion_Departement_3iac");
    gtk_widget_set_name(btn_gestion, "btn_accueil");
    g_signal_connect(btn_gestion, "clicked", G_CALLBACK(on_gestion_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), btn_gestion, TRUE, TRUE, 0);

    // CSS pour les boutons
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider,
        "#btn_accueil {"
        "  background:rgb(169, 211, 211);"
        "  color: white;"
        "  font-weight: bold;"
        "  border-radius: 8px;"
        "  font-size: 16px;"
        "  transition: background 200ms;"
        "  padding: 12px 0;"
        "}"
        "#btn_accueil:hover {"
        "  background:rgb(27, 214, 73);"
        "}", -1, NULL);

    GtkStyleContext *context;
    context = gtk_widget_get_style_context(btn_responsable);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    context = gtk_widget_get_style_context(btn_gestion);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return ;
}