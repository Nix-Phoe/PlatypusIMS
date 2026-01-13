DROP TABLE produit ;
CREATE TABLE produit (
    id SERIAL PRIMARY KEY,
    nom VARCHAR(100) NOT NULL,
    categorie VARCHAR(50),
    quantite INTEGER NOT NULL DEFAULT 0,
    prix DECIMAL(10, 2),
    seuil_alerte INTEGER DEFAULT 10,
    date_ajout TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);