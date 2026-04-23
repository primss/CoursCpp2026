
#include "Fraction.h"

#include <iostream>
#include <windows.h>
#include <vector>
#include <random>

using namespace std;

typedef unsigned int UInt;

UInt choixUtilisateur()
/*********************************************
* Purpose : Permettre le choix de l'utilisateur
**********************************************/
{
    UInt choix;
    cout << "Menu utilisateur :" << endl;
    cout << "1. Saisie des fractions" << endl;
    cout << "2. Génération automatique de 10 fractions" << endl;
    cout << "3. Affichage des fractions" << endl;
    cout << "4. Comparer deux fractions" << endl;
    cout << "5. Additionner deux fractions" << endl;
    cout << "6. Soustraire deux fractions" << endl;
    cout << "7. Multiplier deux fractions" << endl;
    cout << "0. Quitter" << endl;
    cout << "Entrez votre choix : ";
    cin >> choix;
    return choix;
} // UInt choixUtilisateur()

void afficherListeFractions(vector<Fraction>& listFractions) {
    cout << (listFractions.size() == 0 ? "\nListe vide" : "\nListe des fractions :") << endl;
    for (int i = 0; i < listFractions.size(); i++) {
        listFractions[i].afficher(i + 1);
    }
}

void comparerFractions(Fraction& f1, Fraction& f2) {
    bool comparaison = f1 == f2;
    cout << f1.toString() << (comparaison ? " est" : " n'est pas") << " égal à " << f2.toString() << endl;
}

void genererFractionsAleatoire(vector<Fraction>& listFractions)
{
    random_device rd;
    std::mt19937 gen(rd());

    // Définir la plage [min, max]
    std::uniform_int_distribution<> dis(-999, 999);

    // Générer le nombre
    for (int i = 0; i < 10; i++) {
        long numAleatoire = dis(gen);
        long denAleatoire = dis(gen);
        Fraction f(numAleatoire, denAleatoire);
        listFractions.push_back(f);
    }
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    vector<Fraction> listFractions;
    UInt choix;
    bool comparaison;
    int f1, f2;

    do {
        system("CLS");
        cout << "S'AMUSER AVEC LES FRACTIONS." << endl;
        choix = choixUtilisateur();
        switch (choix) {
            case 1: // Saisie de fractions
                {
                    Fraction e;
                    e.creer(listFractions.size() + 1);
                    listFractions.push_back(e);
                }
                break;
            case 2:
                genererFractionsAleatoire(listFractions);
                cout << "Opération de génération aléatoire terminée." << endl;
                break;
            case 3:
                afficherListeFractions(listFractions);
                break;
            case 4: // comparaison par égalité de fraction
                if (listFractions.size() < 2) {
                    cout << "Info : Il faut au moins deux fractions dans la liste pour comparer." << endl;
                } 
                else if (listFractions.size() == 2) { // Comparaison directe
                    comparerFractions(listFractions[0], listFractions[1]);
                }
                else {
                    afficherListeFractions(listFractions);
                    cout << "Choisir les fractions à comparer par leur n° d'ordre séparé par espace : ";
                    if (!(cin >> f1 >> f2)) {
                        cout << "Erreur : les numéros d'ordre doivent des nombres";
                        break;
                    }
                    if (f1 <= 0 || f1 > listFractions.size() || f2 <= 0 || f2 > listFractions.size()) {
                        cout << "Erreur : les numéros d'ordre doivent être dans l'intervalle 1 à " << listFractions.size() << endl;
                        break;
                    }
                    f1--;
                    f2--;
                    comparerFractions(listFractions[f1], listFractions[f2]);
                }
                break;
            default:
                break;
        }
        if (choix != 0) {
            system("pause");
        }
    } while (choix != 0);
}