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
* - Langue (code_l, libelle_l)
* - Village (code_v, libelle_v)
* - Activité (id_a, libelle_a)
*---------------------------------------------------------------------------------------------------
* HISTORIQUE DES MODIFICATIONS :
* Version   Date        Auteur          Description / Correctifs
* -------   ----------  --------------  ------------------------------------------------------------
* 0.0.1     28/05/2026  A. Abdallah     Version initiale, création des tables et CRUD Village.
* 0.0.2     29/05/2026  A. Abdallah     Correction des affichages, création des tables et ajustements mineurs.
***************************************************************************************************/

#include <iostream>
#include <limits>
#include <string>
#include <windows.h>

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

string toUpperStr(string str)
/****************************************************************
* Purpose : Mettre une chaîne de caractères en majuscules
****************************************************************/
{
    // On parcourt la chaîne caractère par caractère grâce à une référence (&c)
    for (char& c : str) {
        c = std::toupper(static_cast<unsigned char>(c)); // On applique toupper sur CHAQUE lettre
    }
    return str;
}

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

bool isUniqueValue(sqlite3* db, const string& tableName, const string& columnName, const string& searchValue)
{
    string query_str = "SELECT 1 FROM " + tableName + " WHERE " + columnName + " = ?;";
    sqlite3_stmt* stmt = nullptr;
    bool hasFoundValue = false;

    if (sqlite3_prepare_v2(db, query_str.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, searchValue.c_str(), -1, SQLITE_TRANSIENT);

        // Si sqlite3_step renvoie SQLITE_ROW, c'est qu'une ligne correspondante a été trouvée
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            hasFoundValue = true;
        }
    }
    sqlite3_finalize(stmt); // Libération impérative du statement
    
	return hasFoundValue;
}


void addContinentToDb(sqlite3* db, const string& code, const string& libelle)
/****************************************************************
* Purpose : Insérer le nouveau continent dans la base de données.
****************************************************************/
{
    const char* sql = "INSERT INTO Continent (code_c, libelle_c) VALUES (?, ?);";
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
        cout << "\nContinent : " << code << " - " << libelle << " ajouté à la base de donnée." << endl;
        system("pause");
    }
}

void inputContinentData(sqlite3* db)
/****************************************************************
* Purpose : Ajouter un continet => Contrôle de la saisie
****************************************************************/
{
    string code, libelle;
    bool test(false);

    showAppTitle();
    cout << "\nAjout d'un nouveau continent" << endl;
    do {
        cout << "Saisir le code (3 lettres) : ";
        cin >> code;
        if (code.empty() || code.length() != 3) {
            cin.clear();
            cin.ignore((numeric_limits<streamsize>::max)(), '\n');
            cout << "Entrée invalide. Réessayez.\n";
            continue;
        }

        // Nécessaire à cause de l'utilisation de \n ci-dessus"
        cin.ignore((numeric_limits<streamsize>::max)(), '\n');

        code = toUpperStr(code);
        // Check if code already exists
        bool isValueUnique = isUniqueValue(db, "Continent", "code_c", code);

        if (isValueUnique) {
            cout << "Le code " << code << " existe déjà. Réessayez.\n";
            continue;
        }

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

        // Check if code already exists
        bool isValueUnique = isUniqueValue(db, "Continent", "libelle_c", libelle);
        if (isValueUnique) {
            cout << "Le libellé " << libelle << " existe déjà. Réessayez.\n";
            continue;
        }

        test = true;
    } while (test == false);

    // Ajouter à la base de données.
    addContinentToDb(db, code, libelle);
}

void listSavedContinents(sqlite3* db)
/****************************************************************
* Purpose : Lister les continents
****************************************************************/
{
    showAppTitle();

    const char* sql = "SELECT code_c, libelle_c FROM Continent;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Erreur de préparation : " << sqlite3_errmsg(db) << endl;
        return;
    }

    cout << "\nListe des Continents" << endl;
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
        cout << "Aucun continent enregistré dans la base." << endl;
    }

    cout << endl;
    system("pause");

    // Destruction du statement pour libérer la mémoire (Crucial pour éviter les fuites)
    sqlite3_finalize(stmt);
}


void addPaysToDb(sqlite3* db, const string& code, const string& libelle, const string& codeContinent)
/****************************************************************
* Purpose : Insérer le nouveau pays dans la base de données.
****************************************************************/
{
    const char* sql = "INSERT INTO Pays (code_p, libelle_p, code_c) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt = nullptr;

    bool insertOk(true);
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        // Liaison des paramètres (les index commencent à 1)
        sqlite3_bind_text(stmt, 1, code.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, libelle.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, codeContinent.c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            cerr << "Erreur d'insertion : " << sqlite3_errmsg(db) << endl;
            insertOk = false;
        }
    }
    sqlite3_finalize(stmt);

    if (insertOk) {
        cout << "\nPays : " << code << " - " << libelle << " ajouté à la base de donnée." << endl;
        system("pause");
    }
}

