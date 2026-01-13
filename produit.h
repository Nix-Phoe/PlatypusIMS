#pragma once


#ifndef PRODUIT_H
#define PRODUIT_H

#include <libpq-fe.h>
#include <string>


struct Produit {
	int id;
	std::string nom;
	std::string categorie;
	int quantite;
	double prix;
	int seuil_alerte;
};

// ========== FONCTIONS CRUD ==========


bool ajouterProduit(PGconn* conn, const Produit& produit);


void afficherInventaire(PGconn* conn);


bool modifierQuantite(PGconn* conn, int id, int nouvelle_quantite);


bool supprimerProduit(PGconn* conn, int id);

// ========== FONCTIONS UTILITAIRES ==========


void verifierStocksBas(PGconn* conn);


bool produitExiste(PGconn* conn, int id);

#endif //PRODUIT_H