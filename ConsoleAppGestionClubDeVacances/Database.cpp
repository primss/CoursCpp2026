#include "Database.h"
#include "sqlite3.h"

#include <iostream>
#include <vector>

Database::Database() = default;
Database::~Database() = default;
Database::Database(Database&&) noexcept = default;
Database& Database::operator=(Database&&) noexcept = default;

sqlite3* Database::getRawDb() const {
	return m_db.get(); // .get() extrait le pointeur brut du unique_ptr
}

std::string Database::getLastError() const {
	if (m_db) {
		// sqlite3_errmsg renvoie le dernier message d'erreur au format const char*
		return sqlite3_errmsg(m_db.get());
	}
	return "Aucune base de donnees ouverte.";
}

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
		"PRIMARY KEY(code_p), UNIQUE(libelle_p), FOREIGN KEY(code_c) "
		"REFERENCES Continent(code_c) ON UPDATE CASCADE ON DELETE CASCADE);",

		// Village
		"CREATE TABLE IF NOT EXISTS Village ( "
		"code_v TEXT, libelle_v TEXT NOT NULL, code_p TEXT NOT NULL, "
		"PRIMARY KEY(code_v), UNIQUE(libelle_v), FOREIGN KEY(code_p) "
		"REFERENCES Pays(code_p) ON UPDATE CASCADE ON DELETE CASCADE);",

		// Activité
		"CREATE TABLE IF NOT EXISTS Activite ( "
		"id_a INTEGER PRIMARY KEY AUTOINCREMENT, libelle_a TEXT NOT NULL );",

		// Langue
		"CREATE TABLE IF NOT EXISTS Langue ( "
		"code_l TEXT, libelle_l TEXT NOT NULL, PRIMARY KEY(code_l), UNIQUE(libelle_l) );",

		// Organisation
		"CREATE TABLE IF NOT EXISTS Organisation ( "
		"id_o INTEGER PRIMARY KEY AUTOINCREMENT, date_o TEXT NOT NULL, cout_o REAL NOT NULL, "
		"code_v TEXT NOT NULL, id_a INTEGER NOT NULL, "
		"FOREIGN KEY(code_v) REFERENCES Village(code_v) ON UPDATE CASCADE ON DELETE CASCADE, "
		"FOREIGN KEY(id_a) REFERENCES Activite(id_a) ON UPDATE CASCADE ON DELETE CASCADE);",

		// Avoir
		"CREATE TABLE IF NOT EXISTS Avoir ( "
		"code_p TEXT, code_v TEXT, PRIMARY KEY(code_p, code_v), "
		"FOREIGN KEY(code_p) REFERENCES Pays(code_p) ON UPDATE CASCADE ON DELETE CASCADE, "
		"FOREIGN KEY(code_v) REFERENCES Village(code_v) ON UPDATE CASCADE ON DELETE CASCADE);",

		// Parler
		"CREATE TABLE IF NOT EXISTS Parler ( "
		"code_v TEXT, code_l TEXT, PRIMARY KEY(code_v, code_l), "
		"FOREIGN KEY(code_v) REFERENCES Village(code_v) ON UPDATE CASCADE ON DELETE CASCADE, "
		"FOREIGN KEY(code_l) REFERENCES Langue(code_l) ON UPDATE CASCADE ON DELETE CASCADE);",

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

	// On injecte les données avant de valider (COMMIT) la transaction
	if (!seedInitialData()) {
		std::cout << "Annulation globale suite a l'echec du Seeding (Rollback)..." << std::endl;
		sqlite3_exec(m_db.get(), "ROLLBACK;", nullptr, nullptr, nullptr);
		system("pause");
		return false;
	}

	// Validation de la transaction globale (Tables + Données)
	if (sqlite3_exec(m_db.get(), "COMMIT;", nullptr, nullptr, &errMsg) != SQLITE_OK) {
		std::cerr << "Erreur lors du Commit : " << errMsg << std::endl;
		sqlite3_free(errMsg);
		return false;
	}

	std::cout << "Base de donnees initialisée avec succès (Transaction validée) !" << std::endl;
	return true;
}

bool Database::seedInitialData() {
	char* errMsg = nullptr;

	// Liste des données de test (Respectez bien l'ordre des contraintes de clés étrangères !)
	std::vector<std::string> seedQueries = {
		// Continents
		"INSERT OR IGNORE INTO Continent (code_c, libelle_c) VALUES ('EUR', 'Europe');",
		"INSERT OR IGNORE INTO Continent (code_c, libelle_c) VALUES ('AFR', 'Afrique');",
		"INSERT OR IGNORE INTO Continent (code_c, libelle_c) VALUES ('ASI', 'Asie');",

		// Pays
		"INSERT OR IGNORE INTO Pays (code_p, libelle_p, code_c) VALUES ('FRA', 'France', 'EUR');",
		"INSERT OR IGNORE INTO Pays (code_p, libelle_p, code_c) VALUES ('TOG', 'Togo', 'AFR');",
		"INSERT OR IGNORE INTO Pays (code_p, libelle_p, code_c) VALUES ('JPN', 'Japon', 'ASI');",

		// Villages
		"INSERT OR IGNORE INTO Village (code_v, libelle_v, code_p) VALUES ('CAR', 'Cargèse', 'FRA');",
		"INSERT OR IGNORE INTO Village (code_v, libelle_v, code_p) VALUES ('KPA', 'Kpalimé', 'TOG');",
		"INSERT OR IGNORE INTO Village (code_v, libelle_v, code_p) VALUES ('SAH', 'Sahoro', 'JPN');",

		// Activités
		"INSERT OR IGNORE INTO Activite (id_a, libelle_a) VALUES (1, 'Agriculture');",
		"INSERT OR IGNORE INTO Activite (id_a, libelle_a) VALUES (2, 'Commerce');",
		"INSERT OR IGNORE INTO Activite (id_a, libelle_a) VALUES (3, 'Pêche');",

		// Langues
		"INSERT OR IGNORE INTO Langue (code_l, libelle_l) VALUES ('FR', 'Français');",
		"INSERT OR IGNORE INTO Langue (code_l, libelle_l) VALUES ('EW', 'Ewé');",
		"INSERT OR IGNORE INTO Langue (code_l, libelle_l) VALUES ('JP', 'Japonais');"
	};

	std::cout << "Injection des donnees initiales (Seed Data)..." << std::endl;

	for (const auto& query : seedQueries) {
		if (sqlite3_exec(m_db.get(), query.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
			std::cerr << "Erreur lors du Seeding : " << errMsg << std::endl;
			sqlite3_free(errMsg);
			return false;
		}
	}

	return true;
}