#include <stdio.h>
#include <libpq-fe.h>

int main() {
    // Chaîne de connexion : adapte les paramètres à ta base
    const char *conninfo = "host=localhost port=2006 dbname=Academie user=postgres password=";

    // Connexion à la base
    PGconn *conn = PQconnectdb(conninfo);

    // Vérification de la connexion
    if (PQstatus(conn) == CONNECTION_OK) {
        printf("Connexion à la base de données réussie !\n");
    } else {
        printf("Erreur de connexion : %s\n", PQerrorMessage(conn));
    }

    // Fermeture de la connexion
    PQfinish(conn);
    return 0;
}