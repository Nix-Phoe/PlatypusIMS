#include <iostream>
#include <limits>
#include "database.h"
#include "produit.h"

using namespace std;

void afficherMenu() {
   
    cout << "--------------------------------------------------" << endl;
    cout << "|       PLATYPUS IMS - GESTION STOCK             |" << endl;
    cout << "--------------------------------------------------" << endl;
    cout << "| 1.  Ajouter un produit                         |" << endl;
    cout << "| 2.  Afficher l'inventaire complet              |" << endl;
    cout << "| 3.  Modifier la quantite d'un produit          |" << endl;
    cout << "| 4.  Supprimer un produit                       |" << endl;
    cout << "| 5.  Verifier les stocks bas                    |" << endl;
    cout << "| 0.  Quitter                                    |" << endl;
    cout << "--------------------------------------------------" << endl;
    cout << "Votre choix : ";
}

void viderBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main() {
    // ========== CONNEXION À LA BASE DE DONNÉES ==========
    cout << " Connexion a PostgreSQL..." << endl;
    
    string conninfo = "host=localhost dbname=platypusIMS_Database user=platypus_user password=nix";
    
    Database db(conninfo);
    
    if (!db.isConnected()) {
        cerr << "Impossible de se connecter a la base de données !" << endl;
        cerr << "Erreur : " << db.getErrorMessage() << endl;
        return 1;
    }
    
    cout << " Connexion reussie a la base de donnees !\n" << endl;
    
    PGconn* conn = db.getConnection();
    int choix;
    
    // ========== BOUCLE DU MENU ==========
    do {
        afficherMenu();
        
     
        if (!(cin >> choix)) {
            cout << " Entree invalide. Veuillez entrer un nombre." << endl;
            viderBuffer();
            continue;
        }
        
        viderBuffer();
        
        switch (choix) {
            // CREATE - Ajouter un produit
            case 1: { 
                
                cout << "      AJOUTER UN NOUVEAU PRODUIT  " << endl;
                
                
                Produit p;
                
                cout << " Nom du produit : ";
                getline(cin, p.nom);
                
                if (p.nom.empty()) {
                    cout << " Le nom ne peut pas etre vide." << endl;
                    break;
                }
                
                cout << "  Categorie : ";
                getline(cin, p.categorie);
                
                cout << " Quantité initiale : ";
                while (!(cin >> p.quantite) || p.quantite < 0) {
                    cout << " Veuillez entrer une quantité valide (≥ 0) : ";
                    viderBuffer();
                }
                
                cout << " Prix unitaire : ";
                while (!(cin >> p.prix) || p.prix < 0) {
                    cout << " Veuillez entrer un prix valide (≥ 0) : ";
                    viderBuffer();
                }
                
                cout << " Seuil d'alerte : ";
                while (!(cin >> p.seuil_alerte) || p.seuil_alerte < 0) {
                    cout << "Veuillez entrer un seuil valide (≥ 0) : ";
                    viderBuffer();
                }
                
                ajouterProduit(conn, p);
                break;
            }
            
            case 2: { // READ - Afficher l'inventaire
                afficherInventaire(conn);
                break;
            }
            
            case 3: { // UPDATE - Modifier la quantité
                
                cout << "     MODIFIER LA QUANTITE       " << endl;
               
                
                int id, quantite;
                
                cout << " ID du produit : ";
                while (!(cin >> id)) {
                    cout << " Veuillez entrer un ID valide : ";
                    viderBuffer();
                }
                
                cout << " Nouvelle quantité : ";
                while (!(cin >> quantite) || quantite < 0) {
                    cout << " Veuillez entrer une quantite valide (≥ 0) : ";
                    viderBuffer();
                }
                
                modifierQuantite(conn, id, quantite);
                break;
            }
            
            case 4: { // DELETE - Supprimer un produit
               
                cout << "      SUPPRIMER UN PRODUIT      " << endl;
                
                
                int id;
                
                cout << " ID du produit a supprimer : ";
                while (!(cin >> id)) {
                    cout << " Veuillez entrer un ID valide : ";
                    viderBuffer();
                }
                
                viderBuffer();
                
                char confirmation;
                cout << "  Etes-vous sur de vouloir supprimer ce produit ? (o/n) : ";
                cin >> confirmation;
                
                if (confirmation == 'o' || confirmation == 'O') {
                    supprimerProduit(conn, id);
                } else {
                    cout << " Suppression annulee." << endl;
                }
                break;
            }
            
            case 5: { // Vérifier les stocks bas
                verifierStocksBas(conn);
                break;
            }
            
            case 0: { // Quitter
                cout << "\nMerci d'avoir utilise Platypus IMS !" << endl;
                cout << " A bientot !\n" << endl;
                break;
            }
            
            default:
                cout << " Choix invalide. Veuillez choisir entre 0 et 5." << endl;
        }
        
    } while (choix != 0);
    
    return 0;
}