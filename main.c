#include <gtk/gtk.h>
#include <libpq-fe.h>
#include "connexion.h"
#include "accueil.h"
#include "responsable_filiere.h"
#include "departement_3iac.h"
#include "departement3iac/direction.h"
#include "departement3iac/gestiondepartement.h"
#include "responsable_filiere/annonces.h"
#include "responsable_filiere/emploie_de_temps.h"
#include "responsable_filiere/enregistrer_enseignant.h"
#include "responsable_filiere/enregistrer_etudiant.h"
// Déclarations des fonctions pour chaque fenêtre
void show_connexion();
void show_accueil();
void show_responsable_filiere();
void show_enregistrer_etudiant(GtkWidget *parent);
void show_enregistrer_enseignant(GtkWidget *parent);
void show_annonces(GtkWidget *parent);
void show_emploi_du_temps(GtkWidget *parent);
void show_departement_3iac();
void show_gestion_departement(GtkWidget *parent);
void show_direction(GtkWidget *parent);

// --- Bouton retour générique ---
GtkWidget* create_btn_retour(void (*callback)(GtkWidget*, gpointer), gpointer data) {
    GtkWidget *btn = gtk_button_new_with_label("Retour");
    gtk_widget_set_name(btn, "btn_retour");
    g_signal_connect(btn, "clicked", G_CALLBACK(callback), data);
    return btn;
}

// void connect_db() {
//     const char *conninfo = "host=localhost port=2006 dbname=Academie user=postgres password=LYRIAN22";
//     PGconn *conn = PQconnectdb(conninfo);

//     if (PQstatus(conn) != CONNECTION_OK) {
//         fprintf(stderr, "Erreur de connexion : %s", PQerrorMessage(conn));
//         PQfinish(conn);
//         exit(1);
//     }

//     printf("Connexion réussie à PostgreSQL !\n");

//     // Fermer la connexion
//     PQfinish(conn);
// }

// --- MAIN ---
int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    //connect_db(); // Connexion à la base de données
    show_connexion();

    gtk_main();
    return 0;
}