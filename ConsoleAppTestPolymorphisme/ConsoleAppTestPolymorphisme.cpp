#include <iostream>
#include <vector>
#include <windows.h>

#include "Animal.h"
#include "Chien.h"
#include "Chat.h"

using namespace std;

int main()
{
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	// Création d'un tableau de pointeurs vers des animaux
	vector<Animal*> refuge;

	// Ajout de différents types d'animaux au refuge
	refuge.push_back(new Chien("Rox"));
	refuge.push_back(new Chat("Felix"));
	refuge.push_back(new Chien("Medor"));

	cout << "--- LE REFUGE S'EVEILLE ---" << endl;

	// Chaque animal fait du bruit (polymorphisme en action)
	for (const Animal* animal : refuge) {
		animal->faireDuBruit();
	}

	cout << "\n--- NETTOYAGE MEMOIRE ---" << endl;
	// Libération de la mémoire allouée pour les animaux
	for (Animal* animal : refuge) {
		delete animal;
	}

	refuge.clear(); // Vider le vecteur pour éviter les pointeurs pendants
	return 0;
}