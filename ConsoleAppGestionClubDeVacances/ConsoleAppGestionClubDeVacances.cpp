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
* - Village (code_v, libelle_v)
*---------------------------------------------------------------------------------------------------
* HISTORIQUE DES MODIFICATIONS :
* Version   Date        Auteur          Description / Correctifs
* -------   ----------  --------------  ------------------------------------------------------------
* 0.0.1     28/05/2026  A. Abdallah     Version initiale, création des tables et CRUD Village.
***************************************************************************************************/

#include <iostream>
#include <limits>
#include <string>

#include "sqlite3.h"

using namespace std;

typedef unsigned int UInt;

enum class Action {
    Add,
    List,
    Edit,
    Delete
};

// Déclaration pour contourner l'écriture des fonctions par appel du haut vers le bas
void handleUserActionAddEntity(sqlite3* db);


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
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
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

    cout << "1. Village" << endl;
    cout << "0. Retour au menu principal" << endl;

    cout << "Entrer votre choix : ";
    if (!(cin >> choix)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return 999; // Retourner un choix invalide pour déclencher le switch par défaut
    }

    return choix;
} // UInt selectEntitySubMenu()

void showAppTitle()
/****************************************************************
* Purpose : Afficher le titre de l'application
****************************************************************/
{
    system("CLS");
    cout << "===========================================================" << endl;
    cout << "|                      CLUB DE VACANCES                   |" << endl;
    cout << "|                        GCV v0.0.1                       |" << endl;
    cout << "===========================================================" << endl;
}

void addVillageToDb(sqlite3* db, const string& code, const string& libelle)
/****************************************************************
* Purpose : Insérer le nouveau village dans la base de données.
****************************************************************/
{
    const char* sql = "INSERT INTO Village (code_v, libelle_v) VALUES (?, ?);";
    sqlite3_stmt* stmt = nullptr;

    bool insertOk(true);
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        // Liaison des paramètres (les index commencent à 1)
        sqlite3_bind_text(stmt, 1, code.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, libelle.c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            cerr << "Erreur d'insertion : " << sqlite3_errmsg(db) << endl;
            insertOk = false;
        }
    }
    sqlite3_finalize(stmt);

    if (insertOk) {
        cout << "\nVillage : " << code << " - " << libelle << " ajouté à la base de donnée." << endl;
        system("pause");
    }
}

void addVillageInput(sqlite3* db)
/****************************************************************
* Purpose : Ajouter un village => Contrôle de la saisie
****************************************************************/
{
    string code, libelle;
    bool test(false);

    showAppTitle();
    cout << "\nAjout d'un nouveau village" << endl;
    do {
        cout << "Saisir le code (3 lettres) : ";
        cin >> code;
        if (code.empty() || code.length() != 3) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrée invalide. Réessayez.\n";
            continue;
        }

        //"nécessaire à cause de l'utilisation de \n ci-dessus"
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        test = true;
    } while (test == false);

    test = false;
    do {
        cout << "Saisir le libelle (30 caratères max) :";
        getline(cin, libelle); // cin s'arrête à espace, retour à la ligne...

        if (libelle.empty()) {
            cout << "Entrée requise. Réessayez.\n";
            continue;
        }
        else if (libelle.length() > 30) {
            cout << "Entrée trop long. Réessayez.\n";
            continue;
        }
        test = true;
    } while (test == false);

    // Ajouter à la base de données.
    addVillageToDb(db, code, libelle);
}

void listSavedVillages(sqlite3* db)
/****************************************************************
* Purpose : Lister les villages
****************************************************************/
{
    showAppTitle();

    const char* sql = "SELECT code_v, libelle_v FROM Village;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Erreur de préparation : " << sqlite3_errmsg(db) << endl;
        return;
    }

    cout << "\nListe des Villages" << endl;
    bool showHeader(true);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        // Récupération des colonnes (0 pour le code, 1 pour le libellé)
        const unsigned char* code = sqlite3_column_text(stmt, 0);
        const unsigned char* libelle = sqlite3_column_text(stmt, 1);

        if (showHeader) {
            cout << "Code | Libellé" << endl;
            showHeader = false;
        }
        cout << (code ? reinterpret_cast<const char*>(code) : "NULL")
            << "  | "
            << (libelle ? reinterpret_cast<const char*>(libelle) : "NULL")
            << endl;
    }

    // Si la table était vide
    if (showHeader) {
        cout << "Aucun village enregistré dans la base." << endl;
    }

    cout << endl;
    system("pause");

    // Destruction du statement pour libérer la mémoire (Crucial pour éviter les fuites)
    sqlite3_finalize(stmt);
}

void handleUserActionAddEntity(sqlite3* db)
/****************************************************************
* Purpose : Permettre à l'utilisateur de choisir l'entité à créer
****************************************************************/
{
    UInt choix;

    do {
        showAppTitle();
        choix = selectEntitySubMenu(Action::Add);
        switch (choix) {
        case 0:
            cout << "\nRetour au menu principal !" << endl;
            break;
        case 1:
            addVillageInput(db);
            break;
        case 2:
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

void handleUserActionListEntities(sqlite3* db)
/****************************************************************
* Purpose : Permettre à l'utilisateur de choisir l'entité à créer
****************************************************************/
{
    UInt choix;

    do {
        showAppTitle();
        choix = selectEntitySubMenu(Action::List);
        switch (choix) {
        case 0:
            cout << "\nRetour au menu principal !" << endl;
            break;
        case 1:
            listSavedVillages(db);
            break;
        case 2:
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

void handleUserActionSelected(sqlite3* db)
/****************************************************************
* Purpose : Permettre à l'utilisateur de choisir l'action à faire
****************************************************************/
{
    UInt choix;

    do {
        showAppTitle();
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

bool createTablesOnStart(sqlite3* db)
/****************************************************************
* Purpose : Créer les tables dans la base de données
****************************************************************/
{
    showAppTitle();

    cout << "\nTentative de création des tables de la base de données." << endl;

    bool result(true);

    // Village
    cout << "Table \"Village\" en cours de création..." << endl;
    string query_str =
        "CREATE TABLE IF NOT EXISTS Village ( "
        "code_v TEXT, "
        "libelle_v TEXT NOT NULL, "
        "PRIMARY KEY(code_v), "
        "UNIQUE(libelle_v) "
        ");";

    const char* query_char = query_str.c_str();
    cout << "Test char : " << query_char << endl;

    if (db == nullptr) {
        cerr << "Erreur : La connexion a la base de donnees n'est pas ouverte !" << endl;
        result = false;
    }
    else if (sqlite3_exec(db, query_char, nullptr, nullptr, nullptr) != SQLITE_OK) {
        cerr << "Erreur lors de la creation de la table \"Village\" : " << sqlite3_errmsg(db) << endl;
        result = false;
    }
    else {
        result = true;
    }
    cout << "Table \"Village\" créé" << endl;

    cout << "Fin de la création des tables" << endl;
    return result;
}

int main()
{
    setlocale(LC_ALL, "");

    sqlite3* db;
    const char* dbName = "gcv.db";

    // Ouverture de la base de données
    if (sqlite3_open(dbName, &db) != SQLITE_OK) {
        cerr << "Erreur lors de l'ouverture de la base de données : " << sqlite3_errmsg(db) << endl;
        return 1;
    }

    if (!createTablesOnStart(db)) {
        // Fermer la base de données
        sqlite3_close(db);
        return 1;
    }

    handleUserActionSelected(db);

    // Fermer la base de données
    sqlite3_close(db);

    return 0;
}