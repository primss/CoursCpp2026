#pragma once

#include <iostream>
#include <string>

#include "Database.h"

void handleCreateVillage(Database &db, const VillageData& villageData);
void inputVillageData(Database &db);
void listSavedVillages(Database &db);