#include <gtk/gtk.h>
#include "direction.h"
// Callbacks pour les boutons
static void on_promouvoir(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Félicitations",
        GTK_WINDOW(data),
        GTK_DIALOG_MODAL,
        "_Fermer", GTK_RESPONSE_CLOSE,
        NULL
    );
    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *label = gtk_label_new("Félicitations à l'enseignant promu !");
    gtk_box_pack_start(GTK_BOX(content), label, TRUE, TRUE, 10);
    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

static void on_renvoyer_enseignant(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Motif de renvoi",
        GTK_WINDOW(data),
        GTK_DIALOG_MODAL,
        "_Fermer", GTK_RESPONSE_CLOSE,
        NULL
    );
    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *label = gtk_label_new("Veuillez saisir le motif du renvoi de l'enseignant :");
    GtkWidget *textview = gtk_text_view_new();
    gtk_box_pack_start(GTK_BOX(content), label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(content), textview, TRUE, TRUE, 5);
    gtk_widget_set_size_request(textview, 300, 80);
    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

static void on_renvoyer_etudiant(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Motif de renvoi",
        GTK_WINDOW(data),
        GTK_DIALOG_MODAL,
        "_Fermer", GTK_RESPONSE_CLOSE,
        NULL
    );
    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *label = gtk_label_new("Veuillez saisir le motif du renvoi de l'étudiant :");
    GtkWidget *textview = gtk_text_view_new();
    gtk_box_pack_start(GTK_BOX(content), label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(content), textview, TRUE, TRUE, 5);
    gtk_widget_set_size_request(textview, 300, 80);
    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void direction_window(void (*retour_cb)(GtkWidget*, gpointer), gpointer data) {
    

    // Fenêtre principale
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Direction");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 500);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    // Layout principal vertical
    GtkWidget *main_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 25);
    gtk_container_set_border_width(GTK_CONTAINER(main_vbox), 30);
    gtk_container_add(GTK_CONTAINER(window), main_vbox);

    // H1 centré
    GtkWidget *label_h1 = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label_h1), "<span font='28' weight='bold'>DIRECTION</span>");
    gtk_widget_set_halign(label_h1, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(main_vbox), label_h1, FALSE, FALSE, 0);

    // Panel enseignants
    GtkWidget *frame_ens = gtk_frame_new(NULL);
    gtk_box_pack_start(GTK_BOX(main_vbox), frame_ens, FALSE, FALSE, 0);
    GtkWidget *vbox_ens = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(vbox_ens), 15);
    gtk_container_add(GTK_CONTAINER(frame_ens), vbox_ens);

    GtkWidget *hbox_ens = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(vbox_ens), hbox_ens, FALSE, FALSE, 0);

    GtkWidget *label_ens = gtk_label_new("Enseignants :");
    gtk_box_pack_start(GTK_BOX(hbox_ens), label_ens, FALSE, FALSE, 0);

    GtkWidget *combo_ens = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_ens), "Enseignant 1");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_ens), "Enseignant 2");
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo_ens), -1);
    gtk_box_pack_start(GTK_BOX(hbox_ens), combo_ens, TRUE, TRUE, 0);

    GtkWidget *btn_promouvoir = gtk_button_new_with_label("Promouvoir");
    gtk_widget_set_name(btn_promouvoir, "btn_ajouter");
    g_signal_connect(btn_promouvoir, "clicked", G_CALLBACK(on_promouvoir), window);
    gtk_box_pack_start(GTK_BOX(hbox_ens), btn_promouvoir, FALSE, FALSE, 0);

    GtkWidget *btn_renvoyer_ens = gtk_button_new_with_label("Renvoyer");
    gtk_widget_set_name(btn_renvoyer_ens, "btn_supprimer");
    g_signal_connect(btn_renvoyer_ens, "clicked", G_CALLBACK(on_renvoyer_enseignant), window);
    gtk_box_pack_start(GTK_BOX(hbox_ens), btn_renvoyer_ens, FALSE, FALSE, 0);

    // Panel étudiants
    GtkWidget *frame_etud = gtk_frame_new(NULL);
    gtk_box_pack_start(GTK_BOX(main_vbox), frame_etud, FALSE, FALSE, 0);
    GtkWidget *vbox_etud = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(vbox_etud), 15);
    gtk_container_add(GTK_CONTAINER(frame_etud), vbox_etud);

    GtkWidget *hbox_etud = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(vbox_etud), hbox_etud, FALSE, FALSE, 0);

    GtkWidget *label_etud = gtk_label_new("Étudiants :");
    gtk_box_pack_start(GTK_BOX(hbox_etud), label_etud, FALSE, FALSE, 0);

    GtkWidget *combo_etud = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_etud), "Étudiant 1");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_etud), "Étudiant 2");
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo_etud), -1);
    gtk_box_pack_start(GTK_BOX(hbox_etud), combo_etud, TRUE, TRUE, 0);

    GtkWidget *btn_renvoyer_etud = gtk_button_new_with_label("Renvoyer");
    gtk_widget_set_name(btn_renvoyer_etud, "btn_supprimer");
    g_signal_connect(btn_renvoyer_etud, "clicked", G_CALLBACK(on_renvoyer_etudiant), window);
    gtk_box_pack_start(GTK_BOX(hbox_etud), btn_renvoyer_etud, FALSE, FALSE, 0);

    // CSS pour les boutons
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider,
        "#btn_ajouter {"
        "  background: #27ae60;"
        "  color: white;"
        "  font-weight: bold;"
        "  border-radius: 8px;"
        "  font-size: 14px;"
        "  transition: background 200ms;"
        "  padding: 8px 0;"
        "}"
        "#btn_ajouter:hover {"
        "  background: #219150;"
        "}"
        "#btn_supprimer {"
        "  background: #e74c3c;"
        "  color: white;"
        "  font-weight: bold;"
        "  border-radius: 8px;"
        "  font-size: 14px;"
        "  transition: background 200ms;"
        "  padding: 8px 0;"
        "}"
        "#btn_supprimer:hover {"
        "  background: #c0392b;"
        "}"
        , -1, NULL);

    GtkStyleContext *context;
    context = gtk_widget_get_style_context(btn_promouvoir);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    context = gtk_widget_get_style_context(btn_renvoyer_ens);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    context = gtk_widget_get_style_context(btn_renvoyer_etud);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return ;
}