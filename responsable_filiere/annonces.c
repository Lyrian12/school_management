#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <cairo-pdf.h>
#include "annonces.h"

// Callback pour le bouton PDF
static void on_generer_pdf(GtkWidget *widget, gpointer data) {
    GtkWidget **widgets = (GtkWidget **)data;
    GtkEntry *entry_titre = GTK_ENTRY(widgets[0]);
    GtkTextView *textview = GTK_TEXT_VIEW(widgets[1]);

    // Récupérer le titre
    const char *titre = gtk_entry_get_text(entry_titre);

    // Récupérer le contenu de l'annonce
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(textview);
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    gchar *contenu = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

    // Boîte de dialogue pour choisir l'emplacement du PDF
    GtkWidget *dialog = gtk_file_chooser_dialog_new(
        "Enregistrer le PDF",
        NULL,
        GTK_FILE_CHOOSER_ACTION_SAVE,
        "_Annuler", GTK_RESPONSE_CANCEL,
        "_Enregistrer", GTK_RESPONSE_ACCEPT,
        NULL
    );
    gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), "annonce.pdf");

    gchar *filename = NULL;
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    }
    gtk_widget_destroy(dialog);

    if (filename) {
        // Création du PDF avec Cairo
        cairo_surface_t *surface = cairo_pdf_surface_create(filename, 595, 842); // A4
        cairo_t *cr = cairo_create(surface);

        // Titre
        cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
        cairo_set_font_size(cr, 24);
        cairo_move_to(cr, 50, 80);
        cairo_show_text(cr, titre);

        // Contenu
        cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
        cairo_set_font_size(cr, 14);
        double y = 120;

        // Découper le contenu en lignes pour l'affichage
        char *contenu_copy = g_strdup(contenu);
        char *line = strtok(contenu_copy, "\n");
        while (line) {
            cairo_move_to(cr, 50, y);
            cairo_show_text(cr, line);
            y += 22;
            line = strtok(NULL, "\n");
        }
        g_free(contenu_copy);

        cairo_destroy(cr);
        cairo_surface_destroy(surface);

        GtkWidget *info = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "PDF enregistré !");
        gtk_dialog_run(GTK_DIALOG(info));
        gtk_widget_destroy(info);

        g_free(filename);
    }
    g_free(contenu);
}

// Fonction pour dessiner le filigrane dans la zone de texte
static gboolean on_draw_textview(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widget));
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    gchar *text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

    // Si la zone est vide, dessiner le filigrane
    if (g_strcmp0(text, "") == 0) {
        GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_scale("filigrane.png", 100, 100, TRUE, NULL);
        if (pixbuf) {
            gdk_cairo_set_source_pixbuf(cr, pixbuf, 50, 50);
            cairo_paint_with_alpha(cr, 0.15); // transparence
            g_object_unref(pixbuf);
        }
    }
    g_free(text);
    return FALSE;
}

void annonces_window(void (*retour_cb)(GtkWidget*, gpointer), gpointer data){
    // Fenêtre principale
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Annonces");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 500);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    // Layout principal vertical
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 20);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Zone pour le titre
    GtkWidget *hbox_titre = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_box_pack_start(GTK_BOX(vbox), hbox_titre, FALSE, FALSE, 0);

    GtkWidget *label_titre = gtk_label_new("Titre :");
    gtk_box_pack_start(GTK_BOX(hbox_titre), label_titre, FALSE, FALSE, 0);

    GtkWidget *entry_titre = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_titre), "Titre de l'annonce");
    gtk_box_pack_start(GTK_BOX(hbox_titre), entry_titre, TRUE, TRUE, 0);

    // Grande zone de texte avec filigrane
    GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_vexpand(scrolled, TRUE);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled, TRUE, TRUE, 0);

    GtkWidget *textview = gtk_text_view_new();
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(textview), GTK_WRAP_WORD_CHAR);
    gtk_container_add(GTK_CONTAINER(scrolled), textview);

    // Dessiner le filigrane quand la zone est vide
    g_signal_connect(textview, "draw", G_CALLBACK(on_draw_textview), NULL);

    // Préparer les widgets à passer au callback PDF
    GtkWidget **widgets = g_malloc(2 * sizeof(GtkWidget*));
    widgets[0] = entry_titre;
    widgets[1] = textview;

    // Bouton Générer PDF
    GtkWidget *btn_generer = gtk_button_new_with_label("Générer PDF");
    gtk_widget_set_name(btn_generer, "btn_generer");
    g_signal_connect(btn_generer, "clicked", G_CALLBACK(on_generer_pdf), widgets);
    gtk_box_pack_end(GTK_BOX(vbox), btn_generer, FALSE, FALSE, 0);

    // CSS pour le bouton
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider,
        "#btn_generer {"
        "  background: #27ae60;"
        "  color: white;"
        "  font-weight: bold;"
        "  border-radius: 8px;"
        "  font-size: 15px;"
        "  transition: background 200ms;"
        "  padding: 10px 0;"
        "}"
        "#btn_generer:hover {"
        "  background: #219150;"
        "}", -1, NULL);

    GtkStyleContext *context = gtk_widget_get_style_context(btn_generer);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    g_free(widgets);
}