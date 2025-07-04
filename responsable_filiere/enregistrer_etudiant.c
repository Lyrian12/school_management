#include <gtk/gtk.h>
#include "enregistrer_etudiant.h"
#include <libpq-fe.h>

// Structure pour passer les widgets à la callback
typedef struct {
    GtkWidget *window;
    GtkWidget *entry_nom;
    GtkWidget *entry_prenom;
    GtkWidget *entry_matricule;
    GtkWidget *entry_telephone;
    GtkWidget *combo_sexe;
    GtkWidget *entry_email;
} EtudiantForm;

// Callback Ajouter
static void on_ajouter_clicked(GtkWidget *widget, gpointer data) {
    EtudiantForm *form = (EtudiantForm *)data;
    const char *nom = gtk_entry_get_text(GTK_ENTRY(form->entry_nom));
    const char *prenom = gtk_entry_get_text(GTK_ENTRY(form->entry_prenom));
    const char *matricule_str = gtk_entry_get_text(GTK_ENTRY(form->entry_matricule));
    const char *telephone_str = gtk_entry_get_text(GTK_ENTRY(form->entry_telephone));
    const char *sexe = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(form->combo_sexe));
    const char *email = gtk_entry_get_text(GTK_ENTRY(form->entry_email));

    // Vérification simple : champs obligatoires
    if (!*nom || !*prenom || !*matricule_str || !*telephone_str || !sexe || !*email) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(form->window), GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "Veuillez remplir tous les champs !");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        if (sexe) g_free((gpointer)sexe);
        return;
    }

    // Conversion en int
    int matricule = atoi(matricule_str);
    int telephone = atoi(telephone_str);

    if (matricule <= 0 || telephone <= 0) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(form->window), GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "Matricule et téléphone doivent être des nombres valides !");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        if (sexe) g_free((gpointer)sexe);
        return;
    }

    const char *conninfo = "host=localhost port=2006 dbname=Academie user=postgres password=LYRIAN22";
    PGconn *conn = PQconnectdb(conninfo);

    if (PQstatus(conn) != CONNECTION_OK) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(form->window), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Erreur de connexion à la base !");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        PQfinish(conn);
        if (sexe) g_free((gpointer)sexe);
        return;
    }

    char query[512];
    snprintf(query, sizeof(query),
        "INSERT INTO etudiant (nom, prenom, matricule, telephone, sexe, email) VALUES ('%s', '%s', %d, %d, '%s', '%s')",
        nom, prenom, matricule, telephone, sexe, email);

    PGresult *res = PQexec(conn, query);

    if (PQresultStatus(res) == PGRES_COMMAND_OK) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(form->window), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Étudiant ajouté avec succès !");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        // Nettoie les champs après ajout
        gtk_entry_set_text(GTK_ENTRY(form->entry_nom), "");
        gtk_entry_set_text(GTK_ENTRY(form->entry_prenom), "");
        gtk_entry_set_text(GTK_ENTRY(form->entry_matricule), "");
        gtk_entry_set_text(GTK_ENTRY(form->entry_telephone), "");
        gtk_combo_box_set_active(GTK_COMBO_BOX(form->combo_sexe), 0);
        gtk_entry_set_text(GTK_ENTRY(form->entry_email), "");
    } else {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(form->window), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
            "Erreur lors de l'ajout !\n%s", PQerrorMessage(conn));
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
    PQclear(res);
    PQfinish(conn);
    if (sexe) g_free((gpointer)sexe);
}

// Callback Annuler : nettoie les champs
static void on_annuler_clicked(GtkWidget *widget, gpointer data) {
    EtudiantForm *form = (EtudiantForm *)data;
    gtk_entry_set_text(GTK_ENTRY(form->entry_nom), "");
    gtk_entry_set_text(GTK_ENTRY(form->entry_prenom), "");
    gtk_entry_set_text(GTK_ENTRY(form->entry_matricule), "");
    gtk_entry_set_text(GTK_ENTRY(form->entry_telephone), "");
    gtk_combo_box_set_active(GTK_COMBO_BOX(form->combo_sexe), 0);
    gtk_entry_set_text(GTK_ENTRY(form->entry_email), "");
}

