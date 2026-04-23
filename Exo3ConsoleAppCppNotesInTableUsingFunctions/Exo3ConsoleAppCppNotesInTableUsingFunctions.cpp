
#include <iostream>
#include <string>
#include <ctime>

/// <summary>
/// Fonction pour déterminer si une chaîne de caractères représente un nombre.
/// </summary>
/// <param name="str">Chaîne de caractères à vérifier</param>
/// <returns>True si la chaîne représente un nombre, false sinon</returns>
bool IsNumber(const std::string& str) {
	bool isNumber = true;
	for (char const& c : str) {
		if (std::isdigit(c) == 0) {
			isNumber = false;
			break;
		}
	}
	return isNumber;
}

/// <summary>
/// Lit un entier depuis l'entrée standard en répétant la demande tant que la chaîne n'est pas un nombre valide.
/// </summary>
/// <returns>L'entier converti depuis la chaîne saisie (std::stoi). Retourne -1 si l'utilisateur saisit une ligne vide pour indiquer qu'il souhaite quitter.</returns>
int GetInput() {
	std::string input;
	std::getline(std::cin, input);
	while (!IsNumber(input)) {
		std::cout << "Veuillez entrer un nombre valide: ";
		std::getline(std::cin, input);
		if (input.empty()) {
			return -1; // Indique que l'utilisateur souhaite quitter
		}
	}
	return std::stoi(input);
}

/// <summary>
/// Fonction pour demander à l'utilisateur la taille du tableau de notes à saisir, avec validation pour s'assurer que la taille est un nombre positif. Si l'utilisateur entre une valeur non valide ou souhaite quitter, la fonction gère ces cas de manière appropriée.
/// </summary>
/// <returns>Entier représentant la taille du tableau</returns>
int GetTableSizeFromUser() {
	std::cout << "Veuillez entrer le nombre de notes à saisir: ";
	int size = GetInput();
	while (size <= 0) {
		std::cout << "Le nombre doit être positif.\n";
		std::cout << "Veuillez entrer le nombre de notes à saisir: ";
		size = GetInput();
	}

	return size;
}

/// <summary>
/// Invite l'utilisateur à choisir comment remplir le tableau (manuel ou automatique) et répète la demande jusqu'à obtenir un choix valide (1 ou 2).
/// </summary>
/// <returns>Le choix de l'utilisateur : 1 pour remplissage manuel, 2 pour remplissage automatique (valeurs aléatoires entre 0 et 20).</returns>
int GetTableFillChoiceFromUser() {
	int choice = -1;
	while (choice == -1) {
		std::cout << "Comment souhaitez-vous remplir le tableau ?\n";
		std::cout << "1. Manuellement\n";
		std::cout << "2. Automatiquement avec des valeurs aléatoires entre 0 et 20\n";
		std::cout << "Veuillez entrer votre choix (1 ou 2): ";
		choice = GetInput();
		if (choice != 1 && choice != 2) {
			std::cout << "Choix invalide.\n";
			GetTableFillChoiceFromUser();
		}
	}

	return choice;
}

/// <summary>
/// Point d'entrée du programme : saisit ou génère un tableau de notes, calcule la moyenne et affiche les notes supérieures à la moyenne.
/// </summary>
/// <returns>Code de sortie du programme. Renvoie 0 à la fin (y compris lors des sorties anticipées en cas d'erreur de saisie).</returns>
int main()
{
	std::setlocale(LC_ALL, "");
	std::cout << "Exercice 2 : Saisie de note dans un tableau avec calcul de la moyenne et détermination de notes supérieures à la moyenne calculée\n";
	std::cout << "Appuyez sur Entrée pour quitter sans saisir de valeur.\n";

	int size = GetTableSizeFromUser();

	int choice = GetTableFillChoiceFromUser();

	int* notes = new int[size];
	switch (choice)
	{
		case 1:
			for (int i = 0; i < size; ++i) {
				std::cout << "Veuillez entrer la note " << (i + 1) << ": ";
				std::string input;
				std::getline(std::cin, input);
				if (input.empty()) {
					std::cout << "Aucune valeur saisie. Fin du programme.\n";
					delete[] notes;
					return 0;
				}
				try {
					int note = std::stoi(input);
					notes[i] = note;
					if (notes[i] < 0 || notes[i] > 20) {
						std::cout << "La note doit être entre 0 et 20.\n";
						delete[] notes;
						return 0;
					}
				}
				catch (const std::invalid_argument&) {
					std::cout << "Veuillez entrer un nombre valide pour la note.\n";
					delete[] notes;
					return 0;
				}
				catch (const std::out_of_range&) {
					std::cout << "Le nombre entré est trop grand pour être traité.\n";
					delete[] notes;
					return 0;
				}
			}
			break;
		case 2: {
			std::srand(static_cast<unsigned int>(std::time(nullptr)));
			for (int i = 0; i < size; ++i) {
				notes[i] = std::rand() % 21; // Génère une note entre 0 et 20
			}
			break;
		}
		default:
			break;
	}

	std::string tableValue;
	int tableTotal = 0;
	for (int i = 0; i < size; ++i) {
		// Convertit l'entier en string et l'ajoute
		tableValue += std::to_string(notes[i]);

		// Ajoute la virgule et l'espace, sauf après le dernier élément
		if (i < size - 1) {
			tableValue += ", ";
		}
		tableTotal += notes[i];
	}
	std::cout << "Notes saisies: " << tableValue << "\n";

	int average = tableTotal / size;
	std::cout << "Moyenne des notes: " << average << "\n";

	// Affichage des notes supérieures à la moyenne
	std::string tableNotesHigherThanAverage = "";
	int count = 0;
	for (int i = 0; i < size; i++)
	{
		if (notes[i] > average)
		{
			// Convertit l'int en string et l'ajoute
			tableNotesHigherThanAverage += std::to_string(notes[i]);

			// Ajoute la virgule et l'espace, sauf après le dernier élément
			if (i < size - 1) {
				tableNotesHigherThanAverage += ", ";
			}
			count++;
		}
	}

	std::cout << "Notes supérieures à la moyenne: " << "(" << count << "): " << tableNotesHigherThanAverage << "\n";
	return 0;
}