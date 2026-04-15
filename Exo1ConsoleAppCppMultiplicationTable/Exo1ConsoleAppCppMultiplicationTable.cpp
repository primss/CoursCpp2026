
#include <iostream>
#include <string>

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

int main()
{
	// Configurer la locale pour permettre l'affichage de caractères spéciaux
	std::setlocale(LC_ALL, "");
    std::cout << "Exercice 1 : Simulation de tables de multiplication!\n";
	std::cout << "Appuyez sur Entrée pour quitter sans saisir de valeur.\n";
	std::cout << "Entrez un nombre pour afficher sa table de multiplication: ";
	
	// Récupérer le nombre pour lequel afficher la table de multiplication
	int number = getInput();
	if (number < 0) {
		std::cout << "Veuillez entrer un nombre positif: ";
		return 0;
	}
	if (number == 0) {
		std::cout << "La table de multiplication de 0 est toujours 0.\n";
		return 0;
	}

	// Récupérer la plage de multiplication
	std::cout << "Veuillez entrer le nombre de début de la plage: ";
	int start = getInput();

	std::cout << "Veuillez entrer le nombre de fin de la plage: ";
	int end = getInput();
	while (end < start) {
		std::cout << "Le nombre de fin doit être supérieur ou égal au nombre de début. Veuillez réessayer: ";
		end = getInput();
	}

	// Afficher la table de multiplication
	std::cout << "Table de multiplication de " << number << " de " << start << " à " << end << ":\n";
	for (int i = start; i <= end; ++i) {
		std::cout << number << " x " << i << " = " << number * i << "\n";
	}
	return 0;
}