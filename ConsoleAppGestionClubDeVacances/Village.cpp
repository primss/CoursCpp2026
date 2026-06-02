#include "Village.h"
#include "Utilities.h"
#include "PaysRepository.h"

/****************************************************************
* Purpose : Permettre à l'utilisateur de choisir l'action à faire
****************************************************************/
UInt selectActionMenu()
{
    UInt choix;
    std::cout << "\nSélectionner une action à effectuer sur les Villages" << std::endl;
    std::cout << "1. Ajouter" << std::endl;
    std::cout << "2. Lister" << std::endl;
    std::cout << "3. Modifier" << std::endl;
    std::cout << "4. Supprimer" << std::endl;
    std::cout << "0. Quitter" << std::endl;

    std::cout << "Entrer votre choix : ";
    if (!(std::cin >> choix)) {
        std::cin.clear();
        std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
        choix = 999; // Retourner un choix invalide pour déclencher le switch par défaut
    }
    return choix;
} // UInt selectActionMenu()

/****************************************************************
* Purpose : Permettre à l'utilisateur de choisir l'action à faire
****************************************************************/
void handleVillageActions(Database& db)
{
    UInt choix;

    do {
        Utilities::showAppTitle();
        choix = selectActionMenu();
        switch (choix) {
        case 0:
            std::cout << "\nFin de l'application !" << std::endl;
            break;
        case 1:
			inputVillageData(db);
            break;
        case 2:
            listSavedVillages(db);
            break;
        case 3:
            inputVillageData(db, false);
            break;
        case 4:
			handleDeleteVillage(db);
            break;
        default:
            std::cout << "Choix invalide." << std::endl;
            system("pause");
            break;
        }
    } while (choix != 0);
}

/****************************************************************
* Purpose : Insérer le nouveau village dans la base de données.
****************************************************************/
void handleCreateVillage(Database &db, const VillageData &villageData)
{
    // Appel du Repository
    VillageRepository villageRepo;

    try {
        auto result = villageRepo.create(db, villageData);
        // On récupère l'objet créé si besoin (ex: resultatOpt->libelle)
        std::cout << "\nVillage : " << result.getCode() << " - " << result.getLibelle()
            << " ajoute avec succes a la base de donnees !" << std::endl;
    }
    catch (const std::runtime_error& e) {
        std::cerr << "\nErreur critique : Impossible d'ajouter le village dans la base de donnees." << std::endl;
        std::cerr << "\nRaison : " << e.what() << std::endl;
	}

    // Le system("pause") global est centralisé ici, à la fin de l'interaction utilisateur
    system("pause");
}

/****************************************************************
* Purpose : Mettre à jour le code ET les données d'un village.
****************************************************************/
void handleUpdateVillageCodeAndData(Database& db, const std::string& ancienCode, const VillageData& newData)
{
    // Appel du Repository
    VillageRepository villageRepo;

    try {
        bool result = villageRepo.update(db, ancienCode, newData);
        if (result) {
            std::cout << "\nVillage mis à jour avec succès !" << std::endl;
            if (ancienCode != newData.getCode()) {
                std::cout << "L'ancien code '" << ancienCode << "' a été remplacé par '" << newData.getCode() << "'.\n";
            }
        }
    }
    catch (const std::runtime_error& e) {
        std::cerr << "\nErreur critique : Impossible d'ajouter le village dans la base de donnees." << std::endl;
        std::cerr << "\nRaison : " << e.what() << std::endl;
    }

    // Le system("pause") global est centralisé ici, à la fin de l'interaction utilisateur
    system("pause");
}

