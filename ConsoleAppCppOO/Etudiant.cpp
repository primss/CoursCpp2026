#include "Etudiant.h"

#include <iostream>
#include <chrono>

Etudiant::Etudiant()
{
	NumeroCarte = 0;
	Nom = "";
	Prenom = "";
	Genre = GenreEnum::Male;
	DateNaissance = std::chrono::year(0)/0/0;
}

Etudiant::~Etudiant()
{
	
}

void Etudiant::renseigner(int position)
{
    std::cout << "--- Saisie d'un étudiant " << "n° " << position << "--- " << std::endl;
    std::cout << "Numero de carte : ";
    std::cin >> NumeroCarte;
    std::cout << "Nom : ";
    std::cin >> Nom;
    std::cout << "Prenom : ";
    std::cin >> Prenom;

    Genre = saisirGenre();
    DateNaissance = saisirDateValide();
}

void Etudiant::afficher(int position)
{
    std::string finMot = Genre == GenreEnum::Male ? "" : "e";
	std::cout << "Etudiant (" << position << "): " << Nom << " " << Prenom << " (Carte #" << NumeroCarte << ")" 
        << " né" << finMot << " le : " << (unsigned)DateNaissance.day() << " / "
		<< (unsigned)DateNaissance.month() << "/"
		<< (int)DateNaissance.year() << "\n" << std::endl;
}

/// <summary>
/// Fonction utilitaire pour saisir un genre valide
/// </summary>
/// <returns>GenderEnum</returns>
GenreEnum Etudiant::saisirGenre() 
{
    int choix;
    while (true) {
        std::cout << "Genre (0 pour Feminin, 1 pour Masculin) : ";

        if (!(std::cin >> choix)) { // Vérifie si l'utilisateur a saisi des nombres
            std::cout << "Erreur : Veuillez entrer un chiffre." << std::endl;
            std::cin.clear(); // Efface l'état d'erreur
            std::cin.ignore(1000, '\n'); // Vide le tampon
            continue;
        }

        // Vérifie si le nombre correspond à un choix valide
        if (choix == 0 || choix == 1) {
            return static_cast<GenreEnum>(choix);
        }
        else {
            std::cout << "Choix invalide. Veuillez entrer 0 pour Masculin ou 1 pour Feminin." << std::endl;
        }
    }
} // GenderEnum saisirGenre()

/// <summary>
/// Fonction utilitaire pour saisir une date valide
/// </summary>
/// <returns>year_month_day</returns>
std::chrono::year_month_day Etudiant::saisirDateValide() 
{
    int j, m, a;
    std::chrono::year_month_day d;

    while (true) {
        std::cout << "Date de naissance (Jour Mois Annee séparés par espaces) : ";
        if (!(std::cin >> j >> m >> a)) { // Vérifie si l'utilisateur a saisi des nombres
            std::cout << "Erreur : Veuillez entrer des chiffres uniquement." << std::endl;
            std::cin.clear(); // Efface l'état d'erreur
            std::cin.ignore(1000, '\n'); // Vide le tampon
            continue;
        }

        // Construction de la date
        d = std::chrono::year(a) / std::chrono::month(m) / std::chrono::day(j);

        // Vérification de la date
        if (d.ok()) {
            return d; // La date est valide, on sort de la fonction
        }
        else {
            std::cout << "Date invalide. Réessayez." << std::endl;
        }
    }
}