void inputPaysData(sqlite3* db)
/****************************************************************
* Purpose : Ajouter un pays => Contrôle de la saisie
****************************************************************/
{
    string code, libelle, codeContinent;
    bool test(false);

    showAppTitle();
    cout << "\nAjout d'un nouveau pays" << endl;
    do {
        cout << "Saisir le code continent du pays (3 lettres) : ";
        cin >> codeContinent;
        if (codeContinent.empty() || codeContinent.length() != 3) {
            cin.clear();
            cin.ignore((numeric_limits<streamsize>::max)(), '\n');
            cout << "Entrée invalide. Réessayez.\n";
            continue;
        }

        // Nécessaire à cause de l'utilisation de \n ci-dessus"
        cin.ignore((numeric_limits<streamsize>::max)(), '\n');

        codeContinent = toUpperStr(codeContinent);
        // Check if code already exists
        bool isValueUnique = isUniqueValue(db, "Continent", "code_c", codeContinent);

        if (!isValueUnique) {
            cout << "Le continent " << codeContinent << " n'existe pas. Réessayez.\n";
            continue;
        }

        test = true;
    } while (test == false);

    test = false;
    do {
        cout << "Saisir le code pays (3 lettres) : ";
        cin >> code;
        if (code.empty() || code.length() != 3) {
            cin.clear();
            cin.ignore((numeric_limits<streamsize>::max)(), '\n');
            cout << "Entrée invalide. Réessayez.\n";
            continue;
        }

        // Nécessaire à cause de l'utilisation de \n ci-dessus"
        cin.ignore((numeric_limits<streamsize>::max)(), '\n');

        code = toUpperStr(code);
        // Check if code already exists
        bool isValueUnique = isUniqueValue(db, "Pays", "code_p", code);

        if (isValueUnique) {
            cout << "Le code " << code << " existe déjà. Réessayez.\n";
            continue;
        }

        test = true;
    } while (test == false);

    test = false;
    do {
        cout << "Saisir le libellé (30 caratères max) :";
        getline(cin, libelle); // cin s'arrête à espace, retour à la ligne...

        if (libelle.empty()) {
            cout << "Entrée requise. Réessayez.\n";
            continue;
        }
        else if (libelle.length() > 30) {
            cout << "Entrée trop longue. Réessayez.\n";
            continue;
        }

        // Check if code already exists
        bool isValueUnique = isUniqueValue(db, "Pays", "libelle_p", libelle);
        if (isValueUnique) {
            cout << "Le libellé " << libelle << " existe déjà. Réessayez.\n";
            continue;
        }

        test = true;
    } while (test == false);

    // Ajouter à la base de données.
    addPaysToDb(db, code, libelle, codeContinent);
}

void listSavedPays(sqlite3* db)
/****************************************************************
* Purpose : Lister les pays
****************************************************************/
{
    showAppTitle();

    const char* sql = "SELECT code_p, libelle_p, code_c FROM Pays;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Erreur de préparation : " << sqlite3_errmsg(db) << endl;
        return;
    }

    cout << "\nListe des Pays" << endl;
    bool showHeader(true);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        // Récupération des colonnes (0 pour le code, 1 pour le libellé)
        const unsigned char* code = sqlite3_column_text(stmt, 0);
        const unsigned char* libelle = sqlite3_column_text(stmt, 1);
        const unsigned char* codeContinent = sqlite3_column_text(stmt, 2);

        if (showHeader) {
			cout << "\n" << endl;
            cout << "CONTINENT | CODE | LIBELLÉ                " << endl;
            cout << "----------+------+------------------------" << endl;
            showHeader = false;
        }
        cout << (codeContinent ? reinterpret_cast<const char*>(codeContinent) : "NULL")
            << "       | "
            << (code ? reinterpret_cast<const char*>(code) : "NULL")
            << "  | "
            << (libelle ? reinterpret_cast<const char*>(libelle) : "NULL")
            << endl;
    }

    // Si la table était vide
    if (showHeader) {
        cout << "Aucun continent enregistré dans la base." << endl;
    }

    cout << endl;
    system("pause");

    // Destruction du statement pour libérer la mémoire (Crucial pour éviter les fuites)
    sqlite3_finalize(stmt);
}


