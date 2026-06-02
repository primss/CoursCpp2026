#include <sqlite3.h>
#include <memory>

#include "PaysRepository.h"
#include "Database.h"

PaysData getPaysFromSqliteStmt(sqlite3_stmt* stmt);

PaysData PaysRepository::create(Database& db, const PaysData& villageData)
{
	return PaysData();
}

PaysData PaysRepository::read(Database& db, const std::string& code)
{
	return PaysData();
}

bool PaysRepository::update(Database& db, const std::string& oldCode, const PaysData& villageData)
{
	return true;
}

bool PaysRepository::destroy(Database& db, const std::string& code)
{
	std::string sql = "DELETE FROM Pays WHERE code = ?;";
	std::vector<std::string> params = { code };
	return db.executePrepared(sql, params);
}

std::vector<PaysData> PaysRepository::getAll(Database& db)
{
	std::vector<PaysData> list;

	
	return list;
}

bool PaysRepository::valueExists(Database& db, const std::string& columnName, const std::string& searchParam)
{
	return db.valueExists("Pays", columnName, searchParam);
}

PaysData getPaysFromSqliteStmt(sqlite3_stmt* stmt) 
{
	const unsigned char* codePtr = sqlite3_column_text(stmt, 0);
	const unsigned char* libellePtr = sqlite3_column_text(stmt, 1);
	const unsigned char* codeContinentPtr = sqlite3_column_text(stmt, 2);
	const unsigned char* libelleContinentPtr = sqlite3_column_text(stmt, 3);

	std::string code = codePtr ? reinterpret_cast<const char*>(codePtr) : "";
	std::string libelle = libellePtr ? reinterpret_cast<const char*>(libellePtr) : "";
	std::string codeContinent = codeContinentPtr ? reinterpret_cast<const char*>(codeContinentPtr) : "";
	std::string libelleContinent = libelleContinentPtr ? reinterpret_cast<const char*>(libelleContinentPtr) : "";

	PaysData data(code, libelle, codeContinent);

	// Liaison avec le Continent si les données du continent existent
	if (!codeContinent.empty()) {
		// On utilise directement le constructeur avec paramètres pour créer le continent
		ContinentData continent(codeContinent, libelleContinent);

		// On l'associe au pays (le setter va déréférencer le pointeur)
		data.setContinent(continent);
	}

	return data;
}