#include <gtk/gtk.h>
#include "gestiondepartement.h"
#include <libpq-fe.h>
// Callbacks pour les boutons (à compléter selon la logique)
static void on_ajouter_cours1(GtkWidget *widget, gpointer data) { g_print("Ajouter cours semestre 1\n"); }
static void on_ajouter_cours2(GtkWidget *widget, gpointer data) { g_print("Ajouter cours semestre 2\n"); }
static void on_supprimer_cours(GtkWidget *widget, gpointer data) { g_print("Supprimer cours\n"); }
static void on_ajouter_classe(GtkWidget *widget, gpointer data) { g_print("Ajouter classe\n"); }
static void on_modifier_capacite(GtkWidget *widget, gpointer data) { g_print("Modifier capacité\n"); }
static void on_ajouter_filiere(GtkWidget *widget, gpointer data) { g_print("Ajouter filière\n"); }
static void on_supprimer_filiere(GtkWidget *widget, gpointer data) { g_print("Supprimer filière\n"); }

typedef struct {
    char **capacites;
    int n;
    GtkWidget *combo_capacite;
} ClasseData;


void gestion_departement_window(void (*retour_cb)(GtkWidget*, gpointer), gpointer data) {
   

    // Fenêtre principale
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Gestion Département");
    gtk_window_set_default_size(GTK_WINDOW(window), 1200, 400);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    // Layout principal horizontal (3 panels)
    GtkWidget *main_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(main_hbox), 20);
    gtk_container_add(GTK_CONTAINER(window), main_hbox);

    // Panel 1 : Matière
    GtkWidget *panel1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_hexpand(panel1, TRUE);
    gtk_box_pack_start(GTK_BOX(main_hbox), panel1, TRUE, TRUE, 0);

    GtkWidget *label_matiere = gtk_label_new("Matière");
    gtk_widget_set_name(label_matiere, "lbl_panel");
    gtk_box_pack_start(GTK_BOX(panel1), label_matiere, FALSE, FALSE, 0);

    GtkWidget *combo_sem1 = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_sem1), "Mathématiques");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_sem1), "Physique");
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo_sem1), -1);
    gtk_box_pack_start(GTK_BOX(panel1), combo_sem1, FALSE, FALSE, 0);

    GtkWidget *combo_sem2 = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_sem2), "Informatique");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_sem2), "Chimie");
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo_sem2), -1);
    gtk_box_pack_start(GTK_BOX(panel1), combo_sem2, FALSE, FALSE, 0);

    GtkWidget *btn_ajouter_sem1 = gtk_button_new_with_label("Ajouter cours semestre 1");
    gtk_widget_set_name(btn_ajouter_sem1, "btn_ajouter");
    g_signal_connect(btn_ajouter_sem1, "clicked", G_CALLBACK(on_ajouter_cours1), NULL);
    gtk_box_pack_start(GTK_BOX(panel1), btn_ajouter_sem1, FALSE, FALSE, 0);

    GtkWidget *btn_ajouter_sem2 = gtk_button_new_with_label("Ajouter cours semestre 2");
    gtk_widget_set_name(btn_ajouter_sem2, "btn_ajouter");
    g_signal_connect(btn_ajouter_sem2, "clicked", G_CALLBACK(on_ajouter_cours2), NULL);
    gtk_box_pack_start(GTK_BOX(panel1), btn_ajouter_sem2, FALSE, FALSE, 0);

    GtkWidget *btn_supprimer_cours = gtk_button_new_with_label("Supprimer");
    gtk_widget_set_name(btn_supprimer_cours, "btn_supprimer");
    g_signal_connect(btn_supprimer_cours, "clicked", G_CALLBACK(on_supprimer_cours), NULL);
    gtk_box_pack_start(GTK_BOX(panel1), btn_supprimer_cours, FALSE, FALSE, 0);

    // Panel 2 : Classe
 // Panel 2 : Classe
GtkWidget *panel2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
gtk_widget_set_hexpand(panel2, TRUE);
gtk_box_pack_start(GTK_BOX(main_hbox), panel2, TRUE, TRUE, 0);

GtkWidget *label_classe = gtk_label_new("Classe");
gtk_widget_set_name(label_classe, "lbl_panel");
gtk_box_pack_start(GTK_BOX(panel2), label_classe, FALSE, FALSE, 0);

GtkWidget *combo_nom_classe = gtk_combo_box_text_new();
GtkWidget *combo_capacite = gtk_combo_box_text_new();
gtk_box_pack_start(GTK_BOX(panel2), combo_nom_classe, FALSE, FALSE, 0);
gtk_box_pack_start(GTK_BOX(panel2), combo_capacite, FALSE, FALSE, 0);

