#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include "departement_3iac.h"
#include "departement3iac/gestiondepartement.h"
#include "departement3iac/direction.h"

 static void on_gestion_departement(GtkWidget *widget, gpointer data) {
    g_print("Gestion département sélectionné\n");
        gtk_widget_destroy(GTK_WIDGET(data));
    gestion_departement_window(NULL, NULL);
}

static void on_direction(GtkWidget *widget, gpointer data) {
    g_print("Direction sélectionné\n");
       gtk_widget_destroy(GTK_WIDGET(data));
    direction_window(NULL, NULL);
}

void show_departement_3iac() {


    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Département 3IAC");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 250);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 30);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 40);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Bouton Gestion département
    GtkWidget *btn_gestion = gtk_button_new_with_label("Gestion département");
    gtk_widget_set_name(btn_gestion, "btn_3iac");
    g_signal_connect(btn_gestion, "clicked", G_CALLBACK(on_gestion_departement),window);
    gtk_box_pack_start(GTK_BOX(vbox), btn_gestion, TRUE, TRUE, 0);

    // Bouton Direction
    GtkWidget *btn_direction = gtk_button_new_with_label("Direction");
    gtk_widget_set_name(btn_direction, "btn_3iac");
    g_signal_connect(btn_direction, "clicked", G_CALLBACK(on_direction), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), btn_direction, TRUE, TRUE, 0);

    // CSS pour les boutons
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider,
        "#btn_3iac {"
        "  background:rgb(169, 211, 211);"
        "  color: white;"
        "  font-weight: bold;"
        "  border-radius: 8px;"
        "  font-size: 16px;"
        "  transition: background 200ms;"
        "  padding: 12px 0;"
        "}"
        "#btn_3iac:hover {"
        "  background:rgb(27,214,73);"
        "}", -1, NULL);

    GtkStyleContext *context;
    context = gtk_widget_get_style_context(btn_gestion);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    context = gtk_widget_get_style_context(btn_direction);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return ;
}