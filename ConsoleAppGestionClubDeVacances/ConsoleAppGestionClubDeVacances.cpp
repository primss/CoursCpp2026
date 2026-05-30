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
***************************************************************************************************/

#include <iostream>
#include <limits>
#include <string>
#include <windows.h>

#include "Database.h"
#include "Utilities.h"
#include "Continent.h"
#include "Pays.h"
#include "Village.h"

using namespace std;

typedef unsigned int UInt;

enum class Action {
    Add,
    List,
    Edit,
    Delete
};

// Déclaration pour contourner l'écriture des fonctions par appel du haut vers le bas
void handleUserActionAddEntity(Database &db);

UInt selectActionMenu()
/****************************************************************
* Purpose : Permettre à l'utilisateur de choisir l'action à faire
****************************************************************/
{
    UInt choix;
    cout << "\nSélectionner une action à effectuer avec la base de données" << endl;
    cout << "1. Ajouter" << endl;
    cout << "2. Lister" << endl;
    cout << "3. Modifier" << endl;
    cout << "4. Supprimer" << endl;
    cout << "0. Quitter" << endl;

    cout << "Entrer votre choix : ";
    if (!(cin >> choix)) {
        cin.clear();
        cin.ignore((numeric_limits<streamsize>::max)(), '\n');
        return 999; // Retourner un choix invalide pour déclencher le switch par défaut
    }
    return choix;
} // UInt selectActionMenu()

UInt selectEntitySubMenu(Action action)
/****************************************************************
* Purpose : Permettre à l'utilisateur de choisir l'entité sur laquelle agir
****************************************************************/
{
    UInt choix;
    switch (action) {
    case Action::Add:
        cout << "\nSélectionner le type d'entité à ajouter" << endl;
        break;
    case Action::List:
        cout << "\nSélectionner le type d'entité à lister" << endl;
        break;
    }

    cout << "1. Continent" << endl;
    cout << "2. Pays" << endl;
    cout << "3. Langue" << endl;
    cout << "4. Village" << endl;
    cout << "5. Activité" << endl;
    cout << "0. Retour au menu principal" << endl;

    cout << "Entrer votre choix : ";
    if (!(cin >> choix)) {
        cin.clear();
        cin.ignore((numeric_limits<streamsize>::max)(), '\n');
        return 999; // Retourner un choix invalide pour déclencher le switch par défaut
    }

    return choix;
} // UInt selectEntitySubMenu()


void handleUserActionAddEntity(Database &db)
/****************************************************************
* Purpose : Permettre à l'utilisateur de choisir l'entité à créer
****************************************************************/
{
    UInt choix;

    do {
        Utilities::showAppTitle();
        choix = selectEntitySubMenu(Action::Add);
        switch (choix) {
        case 0:
            cout << "\nRetour au menu principal !" << endl;
            break;
        case 1:
			inputContinentData(db);
            break;
        case 2:
			inputPaysData(db);
            break;
        case 3:
            break;
        case 4:
            inputVillageData(db);
            break;
        case 5:
			break;
        default:
            cout << "Choix invalide." << endl;
            system("pause");
            break;
        }
    } while (choix != 0);
}

void handleUserActionListEntities(Database &db)
/****************************************************************
* Purpose : Permettre à l'utilisateur de choisir l'entité à créer
****************************************************************/
{
    UInt choix;

    do {
        Utilities::showAppTitle();
        choix = selectEntitySubMenu(Action::List);
        switch (choix) {
        case 0:
            cout << "\nRetour au menu principal !" << endl;
            break;
        case 1:
			listSavedContinents(db);
            break;
        case 2:
			listSavedPays(db);
            break;
        case 3:
            break;
        case 4:
            listSavedVillages(db);
            break;
        case 5:
            break;
        default:
            cout << "Choix invalide." << endl;
            system("pause");
            break;
        }
    } while (choix != 0);
}

void handleUserActionSelected(Database &db)
/****************************************************************
* Purpose : Permettre à l'utilisateur de choisir l'action à faire
****************************************************************/
{
    UInt choix;

    do {
        Utilities::showAppTitle();
        choix = selectActionMenu();
        switch (choix) {
        case 0:
            cout << "\nFin de l'application !" << endl;
            break;
        case 1:
            handleUserActionAddEntity(db);
            break;
        case 2:
            handleUserActionListEntities(db);
            break;
        case 3:
            break;
        case 4:
            break;
        default:
            cout << "Choix invalide." << endl;
            system("pause");
            break;
        }
    } while (choix != 0);
}

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
        std::cerr << "Echec du demarrage : impossible d'initialiser les tables." << std::endl;
        return 1;
    }

    handleUserActionSelected(db);

    // Fermer la base de données
    db.close();

    return 0;
}