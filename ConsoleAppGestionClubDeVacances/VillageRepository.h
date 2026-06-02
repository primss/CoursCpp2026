#pragma once

#include "Repository.h"
#include "VillageData.h"

class VillageRepository : public Repository<VillageData> {
public:
    VillageData create(Database& db, const VillageData& villageData) override;
    VillageData read(Database& db, const std::string& code) override;
    bool update(Database& db, const std::string& oldCode, const VillageData& villageData) override;
    bool destroy(Database& db, const std::string& code) override;
    std::vector<VillageData> getAll(Database& db) override;
	bool valueExists(Database& db, const std::string& columnName, const std::string& searchParam) override;
};