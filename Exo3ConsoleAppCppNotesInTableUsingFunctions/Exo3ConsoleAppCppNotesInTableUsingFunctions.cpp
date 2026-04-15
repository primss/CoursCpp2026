
#include <iostream>
#include <string>
#include <ctime>

// Déterminer si l'input est un nombre
bool isNumber(const std::string& str) {
	for (char const& c : str) {
		if (std::isdigit(c) == 0) return false;
	}
	return true;
}

// Valider et récupérer l'input
int getInput() {
	std::string input;
	std::getline(std::cin, input);
	while (!isNumber(input)) {
		std::cout << "Veuillez entrer un nombre valide: ";
		std::getline(std::cin, input);
		if (input.empty()) {
			return -1; // Indique que l'utilisateur souhaite quitter
		}
	}
	return std::stoi(input);
}

int GetTableSizeFromUser() {
	std::cout << "Veuillez entrer le nombre de notes à saisir: ";
	int size = getInput();
	while (size <= 0) {
		std::cout << "Le nombre doit être positif.\n";
		std::cout << "Veuillez entrer le nombre de notes à saisir: ";
		size = getInput();
	}

	return size;
}

int GetTableFillChoiceFromUser() {
	int choice = -1;
	while (choice == -1) {
		std::cout << "Comment souhaitez-vous remplir le tableau ?\n";
		std::cout << "1. Manuellement\n";
		std::cout << "2. Automatiquement avec des valeurs aléatoires entre 0 et 20\n";
		std::cout << "Veuillez entrer votre choix (1 ou 2): ";
		choice = getInput();
		if (choice != 1 && choice != 2) {
			std::cout << "Choix invalide.\n";
			GetTableFillChoiceFromUser();
		}
	}

	return choice;
}

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