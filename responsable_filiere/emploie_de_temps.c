#include <gtk/gtk.h>
#include <cairo-pdf.h>
#include <libpq-fe.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Emploie_de_temps.h"


#define NB_JOURS 7
#define NB_CRENEAUX 4

const char* jours[] = {"Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi", "Dimanche"};
const char* creneaux[] = {
    "08h00-09h50", "10h10-12h00", "13h00-14h50", "15h10-17h00"
};

typedef struct {
    int id;
    char nom[128];
} ItemDB;

ItemDB *matieres = NULL, *enseignants = NULL, *classes = NULL;
int nb_matieres = 0, nb_enseignants = 0, nb_classes = 0;

typedef struct {
    int id_matiere;
    int id_enseignant;
    int id_classe;
} CelluleEDT;

CelluleEDT edt[NB_JOURS][NB_CRENEAUX];

// Charger une liste depuis la base
void charger_liste(const char *table, const char *id_col, ItemDB **tab, int *nb) {
    const char *conninfo = "host=localhost port=2006 dbname=Academie user=postgres password=LYRIAN22";
    PGconn *conn = PQconnectdb(conninfo);
    if (PQstatus(conn) != CONNECTION_OK) return;

    char query[128];
    snprintf(query, sizeof(query), "SELECT %s, nom FROM %s", id_col, table);
    PGresult *res = PQexec(conn, query);
    int rows = PQntuples(res);

    *tab = malloc(rows * sizeof(ItemDB));
    *nb = rows;
    for (int i = 0; i < rows; i++) {
        (*tab)[i].id = atoi(PQgetvalue(res, i, 0));
        strncpy((*tab)[i].nom, PQgetvalue(res, i, 1), 127);
        (*tab)[i].nom[127] = 0;
    }
    PQclear(res);
    PQfinish(conn);
}

// Callback pour modifier une cellule
static void on_cellule_clicked(GtkWidget *widget, gpointer user_data) {
    int* indices = (int*)user_data;
    int jour = indices[0];
    int creneau = indices[1];

    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Modifier le créneau",
        NULL,
        GTK_DIALOG_MODAL,
        "_Annuler", GTK_RESPONSE_CANCEL,
        "_Valider", GTK_RESPONSE_ACCEPT,
        NULL
    );
    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    // ComboBox Matière
    GtkWidget *combo_matiere = gtk_combo_box_text_new();
    for (int i = 0; i < nb_matieres; i++)
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_matiere), matieres[i].nom);
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo_matiere), 0);
    gtk_box_pack_start(GTK_BOX(content), combo_matiere, FALSE, FALSE, 5);

    // ComboBox Enseignant
    GtkWidget *combo_enseignant = gtk_combo_box_text_new();
    for (int i = 0; i < nb_enseignants; i++)
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_enseignant), enseignants[i].nom);
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo_enseignant), 0);
    gtk_box_pack_start(GTK_BOX(content), combo_enseignant, FALSE, FALSE, 5);

    // ComboBox Classe
    GtkWidget *combo_classe = gtk_combo_box_text_new();
    for (int i = 0; i < nb_classes; i++)
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_classe), classes[i].nom);
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo_classe), 0);
    gtk_box_pack_start(GTK_BOX(content), combo_classe, FALSE, FALSE, 5);

    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        int idx_m = gtk_combo_box_get_active(GTK_COMBO_BOX(combo_matiere));
        int idx_e = gtk_combo_box_get_active(GTK_COMBO_BOX(combo_enseignant));
        int idx_c = gtk_combo_box_get_active(GTK_COMBO_BOX(combo_classe));
        edt[jour][creneau].id_matiere = matieres[idx_m].id;
        edt[jour][creneau].id_enseignant = enseignants[idx_e].id;
        edt[jour][creneau].id_classe = classes[idx_c].id;

        // Mettre à jour le label du bouton
        char label[128];
        snprintf(label, sizeof(label), "%s\n%s\n%s",
            matieres[idx_m].nom,
            enseignants[idx_e].nom,
            classes[idx_c].nom
        );
        gtk_button_set_label(GTK_BUTTON(widget), label);

        // Enregistrer en base
        const char *conninfo = "host=localhost port=2006 dbname=Academie user=postgres password=LYRIAN22";
        PGconn *conn = PQconnectdb(conninfo);
        if (PQstatus(conn) == CONNECTION_OK) {
            char query[512];
            snprintf(query, sizeof(query),
                "INSERT INTO emploi_du_temps (jour, creneau, id_matiere, id_enseignant, id_classe) "
                "VALUES ('%s', '%s', %d, %d, %d) "
                "ON CONFLICT (jour, creneau) DO UPDATE SET id_matiere=%d, id_enseignant=%d, id_classe=%d;",
                jours[jour], creneaux[creneau],
                matieres[idx_m].id, enseignants[idx_e].id, classes[idx_c].id,
                matieres[idx_m].id, enseignants[idx_e].id, classes[idx_c].id
            );
            PQexec(conn, query);
            PQfinish(conn);
        }
    }
    gtk_widget_destroy(dialog);
    g_free(indices);
}

