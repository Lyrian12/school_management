#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <libpq-fe.h>
#include "connexion.h"
#include "accueil.h"

static void on_connect_clicked(GtkWidget *widget, gpointer data) {
    g_print("Connexion...\n");
    GtkWidget **entries = (GtkWidget **)data;
    const char *nom = gtk_entry_get_text(GTK_ENTRY(entries[0]));
    const char *mdp = gtk_entry_get_text(GTK_ENTRY(entries[1]));

    const char *conninfo = "host=localhost port=2006 dbname=Academie user=postgres password=LYRIAN22";
    PGconn *conn = PQconnectdb(conninfo);

    if (PQstatus(conn) != CONNECTION_OK) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Erreur de connexion à la base !");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        PQfinish(conn);
        return;
    }
    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM utilisateur WHERE nom_utilisateur='%s' AND mot_de_passe='%s'", nom, mdp);

    PGresult *res = PQexec(conn, query);

    if (PQntuples(res) == 1) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Connexion réussie !");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        PQclear(res);
        PQfinish(conn);
        show_accueil();
    } else {
        GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Nom ou mot de passe incorrect !");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        PQclear(res);
        PQfinish(conn);
    }
    free(entries);
}

void show_connexion() {
    // Création de la fenêtre principale
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Page de connexion");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 500);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    // Conteneur principal vertical
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 30);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Titre
    GtkWidget *label_title = gtk_label_new("<span font='24' weight='bold'>Bienvenue à IUC</span>");
    gtk_label_set_use_markup(GTK_LABEL(label_title), TRUE);
    gtk_box_pack_start(GTK_BOX(vbox), label_title, FALSE, FALSE, 10);

    // Logo centré
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_scale("logo.png", 100, 100, TRUE, NULL);
    GtkWidget *logo = gtk_image_new_from_pixbuf(pixbuf);
    g_object_unref(pixbuf);
    gtk_box_pack_start(GTK_BOX(vbox), logo, FALSE, FALSE, 10);

    // Champ Nom
    GtkWidget *entry_nom = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_nom), "Nom");
    gtk_box_pack_start(GTK_BOX(vbox), entry_nom, FALSE, FALSE, 10);

    // Champ Mot de passe
    GtkWidget *entry_id = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_id), "Mot de passe");
    gtk_entry_set_visibility(GTK_ENTRY(entry_id), FALSE);
    gtk_box_pack_start(GTK_BOX(vbox), entry_id, FALSE, FALSE, 10);

    // Espace flexible
    GtkWidget *spacer = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(vbox), spacer, TRUE, TRUE, 10);

    // Conteneur horizontal pour le bouton
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
    gtk_box_pack_end(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

    // Bouton Se connecter
    GtkWidget *btn_connect = gtk_button_new_with_label("Se connecter");
    gtk_widget_set_name(btn_connect, "btn_connect");
    gtk_box_pack_start(GTK_BOX(hbox), btn_connect, TRUE, TRUE, 0);

    // CSS pour le bouton
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider,
        "#btn_connect {"
        "  background:rgb(169, 211, 211);"
        "  color: white;"
        "  font-weight: bold;"
        "  border-radius: 8px;"
        "  transition: background 200ms;"
        "}"
        "#btn_connect:hover {"
        "  background:rgb(27, 214, 73);"
        "}", -1, NULL);

    GtkStyleContext *context = gtk_widget_get_style_context(btn_connect);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Passage des entrées à la callback
    GtkWidget **entries = malloc(2 * sizeof(GtkWidget*));
    entries[0] = entry_nom;
    entries[1] = entry_id;
    g_signal_connect(btn_connect, "clicked", G_CALLBACK(on_connect_clicked), entries);

    gtk_widget_show_all(window);
}