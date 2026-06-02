#pragma once

#include "Repository.h"
#include "PaysData.h"

class PaysRepository : public Repository<PaysData> {
public:
	PaysData create(Database& db, const PaysData& villageData) override;
	PaysData read(Database& db, const std::string& code) override;
	bool update(Database& db, const std::string& oldCode, const PaysData& villageData) override;
	bool destroy(Database& db, const std::string& code) override;
	std::vector<PaysData> getAll(Database& db) override;
	bool valueExists(Database& db, const std::string& columnName, const std::string& searchParam) override;
};