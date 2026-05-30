#include "Pays.h"
#include "Utilities.h"

void handleCreatePays(Database& db, const PaysData& paysData)
/****************************************************************
* Purpose : Insérer le nouveau pays dans la base de données.
****************************************************************/
{
    const char* sql = "INSERT INTO Pays (code_p, libelle_p, code_c) VALUES (?, ?, ?);";

    // On regroupe les paramètres dans un tableau de chaînes
    std::vector<std::string> params = {
        paysData.code,
        paysData.libelle,
        paysData.codeContinent
    };

    // On délègue l'exécution à notre classe Database
    if (db.executePrepared(sql, params)) {
        std::cout << "\nPays : " << paysData.code << " - " << paysData.libelle
            << " ajouté à la base de données." << std::endl;

        system("pause");
    }
}

void inputPaysData(Database& db)
/****************************************************************
* Purpose : Ajouter un pays => Contrôle de la saisie
****************************************************************/
{
    std::string code, libelle, codeContinent;
    bool test(false);

    Utilities::showAppTitle();
    std::cout << "\nAjout d'un nouveau pays" << std::endl;
    do {
        std::cout << "Saisir le code du continent (3 lettres) : ";
        std::cin >> codeContinent;
        if (codeContinent.empty() || codeContinent.length() != 3) {
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            std::cout << "Entrée invalide. Réessayez.\n";
            continue;
        }

        // Nécessaire à cause de l'utilisation de \n ci-dessus"
        std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

        codeContinent = Utilities::toUpperStr(codeContinent);
        // Check if code already exists
        bool isValueUnique = db.valueExists("Continent", "code_c", codeContinent);

        if (!isValueUnique) {
            std::cout << "Le continent " << codeContinent << " n'existe pas. Réessayez.\n";
            continue;
        }

        test = true;
    } while (test == false);

    test = false;
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
        bool isValueUnique = db.valueExists("Pays", "code_p", code);

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
        bool isValueUnique = db.valueExists("Pays", "libelle_p", libelle);
        if (isValueUnique) {
            std::cout << "Le libellé " << libelle << " existe déjà. Réessayez.\n";
            continue;
        }

        test = true;
    } while (test == false);

    // Ajouter à la base de données.
    handleCreatePays(db, { code, libelle, codeContinent });
}

void listSavedPays(Database& db)
/****************************************************************
* Purpose : Lister les payss
****************************************************************/
{
    Utilities::showAppTitle();

    // Récupération de la liste typée
    std::vector<PaysData> payss = db.getAllPays();

    std::cout << "\nListe des Payss" << std::endl;

    if (payss.empty()) {
        std::cout << "Aucun pays enregistré dans la base." << std::endl;
        std::cout << std::endl;
        system("pause");
        return;
    }

    std::cout << "\n";
    std::cout << " CODE | PAYS                           | CONTINENT       " << std::endl;
    std::cout << "------+--------------------------------+-----------------" << std::endl;

    for (const auto& pays : payss) {
        std::cout << " " << pays.code << "  |";
        std::cout << pays.libelle;

        for (size_t i = pays.libelle.length(); i < 32; ++i) {
            std::cout << " ";
        }

        std::cout << " |" << pays.libelleContinent << std::endl;
    }

    std::cout << std::endl;
    system("pause");
}