// Générer le PDF
static void on_generer_pdf(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_file_chooser_dialog_new(
        "Enregistrer le PDF",
        NULL,
        GTK_FILE_CHOOSER_ACTION_SAVE,
        "_Annuler", GTK_RESPONSE_CANCEL,
        "_Enregistrer", GTK_RESPONSE_ACCEPT,
        NULL
    );
    gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), "emploi_du_temps.pdf");
    gchar *filename = NULL;
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    gtk_widget_destroy(dialog);

    if (filename) {
        // Dimensions du tableau
        int cell_w = 110, cell_h = 60;
        int start_x = 100, start_y = 100;
        int table_w = (NB_JOURS + 1) * cell_w;
        int table_h = (NB_CRENEAUX + 1) * cell_h;

        cairo_surface_t *surface = cairo_pdf_surface_create(filename, table_w + 2*start_x, table_h + 2*start_y);
        cairo_t *cr = cairo_create(surface);

        // Titre
        cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
        cairo_set_font_size(cr, 22);
        cairo_move_to(cr, start_x, start_y - 40);
        cairo_show_text(cr, "Emploi du temps");

        // Dessiner le tableau
        cairo_set_line_width(cr, 1.2);
        // Lignes horizontales
        for (int i = 0; i <= NB_CRENEAUX + 1; i++) {
            cairo_move_to(cr, start_x, start_y + i*cell_h);
            cairo_line_to(cr, start_x + cell_w*(NB_JOURS+1), start_y + i*cell_h);
        }
        // Lignes verticales
        for (int j = 0; j <= NB_JOURS + 1; j++) {
            cairo_move_to(cr, start_x + j*cell_w, start_y);
            cairo_line_to(cr, start_x + j*cell_w, start_y + cell_h*(NB_CRENEAUX+1));
        }
        cairo_stroke(cr);

        // En-têtes jours
        cairo_set_font_size(cr, 13);
        for (int j = 0; j < NB_JOURS; j++) {
            cairo_move_to(cr, start_x + (j+1)*cell_w + 10, start_y + 0.7*cell_h);
            cairo_show_text(cr, jours[j]);
        }
        // En-têtes créneaux
        for (int i = 0; i < NB_CRENEAUX; i++) {
            cairo_move_to(cr, start_x + 10, start_y + (i+1)*cell_h + 0.7*cell_h);
            cairo_show_text(cr, creneaux[i]);
        }

        // Remplir les cellules
        cairo_set_font_size(cr, 11);
        for (int i = 0; i < NB_CRENEAUX; i++) {
            for (int j = 0; j < NB_JOURS; j++) {
                int idm = edt[j][i].id_matiere, ide = edt[j][i].id_enseignant, idc = edt[j][i].id_classe;
                const char *nom_m = "", *nom_e = "", *nom_c = "";
                for (int k = 0; k < nb_matieres; k++) if (matieres[k].id == idm) nom_m = matieres[k].nom;
                for (int k = 0; k < nb_enseignants; k++) if (enseignants[k].id == ide) nom_e = enseignants[k].nom;
                for (int k = 0; k < nb_classes; k++) if (classes[k].id == idc) nom_c = classes[k].nom;
                char cell[128] = "";
                if (idm > 0)
                    snprintf(cell, sizeof(cell), "%s\n%s\n%s", nom_m, nom_e, nom_c);

                // Affichage multi-ligne dans la cellule
                double x = start_x + (j+1)*cell_w + 5;
                double y = start_y + (i+1)*cell_h + 18;
                char *saveptr, *line = strtok_r(cell, "\n", &saveptr);
                while (line) {
                    cairo_move_to(cr, x, y);
                    cairo_show_text(cr, line);
                    y += 13;
                    line = strtok_r(NULL, "\n", &saveptr);
                }
            }
        }

        cairo_destroy(cr);
        cairo_surface_destroy(surface);

        GtkWidget *info = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "PDF généré !");
        gtk_dialog_run(GTK_DIALOG(info));
        gtk_widget_destroy(info);
        g_free(filename);
    }
}

// Fenêtre principale
void emploi_du_temps_window(void (*retour_cb)(GtkWidget*, gpointer), gpointer data) {
    // Charger les listes depuis la base
    charger_liste("matieres", "id_matieres", &matieres, &nb_matieres);
    charger_liste("enseignants", "id_enseignant", &enseignants, &nb_enseignants);
    charger_liste("classes", "id_classe", &classes, &nb_classes);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Emploi du temps");
    gtk_window_set_default_size(GTK_WINDOW(window), 1100, 600);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    GtkWidget *main_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), main_vbox);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 2);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 2);

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new(""), 0, 0, 1, 1);
    for (int j = 0; j < NB_JOURS; j++) {
        GtkWidget *lbl = gtk_label_new(jours[j]);
        gtk_grid_attach(GTK_GRID(grid), lbl, j+1, 0, 1, 1);
    }
    for (int i = 0; i < NB_CRENEAUX; i++) {
        GtkWidget *lbl = gtk_label_new(creneaux[i]);
        gtk_grid_attach(GTK_GRID(grid), lbl, 0, i+1, 1, 1);
        for (int j = 0; j < NB_JOURS; j++) {
            GtkWidget *btn = gtk_button_new_with_label("");
            int *indices = g_malloc(2 * sizeof(int));
            indices[0] = j; indices[1] = i;
            g_signal_connect(btn, "clicked", G_CALLBACK(on_cellule_clicked), indices);
            gtk_grid_attach(GTK_GRID(grid), btn, j+1, i+1, 1, 1);
        }
    }
    gtk_box_pack_start(GTK_BOX(main_vbox), grid, TRUE, TRUE, 0);

    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
    GtkWidget *btn_generer = gtk_button_new_with_label("Générer PDF");
    gtk_box_pack_start(GTK_BOX(hbox), btn_generer, TRUE, TRUE, 0);
    gtk_box_pack_end(GTK_BOX(main_vbox), hbox, FALSE, FALSE, 0);

    g_signal_connect(btn_generer, "clicked", G_CALLBACK(on_generer_pdf), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    free(matieres); free(enseignants); free(classes);
}