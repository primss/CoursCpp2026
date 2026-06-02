#pragma once
#include <vector>
#include <string>
#include <optional>

class Database; // Déclaration anticipée

template <typename T>
class Repository {
public:
	Repository() = default;
    virtual ~Repository() = default;

    // Les 4 opérations CRUD de base
    virtual T create(Database& db, const T& entity) = 0;
    virtual T read(Database& db, const std::string& code) = 0;
    virtual bool update(Database& db, const std::string& oldCode, const T& entity) = 0;
    virtual bool destroy(Database& db, const std::string& code) = 0;

    // Récupérer tout le monde
    virtual std::vector<T> getAll(Database& db) = 0;

    // Méthode utilitaire pour vérifier l'existence d'une valeur dans une colonne spécifique
	virtual bool valueExists(Database& db, const std::string& columnName, const std::string& searchParam) = 0;
};