// --- Récupération des classes et capacités ---
const char *conninfo = "host=localhost port=2006 dbname=Academie user=postgres password=LYRIAN22";
PGconn *conn = PQconnectdb(conninfo);

char **capacites = NULL;
int n = 0;

if (PQstatus(conn) == CONNECTION_OK) {
    PGresult *res = PQexec(conn, "SELECT nom_classe, capacite FROM classe");
    if (PQresultStatus(res) == PGRES_TUPLES_OK) {
        n = PQntuples(res);
        capacites = malloc(n * sizeof(char*));
        for (int i = 0; i < n; i++) {
            const char *nom = PQgetvalue(res, i, 0);
            const char *cap = PQgetvalue(res, i, 1);
            gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_nom_classe), nom);
            capacites[i] = g_strdup(cap);
        }
    }
    PQclear(res);
}
PQfinish(conn);

ClasseData *classe_data = malloc(sizeof(ClasseData));
classe_data->capacites = capacites;
classe_data->n = n;
classe_data->combo_capacite = combo_capacite;

// --- Callback pour synchroniser capacité ---
void on_classe_changed(GtkComboBox *combo, gpointer user_data) {
    ClasseData *data = (ClasseData *)user_data;
    GtkComboBoxText *combo_capacite = GTK_COMBO_BOX_TEXT(data->combo_capacite);
    int index = gtk_combo_box_get_active(combo);
    gtk_combo_box_text_remove_all(combo_capacite);
    if (index >= 0 && data->capacites) {
        gtk_combo_box_text_append_text(combo_capacite, data->capacites[index]);
        gtk_combo_box_set_active(GTK_COMBO_BOX(combo_capacite), 0);
    }
}
g_signal_connect(combo_nom_classe, "changed", G_CALLBACK(on_classe_changed), classe_data);

// Libération à la fermeture de la fenêtre
void free_classe_data(GtkWidget *widget, gpointer user_data) {
    ClasseData *data = (ClasseData *)user_data;
    if (data->capacites) {
        for (int i = 0; i < data->n; i++) g_free(data->capacites[i]);
        free(data->capacites);
    }
    free(data);
}

g_signal_connect(window, "destroy", G_CALLBACK(free_classe_data), classe_data);

    // Panel 3 : Filière
    GtkWidget *panel3 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_hexpand(panel3, TRUE);
    gtk_box_pack_start(GTK_BOX(main_hbox), panel3, TRUE, TRUE, 0);

    GtkWidget *label_filiere = gtk_label_new("Filière");
    gtk_widget_set_name(label_filiere, "lbl_panel");
    gtk_box_pack_start(GTK_BOX(panel3), label_filiere, FALSE, FALSE, 0);

    GtkWidget *combo_filiere = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_filiere), "Informatique");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_filiere), "Génie Civil");
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo_filiere), -1);
    gtk_box_pack_start(GTK_BOX(panel3), combo_filiere, FALSE, FALSE, 0);

    GtkWidget *btn_ajouter_filiere = gtk_button_new_with_label("Ajouter");
    gtk_widget_set_name(btn_ajouter_filiere, "btn_ajouter");
    g_signal_connect(btn_ajouter_filiere, "clicked", G_CALLBACK(on_ajouter_filiere), NULL);
    gtk_box_pack_start(GTK_BOX(panel3), btn_ajouter_filiere, FALSE, FALSE, 0);

    GtkWidget *btn_supprimer_filiere = gtk_button_new_with_label("Supprimer");
    gtk_widget_set_name(btn_supprimer_filiere, "btn_supprimer");
    g_signal_connect(btn_supprimer_filiere, "clicked", G_CALLBACK(on_supprimer_filiere), NULL);
    gtk_box_pack_start(GTK_BOX(panel3), btn_supprimer_filiere, FALSE, FALSE, 0);

    // CSS
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
        "#btn_modifier {"
        "  background: #f1c40f;"
        "  color: white;"
        "  font-weight: bold;"
        "  border-radius: 8px;"
        "  font-size: 14px;"
        "  transition: background 200ms;"
        "  padding: 8px 0;"
        "}"
        "#btn_modifier:hover {"
        "  background: #b7950b;"
        "}"
        "#lbl_panel {"
        "  font-size: 18px;"
        "  color: #00b8b8;"
        "  font-weight: bold;"
        "  margin-bottom: 10px;"
        "}"
        , -1, NULL);

    // Appliquer le CSS aux boutons et labels
    GtkStyleContext *context;
    GtkWidget *widgets[] = {
        btn_ajouter_sem1, btn_ajouter_sem2, btn_supprimer_cours,
        btn_ajouter_filiere, btn_supprimer_filiere,
        label_matiere, label_classe, label_filiere
    };
    for (int i = 0; i < 10; i++) {
        context = gtk_widget_get_style_context(widgets[i]);
        gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    }

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return ;
}