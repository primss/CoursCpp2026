#include "Village.h"
#include "Utilities.h"

void handleCreateVillage(Database &db, const VillageData &villageData)
/****************************************************************
* Purpose : Insérer le nouveau village dans la base de données.
****************************************************************/
{
    const char* sql = "INSERT INTO Village (code_v, libelle_v, code_p) VALUES (?, ?, ?);";
   
    // On regroupe les paramètres dans un tableau de chaînes
    std::vector<std::string> params = {
        villageData.code,
        villageData.libelle,
        villageData.codePays
    };

    // On délègue l'exécution à notre classe Database
    if (db.executePrepared(sql, params)) {
        std::cout << "\nVillage : " << villageData.code << " - " << villageData.libelle 
                  << " ajouté à la base de données." << std::endl;
        
        system("pause"); 
    }
}

void inputVillageData(Database &db)
/****************************************************************
* Purpose : Ajouter un village => Contrôle de la saisie
****************************************************************/
{
    std::string code, libelle, codePays;
    bool test(false);

    Utilities::showAppTitle();
    std::cout << "\nAjout d'un nouveau village" << std::endl;
    do {
        std::cout << "Saisir le code du pays (3 lettres) : ";
        std::cin >> codePays;
        if (codePays.empty() || codePays.length() != 3) {
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            std::cout << "Entrée invalide. Réessayez.\n";
            continue;
        }

        // Nécessaire à cause de l'utilisation de \n ci-dessus"
        std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

        codePays = Utilities::toUpperStr(codePays);
        // Check if code already exists
        bool isValueUnique = db.valueExists("Pays", "code_p", codePays);

        if (!isValueUnique) {
            std::cout << "Le pays " << codePays << " n'existe pas. Réessayez.\n";
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
        bool isValueUnique = db.valueExists("Village", "code_v", code);

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
        bool isValueUnique = db.valueExists("Village", "libelle_v", libelle);
        if (isValueUnique) {
            std::cout << "Le libellé " << libelle << " existe déjà. Réessayez.\n";
            continue;
        }

        test = true;
    } while (test == false);

    // Ajouter à la base de données.
    handleCreateVillage(db, {code, libelle, codePays});
}

void listSavedVillages(Database& db)
/****************************************************************
* Purpose : Lister les villages
****************************************************************/
{
    Utilities::showAppTitle();

    // Récupération de la liste typée
    std::vector<VillageData> villages = db.getAllVillages();

    std::cout << "\nListe des Villages" << std::endl;

    if (villages.empty()) {
        std::cout << "Aucun village enregistré dans la base." << std::endl;
        std::cout << std::endl;
        system("pause");
        return;
    }

    std::cout << "\n";
    std::cout << " CODE | VILLAGE                        | PAYS            " << std::endl;
    std::cout << "------+--------------------------------+-----------------" << std::endl;

    for (const auto& village : villages) {
        std::cout << " " << village.code << "  |";
        std::cout << village.libelle;

        for (size_t i = village.libelle.length(); i < 32; ++i) {
            std::cout << " ";
        }

        std::cout << " |" << village.libellePays << std::endl;
    }

    std::cout << std::endl;
    system("pause");
}