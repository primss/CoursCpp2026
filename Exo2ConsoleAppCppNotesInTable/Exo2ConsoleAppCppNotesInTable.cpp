
#include <iostream>
#include <string>

int main()
{
	std::setlocale(LC_ALL, "");
	std::cout << "Exercice 2 : Saisie de note dans un tableau avec calcul de la moyenne et détermination de notes supérieures à la moyenne calculée\n";
	std::cout << "Appuyez sur Entrée pour quitter sans saisir de valeur.\n";

	std::string input;

	std::cout << "Veuillez entrer le nombre de notes à saisir: ";
	std::getline(std::cin, input);
	if (input.empty()) {
		std::cout << "Aucune valeur saisie. Fin du programme.\n";
		return 0;
	}

	int size = 0;
	try {
		size = std::stoi(input);
		if (size <= 0) {
			std::cout << "Le nombre de notes doit être un nombre positif.\n";
			return 0;
		}
	}
	catch (const std::invalid_argument&) {
		std::cout << "Veuillez entrer un nombre de notes valide.\n";
		return 0;
	}
	catch (const std::out_of_range&) {
		std::cout << "Le nombre entré est trop grand pour être traité.\n";
		return 0;
	}

	int choice = -1;
	while (choice == -1) {
		std::cout << "Comment souhaitez-vous remplir le tableau ?\n";
		std::cout << "1. Manuellement\n";
		std::cout << "2. Automatiquement avec des valeurs aléatoires entre 0 et 20\n";
		std::cout << "Veuillez entrer votre choix (1 ou 2): ";
		std::getline(std::cin, input);
		if (input.empty()) {
			std::cout << "Aucune valeur saisie. Fin du programme.\n";
			return 0;
		}
		try {
			choice = std::stoi(input);
		}
		catch (const std::invalid_argument&) {
			std::cout << "Choix invalide.\n";
		}
		catch (const std::out_of_range&) {
			std::cout << "Le nombre entré est trop grand pour être traité.\n";
		}

		if (choice != 1 && choice != 2) {
			std::cout << "Choix invalide.\n";
		}
	}

	int* notes = new int[size];
	switch (choice)
	{
		case 1:
			for (int i = 0; i < size; ++i) {
				std::cout << "Veuillez entrer la note " << (i + 1) << ": ";
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

	std::string tableValue = "";
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