#pragma once

#include <iostream>
#include <string>

#include "Database.h"

void handleCreatePays(Database& db, const PaysData& paysData);
void inputPaysData(Database& db);
void listSavedPays(Database& db);