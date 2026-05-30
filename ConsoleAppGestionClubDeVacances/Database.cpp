#include "Database.h"
#include "sqlite3.h"

#include <iostream>
#include <vector>

Database::Database() = default;
Database::~Database() = default;
Database::Database(Database&&) noexcept = default;
Database& Database::operator=(Database&&) noexcept = default;

// Implementation du foncteur de destruction personnalisé pour fermer proprement la BDD
void Database::sqlite3Deleter::operator()(sqlite3* db) const
{
	if (db) {
		sqlite3_close(db);
		std::cout << "Base de données fermée." << std::endl;
	}
}

bool Database::open(const std::string& dbName)
{
	sqlite3* rawDbPtr = nullptr;
	if (sqlite3_open(dbName.c_str(), &rawDbPtr) != SQLITE_OK) {
		std::cerr << "Erreur lors de l'ouverture de la base de données : " << sqlite3_errmsg(rawDbPtr) << std::endl;
		return false;
	}
	m_db.reset(rawDbPtr); // Transfert de la propriété du pointeur brut vers le unique_ptr
	std::cout << "Base de données ouverte avec succès." << std::endl;
	return true;
}

void Database::close()
{
	m_db.reset(); // Libère la base de données (le foncteur de destruction personnalisé s'occupera de fermer la BDD)
}

