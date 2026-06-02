#pragma once

#include <iostream>
#include <string>

#include "VillageRepository.h"

typedef unsigned int UInt;

UInt selectActionMenu();
void handleVillageActions(Database& db);

void handleCreateVillage(Database& db, const VillageData& villageData);
void handleUpdateVillageCodeAndData(Database& db, const std::string& ancienCode, const VillageData& newData);
void handleDeleteVillage(Database& db);
void inputVillageData(Database &db, bool onCreate = true);
void listSavedVillages(Database &db);
std::string getVillageCode(Database& db);