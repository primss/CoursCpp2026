#include "Continent.h"
#include "Utilities.h"

void handleCreateContinent(Database& db, const ContinentData& continentData)
/****************************************************************
* Purpose : Insérer le nouveau continent dans la base de données.
****************************************************************/
{
    const char* sql = "INSERT INTO Continent (code_c, libelle_c) VALUES (?, ?);";

    // On regroupe les paramètres dans un tableau de chaînes
    std::vector<std::string> params = {
        continentData.code,
        continentData.libelle
    };

    // On délègue l'exécution à notre classe Database
    if (db.executePrepared(sql, params)) {
        std::cout << "\nContinent : " << continentData.code << " - " << continentData.libelle
            << " ajouté à la base de données." << std::endl;

        system("pause");
    }
}

void inputContinentData(Database& db)
/****************************************************************
* Purpose : Ajouter un continent => Contrôle de la saisie
****************************************************************/
{
    std::string code, libelle;
    bool test(false);

    Utilities::showAppTitle();

    do {
        std::cout << "Saisir le code (3 lettres) : ";
        std::cin >> code;
        if (code.empty() || code.length() != 3) {
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            std::cout << "Entrée invalide. Réessayez.\n";
            continue;
        }

        // Nécessaire à cause de l'utilisation de \n ci-dessus"
        std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

        code = Utilities::toUpperStr(code);
        // Check if code already exists
        bool isValueUnique = db.valueExists("Continent", "code_c", code);

        if (isValueUnique) {
            std::cout << "Le code " << code << " existe déjà. Réessayez.\n";
            continue;
        }

        test = true;
    } while (test == false);

    test = false;
    do {
        std::cout << "Saisir le libelle (30 caratères max) :";
        std::getline(std::cin, libelle); // cin s'arrête à espace, retour à la ligne...

        if (libelle.empty()) {
            std::cout << "Entrée requise. Réessayez.\n";
            continue;
        }
        else if (libelle.length() > 30) {
            std::cout << "Entrée trop long. Réessayez.\n";
            continue;
        }

        // Check if code already exists
        bool isValueUnique = db.valueExists("Continent", "libelle_c", libelle);
        if (isValueUnique) {
            std::cout << "Le libellé " << libelle << " existe déjà. Réessayez.\n";
            continue;
        }

        test = true;
    } while (test == false);

    // Ajouter à la base de données.
    handleCreateContinent(db, { code, libelle });
}

void listSavedContinents(Database& db)
/****************************************************************
* Purpose : Lister les continents
****************************************************************/
{
    Utilities::showAppTitle();

    // Récupération de la liste typée
    std::vector<ContinentData> continents = db.getAllContinents();

    std::cout << "\nListe des Continents" << std::endl;

    if (continents.empty()) {
        std::cout << "Aucun continent enregistré dans la base." << std::endl;
        std::cout << std::endl;
        system("pause");
        return;
    }

    std::cout << "\n";
    std::cout << " CODE | NOM                            " << std::endl;
    std::cout << "------+--------------------------------" << std::endl;

    for (const auto& continent : continents) {
        std::cout << " " << continent.code << "  |";
        std::cout << continent.libelle;

        for (size_t i = continent.libelle.length(); i < 32; ++i) {
            std::cout << " ";
        }

        std::cout << "" << std::endl;
    }

    std::cout << std::endl;
    system("pause");
}