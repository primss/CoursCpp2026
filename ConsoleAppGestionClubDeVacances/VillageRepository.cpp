#include <sqlite3.h>
#include <stdexcept>
#include <iostream>
#include <memory>

#include "VillageRepository.h"
#include "Database.h"

VillageData getVillageFromSqliteStmt(sqlite3_stmt* stmt);

VillageData VillageRepository::create(Database& db, const VillageData& villageData)
{
	std::string sql = "INSERT INTO Village (code_v, libelle_v, code_p) VALUES (?, ?, ?);";

	// On regroupe les paramètres dans un tableau de chaînes
	std::vector<std::string> params = {
		villageData.getCode(),
		villageData.getLibelle(),
		villageData.getCodePays()
	};

	VillageData result;

	// On délègue l'exécution à notre classe Database
	if (db.executePrepared(sql, params)) {
		result = this->read(db, villageData.getCode());
	}
	else {
		throw std::runtime_error(db.getLastError());
	}

	return result;
}

VillageData VillageRepository::read(Database& db, const std::string& code)
{
	VillageData data;
	// On vérifie que la BDD est bien ouverte
	if (db.getRawDb() == nullptr) {
		return data;
	}
	std::string sql =
		"SELECT V.code_v, V.libelle_v, P.code_p, P.libelle_p "
		"FROM Village V "
		"INNER JOIN Pays P ON V.code_p = P.code_p "
		"WHERE V.code_v = ?;";
	sqlite3_stmt* stmt = nullptr;
	if (sqlite3_prepare_v2(db.getRawDb(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
		return data;
	}
	sqlite3_bind_text(stmt, 1, code.c_str(), -1, SQLITE_STATIC);

	if (sqlite3_step(stmt) == SQLITE_ROW) {
		data = getVillageFromSqliteStmt(stmt);
	}

	sqlite3_finalize(stmt);
	return data;
}

bool VillageRepository::update(Database& db, const std::string& oldCode, const VillageData& villageData)
{
	// On met à jour code_v, libelle_v et code_p en se basant sur l'ancien code_v
	std::string sql = "UPDATE Village SET code_v = ?, libelle_v = ?, code_p = ? WHERE code_v = ?;";

	std::vector<std::string> params = {
		villageData.getCode(),      // 1er ? : Nouveau Code
		villageData.getLibelle(),   // 2e ?  : Nouveau Libellé
		villageData.getCodePays(),  // 3e ?  : Nouveau Pays
		oldCode					// 4e ?  : l'Ancien Code pour le WHERE
	};

	bool success(false);
	if (db.executePrepared(sql, params)) {
		std::cout << "\nVillage mis à jour avec succès !" << std::endl;
		if (oldCode != villageData.getCode()) {
			std::cout << "L'ancien code '" << oldCode << "' a été remplacé par '" << villageData.getCode() << "'.\n";
		}

		system("pause");
		success = true;
	}

	return success;
}

bool VillageRepository::destroy(Database& db, const std::string& code)
{
	std::string sql = "DELETE FROM Village WHERE code_v = ?;";
	std::vector<std::string> params = { code };
	return db.executePrepared(sql, params);
}

std::vector<VillageData> VillageRepository::getAll(Database& db)
{
	std::vector<VillageData> list;

	// On vérifie que la BDD est bien ouverte
	if (db.getRawDb() == nullptr) {
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
	if (sqlite3_prepare_v2(db.getRawDb(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
		std::cerr << "Erreur de préparation : " << sqlite3_errmsg(db.getRawDb()) << std::endl;
		return list;
	}

	// On parcourt les lignes retournées par SQLite
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		VillageData data = getVillageFromSqliteStmt(stmt);

		// Ajout du village à la liste
		list.push_back(data);
	}

	sqlite3_finalize(stmt);
	return list;
}

bool VillageRepository::valueExists(Database& db, const std::string& columnName, const std::string& searchParam)
{
	return db.valueExists("Village", columnName, searchParam);
}

VillageData getVillageFromSqliteStmt(sqlite3_stmt* stmt) {
	const unsigned char* codePtr = sqlite3_column_text(stmt, 0);
	const unsigned char* libellePtr = sqlite3_column_text(stmt, 1);
	const unsigned char* codePaysPtr = sqlite3_column_text(stmt, 2);
	const unsigned char* libellePaysPtr = sqlite3_column_text(stmt, 3);
	std::string code = codePtr ? reinterpret_cast<const char*>(codePtr) : "";
	std::string libelle = libellePtr ? reinterpret_cast<const char*>(libellePtr) : "";
	std::string codePays = codePaysPtr ? reinterpret_cast<const char*>(codePaysPtr) : "";
	std::string libellePays = libellePaysPtr ? reinterpret_cast<const char*>(libellePaysPtr) : "";

	VillageData data(code, libelle, codePays);
	
	// Liaison avec le Continent si les données du continent existent
    if (!codePays.empty()) {
		// On utilise directement le constructeur avec paramètres pour créer le pays
		PaysData pays(codePays, libellePays);

		// On l'associe au village en transférant la propriété
		data.setPays(pays);
	}

	return data;
}