bool Database::executeQuery(const std::string& query)
{
	char* errMsg = nullptr;
	// m_db.get() permet d'obtenir le pointeur brut sqlite3* à partir du unique_ptr
	if (sqlite3_exec(m_db.get(), query.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
		std::cerr << "Erreur lors de l'exécution de la requête : " << errMsg << std::endl;
		sqlite3_free(errMsg);
		return false;
	}
	return true;
}

bool Database::executePrepared(const std::string& sql, const std::vector<std::string>& params)
{
	// On vérifie que la BDD est bien ouverte
	if (m_db == nullptr) {
		std::cerr << "Erreur : La connexion à la base de données n'est pas ouverte !" << std::endl;
		return false;
	}

	sqlite3_stmt* stmt = nullptr;
	if (sqlite3_prepare_v2(m_db.get(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
		std::cerr << "Erreur de préparation : " << sqlite3_errmsg(m_db.get()) << std::endl;
		return false;
	}
	// Liaison des paramètres (les index commencent à 1)
	for (size_t i = 0; i < params.size(); ++i) {
		sqlite3_bind_text(stmt, static_cast<int>(i + 1), params[i].c_str(), -1, SQLITE_TRANSIENT);
	}
	bool success = true;
	if (sqlite3_step(stmt) != SQLITE_DONE) {
		std::cerr << "Erreur d'exécution : " << sqlite3_errmsg(m_db.get()) << std::endl;
		success = false;
	}
	sqlite3_finalize(stmt); // Libération du statement
	return success;
}

bool Database::valueExists(const std::string& tableName, const std::string& columnName, const std::string& searchParam)
{
	// On vérifie que la BDD est bien ouverte
	if (m_db == nullptr) {
		std::cerr << "Erreur : La connexion à la base de données n'est pas ouverte !" << std::endl;
		return false;
	}

	std::string query_str = "SELECT 1 FROM " + tableName + " WHERE " + columnName + " = ?;";
	sqlite3_stmt* stmt = nullptr;
	bool hasFoundValue = false;

	if (sqlite3_prepare_v2(m_db.get(), query_str.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {

		sqlite3_bind_text(stmt, 1, searchParam.c_str(), -1, SQLITE_TRANSIENT);

		// Si sqlite3_step renvoie SQLITE_ROW, c'est que la valeur existe !
		if (sqlite3_step(stmt) == SQLITE_ROW) {
			hasFoundValue = true;
		}
	}
	else {
		std::cerr << "Erreur de préparation SQL : " << sqlite3_errmsg(m_db.get()) << std::endl;
	}

	sqlite3_finalize(stmt); // Libération du statement

	// Renvoie true si trouvé, false sinon
	return hasFoundValue;
}

/****************************************************************
* Purpose : Créer les tables dans la base de données
****************************************************************/
bool Database::createTablesOnStart()
{
	std::cout << "\nTentative de creation des tables de la base de données." << std::endl;

	// On vérifie que la BDD est bien ouverte
	if (m_db == nullptr) {
		std::cerr << "Erreur : La connexion à la base de données n'est pas ouverte !" << std::endl;
		return false;
	}

	char* errMsg = nullptr;

	// Activation des clés étrangères
	if (sqlite3_exec(m_db.get(), "PRAGMA foreign_keys = ON;", nullptr, nullptr, &errMsg) != SQLITE_OK) {
		std::cerr << "Impossible d'activer les contraintes de cles etrangeres : " << errMsg << std::endl;
		sqlite3_free(errMsg);
		return false;
	}

	// Début de la transaction
	if (sqlite3_exec(m_db.get(), "BEGIN TRANSACTION;", nullptr, nullptr, &errMsg) != SQLITE_OK) {
		std::cerr << "Impossible de demarrer la transaction : " << errMsg << std::endl;
		sqlite3_free(errMsg);
		return false;
	}

	// Liste ordonnée de toutes les requêtes de création (en respectant l'ordre des Foreign Keys)
	std::vector<std::string> creationQueries = {
		// Continent
		"CREATE TABLE IF NOT EXISTS Continent ( "
		"code_c TEXT, libelle_c TEXT NOT NULL, PRIMARY KEY(code_c), UNIQUE(libelle_c) );",

		// Pays
		"CREATE TABLE IF NOT EXISTS Pays ( "
		"code_p TEXT, libelle_p TEXT NOT NULL, code_c TEXT NOT NULL, "
		"PRIMARY KEY(code_p), UNIQUE(libelle_p), FOREIGN KEY(code_c) REFERENCES Continent(code_c) );",

		// Village
		"CREATE TABLE IF NOT EXISTS Village ( "
		"code_v TEXT, libelle_v TEXT NOT NULL, code_p TEXT NOT NULL, "
		"PRIMARY KEY(code_v), UNIQUE(libelle_v), FOREIGN KEY(code_p) REFERENCES Pays(code_p) );",

		// Activité
		"CREATE TABLE IF NOT EXISTS Activite ( "
		"id_a INTEGER, libelle_a TEXT NOT NULL, PRIMARY KEY(id_a) );",

		// Langue
		"CREATE TABLE IF NOT EXISTS Langue ( "
		"code_l TEXT, libelle_l TEXT NOT NULL, PRIMARY KEY(code_l), UNIQUE(libelle_l) );",

		// Organisation
		"CREATE TABLE IF NOT EXISTS Organisation ( "
		"id_o TEXT, date_o TEXT NOT NULL, cout_o REAL NOT NULL, code_v TEXT NOT NULL, id_a INTEGER NOT NULL, "
		"PRIMARY KEY(id_o), FOREIGN KEY(code_v) REFERENCES Village(code_v), FOREIGN KEY(id_a) REFERENCES Activite(id_a) );",

		// Avoir
		"CREATE TABLE IF NOT EXISTS Avoir ( "
		"code_p TEXT, code_v TEXT, PRIMARY KEY(code_p, code_v), "
		"FOREIGN KEY(code_p) REFERENCES Pays(code_p), FOREIGN KEY(code_v) REFERENCES Village(code_v) );",

		// Parler
		"CREATE TABLE IF NOT EXISTS Parler ( "
		"code_v TEXT, code_l TEXT, PRIMARY KEY(code_v, code_l), "
		"FOREIGN KEY(code_v) REFERENCES Village(code_v), FOREIGN KEY(code_l) REFERENCES Langue(code_l) );",

		// Index
		"CREATE INDEX IF NOT EXISTS idx_o_v_a ON Organisation (code_v, id_a);",
		"CREATE INDEX IF NOT EXISTS idx_p_code_c ON Pays (code_c);",
		"CREATE INDEX IF NOT EXISTS idx_v_code_p ON Village (code_p);"
	};

	// Exécution de la boucle de création
	for (const auto& query : creationQueries) {
		if (sqlite3_exec(m_db.get(), query.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
			// Si une seule table échoue, on gère l'erreur et on annule tout (Rollback)
			std::cerr << "Erreur critique lors de l'initialisation : " << errMsg << std::endl;
			sqlite3_free(errMsg);

			std::cout << "Annulation des modifications (Rollback)..." << std::endl;
			sqlite3_exec(m_db.get(), "ROLLBACK;", nullptr, nullptr, nullptr);

			system("pause");
			return false;
		}
	}

	// Validation de la transaction
	if (sqlite3_exec(m_db.get(), "COMMIT;", nullptr, nullptr, &errMsg) != SQLITE_OK) {
		std::cerr << "Erreur lors du Commit : " << errMsg << std::endl;
		sqlite3_free(errMsg);
		return false;
	}

	std::cout << "Base de donnees initialisée avec succès (Transaction validée) !" << std::endl;
	return true;
}

std::vector<ContinentData> Database::getAllContinents() {
	std::vector<ContinentData> list;

	// On vérifie que la BDD est bien ouverte
	if (m_db == nullptr) {
		std::cerr << "Erreur : La connexion à la base de données n'est pas ouverte !" << std::endl;
		return list;
	}

	// L'ordre du SELECT doit impérativement correspondre d'aux indices 0, 1, 2 du .cpp
	std::string sql =
		"SELECT C.code_c, C.libelle_c "
		"FROM Continent C "
		"ORDER BY C.libelle_c ASC;";

	sqlite3_stmt* stmt = nullptr;
	if (sqlite3_prepare_v2(m_db.get(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
		std::cerr << "Erreur de préparation : " << sqlite3_errmsg(m_db.get()) << std::endl;
		return list;
	}

	// On parcourt les lignes retournées par SQLite
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		ContinentData data;

		// Extraction des colonnes (attention à bien respecter l'ordre du SELECT)
		const unsigned char* codePtr = sqlite3_column_text(stmt, 0);
		const unsigned char* libellePtr = sqlite3_column_text(stmt, 1);

		// Conversion sécurisée en std::string (gestion des valeurs potentiellement NULL)
		data.code = codePtr ? reinterpret_cast<const char*>(codePtr) : "";
		data.libelle = libellePtr ? reinterpret_cast<const char*>(libellePtr) : "";

		// Ajout du continent à la liste
		list.push_back(data);
	}

	sqlite3_finalize(stmt);
	return list;
}

std::vector<PaysData> Database::getAllPays() {
	std::vector<PaysData> list;

	// On vérifie que la BDD est bien ouverte
	if (m_db == nullptr) {
		std::cerr << "Erreur : La connexion à la base de données n'est pas ouverte !" << std::endl;
		return list;
	}

	// L'ordre du SELECT doit impérativement correspondre d'aux indices 0, 1, 2 du .cpp
	std::string sql =
		"SELECT P.code_p, P.libelle_p, C.code_c, C.libelle_c "
		"FROM Pays P "
		"INNER JOIN Continent C ON P.code_c = C.code_c "
		"ORDER BY C.libelle_c ASC, P.libelle_p ASC;";

	sqlite3_stmt* stmt = nullptr;
	if (sqlite3_prepare_v2(m_db.get(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
		std::cerr << "Erreur de préparation : " << sqlite3_errmsg(m_db.get()) << std::endl;
		return list;
	}

	// On parcourt les lignes retournées par SQLite
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		PaysData data;

		// Extraction des colonnes (attention à bien respecter l'ordre du SELECT)
		const unsigned char* codePtr = sqlite3_column_text(stmt, 0);
		const unsigned char* libellePtr = sqlite3_column_text(stmt, 1);
		const unsigned char* codeContinentPtr = sqlite3_column_text(stmt, 2);
		const unsigned char* libelleContinentPtr = sqlite3_column_text(stmt, 3);

		// Conversion sécurisée en std::string (gestion des valeurs potentiellement NULL)
		data.code = codePtr ? reinterpret_cast<const char*>(codePtr) : "";
		data.libelle = libellePtr ? reinterpret_cast<const char*>(libellePtr) : "";
		data.codeContinent = codeContinentPtr ? reinterpret_cast<const char*>(codeContinentPtr) : "";
		data.libelleContinent = libelleContinentPtr ? reinterpret_cast<const char*>(libelleContinentPtr) : "";

		// Ajout du pays à la liste
		list.push_back(data);
	}

	sqlite3_finalize(stmt);
	return list;
}

std::vector<VillageData> Database::getAllVillages() {
	std::vector<VillageData> list;

	// On vérifie que la BDD est bien ouverte
	if (m_db == nullptr) {
		std::cerr << "Erreur : La connexion à la base de données n'est pas ouverte !" << std::endl;
		return list;
	}

	// L'ordre du SELECT doit impérativement correspondre d'aux indices 0, 1, 2 du .cpp
	std::string sql =
		"SELECT V.code_v, V.libelle_v, P.code_p, P.libelle_p "
		"FROM Village V "
		"INNER JOIN Pays P ON V.code_p = P.code_p "
		"ORDER BY P.libelle_p ASC, V.libelle_v ASC;";

	sqlite3_stmt* stmt = nullptr;
	if (sqlite3_prepare_v2(m_db.get(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
		std::cerr << "Erreur de préparation : " << sqlite3_errmsg(m_db.get()) << std::endl;
		return list;
	}

	// On parcourt les lignes retournées par SQLite
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		VillageData data;

		// Extraction des colonnes (attention à bien respecter l'ordre du SELECT)
		const unsigned char* codePtr = sqlite3_column_text(stmt, 0);
		const unsigned char* libellePtr = sqlite3_column_text(stmt, 1);
		const unsigned char* codePaysPtr = sqlite3_column_text(stmt, 2);
		const unsigned char* libellePaysPtr = sqlite3_column_text(stmt, 3);

		// Conversion sécurisée en std::string (gestion des valeurs potentiellement NULL)
		data.code = codePtr ? reinterpret_cast<const char*>(codePtr) : "";
		data.libelle = libellePtr ? reinterpret_cast<const char*>(libellePtr) : "";
		data.codePays = codePaysPtr ? reinterpret_cast<const char*>(codePaysPtr) : "";
		data.libellePays = libellePaysPtr ? reinterpret_cast<const char*>(libellePaysPtr) : "";

		// Ajout du village à la liste
		list.push_back(data);
	}

	sqlite3_finalize(stmt);
	return list;
}