void addVillageToDb(sqlite3* db, const string& code, const string& libelle, const string &codePays)
/****************************************************************
* Purpose : Insérer le nouveau village dans la base de données.
****************************************************************/
{
    const char* sql = "INSERT INTO Village (code_v, libelle_v, code_p) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt = nullptr;

    bool insertOk(true);
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        // Liaison des paramètres (les index commencent à 1)
        sqlite3_bind_text(stmt, 1, code.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, libelle.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, codePays.c_str(), -1, SQLITE_TRANSIENT);

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

void inputVillageData(sqlite3* db)
/****************************************************************
* Purpose : Ajouter un village => Contrôle de la saisie
****************************************************************/
{
    string code, libelle, codePays;
    bool test(false);

    showAppTitle();
    cout << "\nAjout d'un nouveau village" << endl;
    do {
        cout << "Saisir le code du pays (3 lettres) : ";
        cin >> codePays;
        if (codePays.empty() || codePays.length() != 3) {
            cin.clear();
            cin.ignore((numeric_limits<streamsize>::max)(), '\n');
            cout << "Entrée invalide. Réessayez.\n";
            continue;
        }

        // Nécessaire à cause de l'utilisation de \n ci-dessus"
        cin.ignore((numeric_limits<streamsize>::max)(), '\n');

        codePays = toUpperStr(codePays);
        // Check if code already exists
        bool isValueUnique = isUniqueValue(db, "Pays", "code_p", codePays);

        if (!isValueUnique) {
            cout << "Le pays " << codePays << " n'existe pas. Réessayez.\n";
            continue;
        }

        test = true;
    } while (test == false);

    test = false;
    do {
        cout << "Saisir le code (3 lettres) : ";
        cin >> code;
        if (code.empty() || code.length() != 3) {
            cin.clear();
            cin.ignore((numeric_limits<streamsize>::max)(), '\n');
            cout << "Entrée invalide. Réessayez.\n";
            continue;
        }

        // Nécessaire à cause de l'utilisation de \n ci-dessus"
        cin.ignore((numeric_limits<streamsize>::max)(), '\n');

        code = toUpperStr(code);
        // Check if code already exists
        bool isValueUnique = isUniqueValue(db, "Village", "code_v", code);

        if (isValueUnique) {
            cout << "Le code " << code << " existe déjà. Réessayez.\n";
            continue;
        }

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

        // Check if code already exists
        bool isValueUnique = isUniqueValue(db, "Village", "libelle_v", libelle);
        if (isValueUnique) {
            cout << "Le libellé " << libelle << " existe déjà. Réessayez.\n";
            continue;
        }

        test = true;
    } while (test == false);

    // Ajouter à la base de données.
    addVillageToDb(db, code, libelle, codePays);
}

void listSavedVillages(sqlite3* db)
/****************************************************************
* Purpose : Lister les villages
****************************************************************/
{
    showAppTitle();

    string sql =
        "SELECT V.code_v, V.libelle_v, P.libelle_p "
        "FROM Village V "
        "INNER JOIN Pays P ON V.code_p = P.code_p "
        "ORDER BY P.libelle_p ASC, V.libelle_v ASC;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Erreur de préparation : " << sqlite3_errmsg(db) << endl;
        system("pause");
        return;
    }

    cout << "\nListe des Villages" << endl;
    bool showHeader(true);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        string codeV = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        string libelleV = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        string libelleP = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));

        if (showHeader) {
			cout << "\n" << endl;
            cout << " CODE | VILLAGE                        | PAYS            " << endl;
            cout << "------+--------------------------------+-----------------" << endl;
            showHeader = false;
        }
        cout << " " << codeV << "  |";
        cout << libelleV;
        for (size_t i = libelleV.length(); i < 32; ++i) {
            cout << " ";
        }
        cout << " |" << libelleP << endl;
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
    if (db == nullptr) {
        cerr << "Erreur : La connexion a la base de donnees n'est pas ouverte !" << endl;
        return false;
    }

    bool result(true);
    char* errMsg = nullptr;
    string query_str;

    // (Par défaut, SQLite ignore les clauses FOREIGN KEY si on ne l'active pas à l'ouverture)
    const char* pragma_query = "PRAGMA foreign_keys = ON;";
    if (sqlite3_exec(db, pragma_query, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        cerr << "Impossible d'activer les contraintes de clés étrangères : " << errMsg << endl;
        sqlite3_free(errMsg);
        system("pause");
        return false;
    }

    // DÉBUT DE LA TRANSACTION
    if (sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, &errMsg) != SQLITE_OK) {
        cerr << "Impossible de démarrer la transaction : " << errMsg << endl;
        sqlite3_free(errMsg);
        system("pause");
        return false;
    }

    // Continent
    query_str =
        "CREATE TABLE IF NOT EXISTS Continent ( "
        "code_c TEXT, "
        "libelle_c TEXT NOT NULL, "
        "PRIMARY KEY(code_c), "
        "UNIQUE(libelle_c) "
        ");";
    if (sqlite3_exec(db, query_str.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) goto error_rollback;

    // Pays
    query_str =
        "CREATE TABLE IF NOT EXISTS Pays ( "
        "code_p TEXT, "
        "libelle_p TEXT NOT NULL, "
        "code_c TEXT NOT NULL, "
        "PRIMARY KEY(code_p), "
        "UNIQUE(libelle_p) "
        "FOREIGN KEY(code_c) REFERENCES Continent(code_c) "
        ");";
    if (sqlite3_exec(db, query_str.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) goto error_rollback;

    // Village
    query_str =
        "CREATE TABLE IF NOT EXISTS Village ( "
        "code_v TEXT, "
        "libelle_v TEXT NOT NULL, "
        "code_p TEXT NOT NULL, "
        "PRIMARY KEY(code_v), "
        "UNIQUE(libelle_v) "
        "FOREIGN KEY(code_p) REFERENCES Pays(code_p) "
        ");";
    if (sqlite3_exec(db, query_str.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) goto error_rollback;

    // Activité
    query_str =
        "CREATE TABLE IF NOT EXISTS Activite ( "
        "id_a INTEGER, "
        "libelle_a TEXT NOT NULL, "
        "PRIMARY KEY(id_a) "
        ");";
    if (sqlite3_exec(db, query_str.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) goto error_rollback;

	// Langue
    query_str =
        "CREATE TABLE IF NOT EXISTS Langue ( "
        "code_l TEXT, "
        "libelle_l TEXT NOT NULL, "
        "PRIMARY KEY(code_l), "
        "UNIQUE(libelle_l) "
        ");";
    if (sqlite3_exec(db, query_str.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) goto error_rollback;

    // Organisation
    query_str =
        "CREATE TABLE IF NOT EXISTS Organisation ( "
        "id_o TEXT, "
        "date_o TEXT NOT NULL, "
        "cout_o REAL NOT NULL, "
		"code_v TEXT NOT NULL, "
		"id_a INTEGER NOT NULL, "
        "PRIMARY KEY(id_o), "
        "FOREIGN KEY(code_v) REFERENCES Village(code_v), "
        "FOREIGN KEY(id_a) REFERENCES Activite(id_a) "
        ");";
    if (sqlite3_exec(db, query_str.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) goto error_rollback;

    // Avoir
    query_str =
        "CREATE TABLE IF NOT EXISTS Avoir ( "
        "code_p TEXT, "
        "code_v TEXT, "
        "PRIMARY KEY(code_p, code_v), "
        "FOREIGN KEY(code_p) REFERENCES Pays(code_p) "
        "FOREIGN KEY(code_v) REFERENCES Village(code_v) "
        ");";
    if (sqlite3_exec(db, query_str.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) goto error_rollback;

    // Parler
    query_str =
        "CREATE TABLE IF NOT EXISTS Parler ( "
        "code_v TEXT, "
        "code_l TEXT, "
        "PRIMARY KEY(code_v, code_l), "
        "FOREIGN KEY(code_v) REFERENCES Village(code_v), "
        "FOREIGN KEY(code_l) REFERENCES Langue(code_l) "
        ");";
    if (sqlite3_exec(db, query_str.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) goto error_rollback;

    // Index
    query_str =
        "CREATE INDEX IF NOT EXISTS idx_o_v_a ON Organisation (code_v, id_a); "
        "CREATE INDEX IF NOT EXISTS idx_p_code_c ON Pays (code_c); "
        "CREATE INDEX IF NOT EXISTS idx_v_code_p ON Village (code_p);";

    if (sqlite3_exec(db, query_str.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) goto error_rollback;


    // 3. VALIDATION DE LA TRANSACTION (Si tout s'est bien passé)
    if (sqlite3_exec(db, "COMMIT;", nullptr, nullptr, &errMsg) != SQLITE_OK) {
        cerr << "Erreur lors du Commit : " << errMsg << endl;
        sqlite3_free(errMsg);
        system("pause");
        return false;
    }

    cout << "Base de données initialisée avec succès (Transaction validée) !" << endl;
    return true;

    // GESTION DE L'ÉCHEC : Bloc d'annulation
error_rollback:
    cerr << "Erreur critique lors de l'initialisation : " << errMsg << endl;
    sqlite3_free(errMsg);

    cout << "Annulation des modifications (Rollback)..." << endl;
    sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr); // On efface tout ce qui a été fait dans cette session

    system("pause");
    return false;
}


int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

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