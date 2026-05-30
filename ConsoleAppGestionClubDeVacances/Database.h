#pragma once // Alternative moderne à #ifndef, #define, #endif

#include <string>
#include <memory>
#include <vector>

// Déclaration anticipée pour éviter l'inclusion de sqlite3.h dans ce module 
// (et ainsi éviter les problèmes de dépendances circulaires)
struct sqlite3;

typedef struct ContinentData ContinentData;
struct ContinentData {
	std::string code;
	std::string libelle;
};

typedef struct PaysData PaysData;
struct PaysData {
	std::string code;
	std::string libelle;
	std::string codeContinent;
	std::string libelleContinent;
};

typedef struct VillageData VillageData;
struct VillageData {
	std::string code;
	std::string libelle;
	std::string codePays;
	std::string libellePays;
};

class Database
{
public:
	Database();
	~Database(); // obligatoire avec unique_ptr et déclration anticipée de sqlite3

	Database(const Database&) = delete; // Interdire la copie pour éviter de dupliquer la BDD accidentellement
	Database& operator=(const Database&) = delete; // Interdire l'affectation par copie pour éviter de dupliquer la BDD accidentellement

	Database(Database&&) noexcept; // Autoriser le déplacement
	Database& operator=(Database&&) noexcept; // Autoriser l'affectation par déplacement

	// API publiques propores
	bool open(const std::string& dbName);
	void close();
	bool executeQuery(const std::string& query);
	bool executePrepared(const std::string& sql, const std::vector<std::string>& params);
	bool valueExists(const std::string& tableName, const std::string& columnName, const std::string& searchParam);
	bool createTablesOnStart();

	std::vector<ContinentData> getAllContinents();
	std::vector<PaysData> getAllPays();
	std::vector<VillageData> getAllVillages();

private:
	// Un foncteur de destruction personnalisé pour fermer proprement la BDD
	struct sqlite3Deleter {
		void operator()(sqlite3* db) const;
	};

	// Utilisation d'un unique_ptr avec un foncteur de destruction personnalisé pour gérer la durée de vie de la BDD
	std::unique_ptr<sqlite3, sqlite3Deleter> m_db;
};