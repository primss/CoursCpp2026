
#include <iostream>
#include <windows.h>
#include <vector>
#include <clocale>

#include "Etudiant.h"

using namespace std;
using namespace std::chrono;

typedef unsigned int UInt;

UInt choixUtilisateur()
/**
    Purpose : Permettre le choix de l'utilisateur
**/
{
    UInt choix;
    cout << "Menu utilisateur :" << endl;
    cout << "1. Saisie des étudiants" << endl;
    cout << "2. Affichage de la liste des étudiants" << endl;
    cout << "0. Quitter" << endl;
    cout << "Entrez votre choix : ";
    cin >> choix;
    return choix;
} // UInt choixUtilisateur()


int main()
{
    SetConsoleOutputCP(CP_UTF8);

    vector<Etudiant> listEtudiants;
    UInt choix;

    do {
        system("CLS");
        cout << "SERVICE DES EXAMENS." << endl;
        choix = choixUtilisateur();
        switch (choix) {
            case 1:
                {
                    Etudiant e;
                    e.renseigner(listEtudiants.size() + 1);
                    listEtudiants.push_back(e);
                }
                break;
            case 2:
                {
                    string sortie = listEtudiants.size() == 0 ? "\nListe vide" : "\nListe des étudiants :";
                    std::cout << sortie << std::endl;
                    for (int i = 0; i < listEtudiants.size(); i++) {
                        listEtudiants[i].afficher(i + 1);
                    }
                }
                break;
            default:
                break;
        }
        if (choix != 0) {
            system("pause");
        }
    } while (choix != 0);

    return 0;
}