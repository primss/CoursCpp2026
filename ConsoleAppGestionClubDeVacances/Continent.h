#pragma once
#include <iostream>
#include <string>

#include "Database.h"

void handleCreateContinent(Database& db, const ContinentData& continentData);
void inputContinentData(Database& db);
void listSavedContinents(Database& db);