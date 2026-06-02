/***************************************************************************************************
* PROJET        : Gestion Club de Vacances (GCV)
* FICHIER       : main.cpp
* TYPE          : Code Source C++
* ARCHITECTURE  : x64 | SQLite 3 (Local-First Architecture)
* OUTILS        : Microsoft Visual Studio 2026
*---------------------------------------------------------------------------------------------------
* DESCRIPTION   :
* Point d'entrée principal de l'application GCV. Ce module gère l'interface utilisateur en console,
* les menus et sous menus dynamiques, le contrôle des saisies de données (cin/getline) ainsi
* que l'initialisation et la persistance des données via le SGBD relationnel SQLite3.
*
* ENTITÉS GÉRÉES:
* - Continent (code_c, libelle_c)
* - Pays (code_p, libelle_p)
* - Village (code_v, libelle_v)
*---------------------------------------------------------------------------------------------------
* HISTORIQUE DES MODIFICATIONS :
* Version   Date        Auteur          Description / Correctifs
* -------   ----------  --------------  ------------------------------------------------------------
* 0.0.1     28/05/2026  A. Abdallah     Version initiale, création des tables et CRUD Village.
* 0.0.2     29/05/2026  A. Abdallah     Correction des affichages, création des tables et ajustements mineurs.
* 0.0.3     30/05/2026  A. Abdallah     Ajout de la classe Database pour l'encapsulation de sqlite3.
* 0.0.4     31/05/2026  A. Abdallah     Utilisation de classes entités et du repository pattern.
***************************************************************************************************/

#include <iostream>
#include <limits>
#include <string>
#include <windows.h>

#include "Database.h"
#include "Utilities.h"
#include "Village.h"

using namespace std;

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    Database db;
    const char* dbName = "gcv.db";

    // Ouverture de la base de données
    if (!db.open(dbName)) {
        std::cerr << "Impossible d'ouvrir le fichier de BDD." << std::endl;
        return 1;
    }

    // On crée l'architecture des tables si elles n'existent pas
    if (!db.createTablesOnStart()) {
        std::cerr << "Echec du démarrage : impossible d'initialiser les tables." << std::endl;
        return 1;
    }

    handleVillageActions(db);

    // Fermer la base de données
    db.close();

    return 0;
}