void enregistrer_etudiant_window(void (*retour_cb)(GtkWidget*, gpointer), gpointer data) {
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Enregistrer Étudiant");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 450);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 30);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Nom
    GtkWidget *entry_nom = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_nom), "Nom");
    gtk_box_pack_start(GTK_BOX(vbox), entry_nom, FALSE, FALSE, 0);

    // Prénom
    GtkWidget *entry_prenom = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_prenom), "Prénom");
    gtk_box_pack_start(GTK_BOX(vbox), entry_prenom, FALSE, FALSE, 0);

    // Matricule
    GtkWidget *entry_matricule = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_matricule), "Matricule");
    gtk_box_pack_start(GTK_BOX(vbox), entry_matricule, FALSE, FALSE, 0);

    // Téléphone
    GtkWidget *entry_telephone = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_telephone), "Téléphone");
    gtk_box_pack_start(GTK_BOX(vbox), entry_telephone, FALSE, FALSE, 0);

    // Sexe
    GtkWidget *combo_sexe = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_sexe), "Masculin");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_sexe), "Féminin");
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo_sexe), 0);
    gtk_box_pack_start(GTK_BOX(vbox), combo_sexe, FALSE, FALSE, 0);

    // Email
    GtkWidget *entry_email = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_email), "email");
    gtk_box_pack_start(GTK_BOX(vbox), entry_email, FALSE, FALSE, 0);

    // Espace flexible
    GtkWidget *spacer = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(vbox), spacer, TRUE, TRUE, 0);

    // HBox pour les boutons
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
    gtk_box_pack_end(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

    // Bouton Ajouter
    GtkWidget *btn_ajouter = gtk_button_new_with_label("Ajouter");
    gtk_widget_set_name(btn_ajouter, "btn_ajouter");
    gtk_box_pack_start(GTK_BOX(hbox), btn_ajouter, TRUE, TRUE, 0);

    // Bouton Annuler
    GtkWidget *btn_annuler = gtk_button_new_with_label("Annuler");
    gtk_widget_set_name(btn_annuler, "btn_annuler");
    gtk_box_pack_start(GTK_BOX(hbox), btn_annuler, TRUE, TRUE, 0);

    // CSS pour les boutons (optionnel)
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider,
        "#btn_ajouter {"
        "  background: #27ae60;"
        "  color: white;"
        "  font-weight: bold;"
        "  border-radius: 8px;"
        "  font-size: 15px;"
        "  transition: background 200ms;"
        "  padding: 10px 0;"
        "}"
        "#btn_ajouter:hover {"
        "  background: #219150;"
        "}"
        "#btn_annuler {"
        "  background: #e74c3c;"
        "  color: white;"
        "  font-weight: bold;"
        "  border-radius: 8px;"
        "  font-size: 15px;"
        "  transition: background 200ms;"
        "  padding: 10px 0;"
        "}"
        "#btn_annuler:hover {"
        "  background: #c0392b;"
        "}", -1, NULL);

    GtkStyleContext *context;
    context = gtk_widget_get_style_context(btn_ajouter);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    context = gtk_widget_get_style_context(btn_annuler);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Préparation de la structure pour les callbacks
    EtudiantForm *form = malloc(sizeof(EtudiantForm));
    form->window = window;
    form->entry_nom = entry_nom;
    form->entry_prenom = entry_prenom;
    form->entry_matricule = entry_matricule;
    form->entry_telephone = entry_telephone;
    form->combo_sexe = combo_sexe;
    form->entry_email = entry_email;

    g_signal_connect(btn_ajouter, "clicked", G_CALLBACK(on_ajouter_clicked), form);
    g_signal_connect(btn_annuler, "clicked", G_CALLBACK(on_annuler_clicked), form);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    free(form);
}