/****************************************************************
* Purpose : Ajouter un village => Contrôle de la saisie
****************************************************************/
void inputVillageData(Database &db, bool onCreate)
{
    std::string code, libelle, codePays, oldCode;
    bool test(false);
	VillageRepository villageRepo;
	PaysRepository paysRepo;

    Utilities::showAppTitle();

    if (!onCreate) { // Récupérer le code du village à modifier
        std::cout << "\nModification d'un village" << std::endl;
        oldCode = getVillageCode(db);
        if (oldCode.empty()) {
            std::cout << "Aucun village sélectionné pour la modification." << std::endl;
            return; // Sortir de la fonction pour revenir au menu principal
		}
        std::cout << "\nVillage sélectionné : " << oldCode << std::endl;
    }
    else {
        std::cout << "\nAjout d'un nouveau village" << std::endl;
    }

    do {
        std::cout << "Saisir le code du pays (3 lettres) ou 'Q' pour quitter : ";
        std::cin >> codePays;
        if (codePays == "Q" || codePays == "q") {
            std::cout << "Abandon de l'ajout du village." << std::endl;
            return; // Sortir de la fonction pour revenir au menu principal
		}
        if (codePays.empty() || codePays.length() != 3) {
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            std::cout << "Entrée invalide. Réessayez.\n";
        } 
        else {
            // Nécessaire à cause de l'utilisation de \n ci-dessus"
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

            codePays = Utilities::toUpperStr(codePays);
            // Check if code already exists
            bool isValueUnique = paysRepo.valueExists(db, "code_p", codePays);

            if (!isValueUnique) {
                std::cout << "Le pays " << codePays << " n'existe pas. Réessayez.\n";
            }
            else {
                test = true;
            }
        }
    } while (test == false);

    test = false;
    do {
        std::cout << "Saisir le code du village (3 lettres) ou 'Q' pour quitter : ";
        std::cin >> code;
        if (code == "Q" || code == "q") {
            std::cout << "Abandon de l'ajout du village." << std::endl;
            return; // Sortir de la fonction pour revenir au menu principal
        }
        if (code.empty() || code.length() != 3) {
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            std::cout << "Entrée invalide. Réessayez.\n";
        }
        else {
            // Nécessaire à cause de l'utilisation de \n ci-dessus"
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

            code = Utilities::toUpperStr(code);
            if (code == oldCode || !villageRepo.valueExists(db, "code_v", code)) {
                test = true;
            }
            else {
                std::cout << "Le code " << code << " existe déjà. Réessayez.\n";
            }
        }
    } while (test == false);

    test = false;
    do {
        std::cout << "Saisir le libelle du village (30 caratères max) ou 'Q' pour quitter : ";
        std::getline(std::cin, libelle); // cin s'arrête à espace, retour à la ligne...
        if (libelle == "Q" || libelle == "q") {
            std::cout << "Abandon de l'ajout du village." << std::endl;
            return; // Sortir de la fonction pour revenir au menu principal
		}
        if (libelle.empty()) {
            std::cout << "Entrée requise. Réessayez.\n";
        }
        else if (libelle.length() > 30) {
            std::cout << "Entrée trop longue. Réessayez.\n";
        }
        else {
            // Check if code already exists
            bool isValueUnique = villageRepo.valueExists(db, "libelle_v", libelle);
            if (isValueUnique) {
                std::cout << "Le libellé " << libelle << " existe déjà. Réessayez.\n";
            }
            else {
                test = true;
            }
        }
    } while (test == false);

    if (onCreate) {
        // Ajouter à la base de données.
        handleCreateVillage(db, { code, libelle, codePays });
    }
    else {
		handleUpdateVillageCodeAndData(db, oldCode, { code, libelle, codePays });
    }
}

/****************************************************************
* Purpose : Lister les villages
****************************************************************/
void listSavedVillages(Database& db)
{
    Utilities::showAppTitle();

	VillageRepository villageRepo;
    // Récupération de la liste typée
    std::vector<VillageData> villages = villageRepo.getAll(db);

    std::cout << "\nListe des Villages" << std::endl;

    if (villages.empty()) {
        std::cout << "Aucun village enregistré dans la base." << std::endl;
        std::cout << std::endl;
        system("pause");
    }
    else {
        std::cout << "\n";
        std::cout << " CODE | VILLAGE                        | PAYS            " << std::endl;
        std::cout << "------+--------------------------------+-----------------" << std::endl;

        for (const auto& village : villages) {
            std::cout << " " << village.getCode() << "  |";
            std::cout << village.getLibelle();

            for (size_t i = village.getLibelle().length(); i < 32; ++i) {
                std::cout << " ";
            }

            std::cout << " |" << village.getPays().getLibelle() << std::endl;
        }

        std::cout << std::endl;
        system("pause");
    }
}

void handleDeleteVillage(Database& db)
{
	std::string code;
	bool test(false);
	bool result(false);
    VillageData villageData;
	VillageRepository villageRepo;

    Utilities::showAppTitle();
    std::cout << "\nSuppression d'un village" << std::endl;

    do {
        std::cout << "Saisir le code du village (3 lettres) ou 'Q' pour quitter : ";
        std::cin >> code;
        if (code == "Q" || code == "q") {
            return; // Sortir de la fonction pour revenir au menu principal
        }
        if (code.empty() || code.length() != 3) {
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            std::cout << "Entrée invalide. Réessayez.\n";
        }
        else {
            // Nécessaire à cause de l'utilisation de \n ci-dessus"
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
			code = Utilities::toUpperStr(code);
            villageData = villageRepo.read(db, code);
            if (villageData.getCode().empty()) {
                std::cout << "Le village " << code << " n'existe pas. Réessayez.\n";
            }
            else {
                test = true;
				result = villageRepo.destroy(db, code);
				if (result) {
					std::cout << "Village " << code << " supprimé avec succès !" << std::endl;
                    system("pause");
				}
            }
        }
    } while (test == false);
}

std::string getVillageCode(Database& db)
{
    std::string code;
    bool test(false);
    VillageRepository villageRepo;

    do {
        std::cout << "Saisir le code du village (3 lettres) ou 'Q' pour quitter : ";
        std::cin >> code;
        if (code != "Q" && code != "q") {
            if (code.empty() || code.length() != 3) {
                std::cin.clear();
                std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
                std::cout << "Entrée invalide. Réessayez.\n";
            }
            else {
                // Nécessaire à cause de l'utilisation de \n ci-dessus"
                std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
                code = Utilities::toUpperStr(code);
                test = villageRepo.valueExists(db, "code_v", code);
                if (!test) {
                    std::cout << "Le village " << code << " n'existe pas. Réessayez.\n";
                }
            }
        }
        
    } while (test == false);

    return code;
}