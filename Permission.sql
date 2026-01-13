-- Donner les droits
GRANT ALL PRIVILEGES ON DATABASE "platypusIMS_Database" TO platypus_user;
-- Donner tous les droits sur la table produit
GRANT ALL PRIVILEGES ON TABLE produit TO platypus_user;

-- Donner les droits sur la séquence (pour l'auto-incrémentation de l'ID)
GRANT USAGE, SELECT ON SEQUENCE produit_id_seq TO platypus_user;

-- Donner les droits sur le schéma public (pour éviter d'autres erreurs)
GRANT ALL PRIVILEGES ON SCHEMA public TO platypus_user;

-- Donner les droits sur toutes les futures tables (optionnel mais utile)
ALTER DEFAULT PRIVILEGES IN SCHEMA public GRANT ALL ON TABLES TO platypus_user;
ALTER DEFAULT PRIVILEGES IN SCHEMA public GRANT ALL ON SEQUENCES TO platypus_user;