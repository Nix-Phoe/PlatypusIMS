#include "produit.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

// ========== CREATE - AJOUTER UN PRODUIT ==========
bool ajouterProduit(PGconn* conn, const Produit& produit) {
   
    if (produit.quantite < 0) {
        cerr << " La quantite ne peut pas etre negative." << endl;
        return false;
    }

    if (produit.prix < 0) {
        cerr << " Le prix ne peut pas etre negatif." << endl;
        return false;
    }

    
    stringstream query;
    query << "INSERT INTO produit (nom, categorie, quantite, prix, seuil_alerte) VALUES ('"
        << produit.nom << "', '"
        << produit.categorie << "', "
        << produit.quantite << ", "
        << produit.prix << ", "
        << produit.seuil_alerte << ");";

    
    PGresult* res = PQexec(conn, query.str().c_str());

   
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cerr << " Erreur lors de l'ajout : " << PQerrorMessage(conn) << endl;
        PQclear(res);
        return false;
    }

    cout << " Produit '" << produit.nom << "' ajoute avec succes !" << endl;
    PQclear(res);
    return true;
}

// ========== READ - AFFICHER L'INVENTAIRE ==========
void afficherInventaire(PGconn* conn) {
    const char* query = "SELECT id, nom, categorie, quantite, prix, seuil_alerte FROM produit ORDER BY id;";

    PGresult* res = PQexec(conn, query);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        cerr << " Erreur lors de la récupération : " << PQerrorMessage(conn) << endl;
        PQclear(res);
        return;
    }

    int nbProduits = PQntuples(res);

    if (nbProduits == 0) {
        cout << "\n Aucun produit dans l'inventaire." << endl;
        PQclear(res);
        return;
    }

    
    cout << "                          INVENTAIRE COMPLET                          " << endl;
   

    cout << left
        << setw(6) << "ID"
        << setw(25) << "Nom"
        << setw(15) << "Categorie"
        << setw(10) << "Quantite"
        << setw(12) << "Prix ($)"
        << setw(10) << "Seuil" << endl;
    cout << string(78, '-') << endl;

    for (int i = 0; i < nbProduits; i++) {
        int quantite = atoi(PQgetvalue(res, i, 3));
        int seuil = atoi(PQgetvalue(res, i, 5));

        cout << left
            << setw(6) << PQgetvalue(res, i, 0)   
            << setw(25) << PQgetvalue(res, i, 1)  
            << setw(15) << PQgetvalue(res, i, 2)  
            << setw(10) << PQgetvalue(res, i, 3); 

        
        cout << setw(12) << fixed << setprecision(2) << atof(PQgetvalue(res, i, 4));

        
        if (quantite <= seuil) {
            cout << setw(10) << PQgetvalue(res, i, 5) << " !!!!!!!";
        }
        else {
            cout << setw(10) << PQgetvalue(res, i, 5);
        }

        cout << endl;
    }

    cout << string(78, '=') << endl;
    cout << " Total : " << nbProduits << " produit(s)" << endl << endl;

    PQclear(res);
}

// ========== UPDATE - MODIFIER LA QUANTITÉ ==========
bool modifierQuantite(PGconn* conn, int id, int nouvelle_quantite) {
    
    if (!produitExiste(conn, id)) {
        cerr << " Produit avec ID " << id << " introuvable." << endl;
        return false;
    }

    
    if (nouvelle_quantite < 0) {
        cerr << " La quantite ne peut pas etre negative." << endl;
        return false;
    }

   
    stringstream query;
    query << "UPDATE produit SET quantite = " << nouvelle_quantite
        << " WHERE id = " << id << ";";

    PGresult* res = PQexec(conn, query.str().c_str());

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cerr << " Erreur lors de la modification : " << PQerrorMessage(conn) << endl;
        PQclear(res);
        return false;
    }

    cout << " Quantite du produit ID " << id << " modifiee a " << nouvelle_quantite << endl;
    PQclear(res);
    return true;
}

// ========== DELETE - SUPPRIMER UN PRODUIT ==========
bool supprimerProduit(PGconn* conn, int id) {
    
    if (!produitExiste(conn, id)) {
        cerr << " Produit avec ID " << id << " introuvable." << endl;
        return false;
    }

   
    stringstream query;
    query << "DELETE FROM produit WHERE id = " << id << ";";

    PGresult* res = PQexec(conn, query.str().c_str());

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cerr << " Erreur lors de la suppression : " << PQerrorMessage(conn) << endl;
        PQclear(res);
        return false;
    }

    cout << " Produit ID " << id << " supprime avec succes." << endl;
    PQclear(res);
    return true;
}

// ========== VÉRIFIER LES STOCKS BAS ==========
void verifierStocksBas(PGconn* conn) {
    const char* query = "SELECT id, nom, quantite, seuil_alerte FROM produit WHERE quantite <= seuil_alerte ORDER BY quantite;";

    PGresult* res = PQexec(conn, query);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        cerr << " Erreur lors de la verification : " << PQerrorMessage(conn) << endl;
        PQclear(res);
        return;
    }

    int nbAlertes = PQntuples(res);

    if (nbAlertes == 0) {
        cout << "\n Tous les stocks sont au-dessus du seuil d'alerte." << endl << endl;
        PQclear(res);
        return;
    }

  
    cout << "             !!!  ALERTES : STOCKS BAS DETECTES   !!!             " << endl;
  

    cout << left
        << setw(6) << "ID"
        << setw(35) << "Nom"
        << setw(15) << "Quantite"
        << setw(15) << "Seuil" << endl;
    cout << string(71, '-') << endl;

    for (int i = 0; i < nbAlertes; i++) {
        cout << left
            << setw(6) << PQgetvalue(res, i, 0)
            << setw(35) << PQgetvalue(res, i, 1)
            << setw(15) << PQgetvalue(res, i, 2)
            << setw(15) << PQgetvalue(res, i, 3)
            << " 0" << endl;
    }

    cout << string(71, '=') << endl;
    cout << "(0-0) " << nbAlertes << " produit(s) necessitent un reapprovisionnement immediat !" << endl << endl;

    PQclear(res);
}

// ========== VÉRIFIER SI UN PRODUIT EXISTE ==========
bool produitExiste(PGconn* conn, int id) {
    stringstream query;
    query << "SELECT id FROM produit WHERE id = " << id << ";";

    PGresult* res = PQexec(conn, query.str().c_str());

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        PQclear(res);
        return false;
    }

    bool existe = PQntuples(res) > 0;
    PQclear(res);

    return existe;
}