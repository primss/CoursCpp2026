
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
    cout << "4. Comparer par égalité deux fractions" << endl;
    cout << "5. Additionner deux fractions" << endl;
    cout << "6. Soustraire deux fractions" << endl;
    cout << "7. Multiplier deux fractions" << endl;
    cout << "8. Diviser deux fractions" << endl;
    cout << "9. Simplifier les fractions" << endl;
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

void comparerDeuxFractions(vector<Fraction>& listFractions) {
    if (listFractions.size() < 2) {
        cout << "Info : Il faut au moins deux fractions dans la liste pour comparer." << endl;
    } 
    else if (listFractions.size() == 2) { // Comparaison directe
        comparerFractions(listFractions[0], listFractions[1]);
    }
    else {
        int f1, f2;
        afficherListeFractions(listFractions);
        cout << "Choisir les fractions à comparer par leur n° d'ordre séparé par espace : ";
        if (!(cin >> f1 >> f2)) {
            cout << "Erreur : les numéros d'ordre doivent des nombres";                    
        }
        if (f1 <= 0 || f1 > listFractions.size() || f2 <= 0 || f2 > listFractions.size()) {
            cout << "Erreur : les numéros d'ordre doivent être dans l'intervalle 1 à " << listFractions.size() << endl;
        }
        f1--;
        f2--;
        comparerFractions(listFractions[f1], listFractions[f2]);
    }
}

void faireUneOperation(vector<Fraction>& listFractions, UInt choix) {
    if (listFractions.size() < 2) {
        cout << "Info : Il faut au moins deux fractions dans la liste pour effectuer cette opération." << endl;
        return;
    }
    afficherListeFractions(listFractions);
    int f1, f2;
    cout << "Choisir les fractions à utiliser par leur n° d'ordre séparé par espace : ";
    if (!(cin >> f1 >> f2)) {
        cout << "Erreur : les numéros d'ordre doivent des nombres";
        return;
    }
    if (f1 <= 0 || f1 > listFractions.size() || f2 <= 0 || f2 > listFractions.size()) {
        cout << "Erreur : les numéros d'ordre doivent être dans l'intervalle 1 à " << listFractions.size() << endl;
        return;
    }
	f1--; // Convertir en index de tableau
	f2--; // Convertir en index de tableau
    Fraction result;
    switch (choix) {
	case 5: // addition
        result = listFractions[f1] + listFractions[f2];
        cout << listFractions[f1].toString() << " + " << listFractions[f2].toString() << " = " << result.toString() << endl;
        break;
	case 6: // soustraction
        result = listFractions[f1] - listFractions[f2];
        cout << listFractions[f1].toString() << " - " << listFractions[f2].toString() << " = " << result.toString() << endl;
        break;
	case 7: // multiplication
        result = listFractions[f1] * listFractions[f2];
        cout << listFractions[f1].toString() << " * " << listFractions[f2].toString() << " = " << result.toString() << endl;
        break;
	case 8: // division
        if (listFractions[f2].getNum() == 0) {
            cout << "Erreur : division par zéro !" << endl;
            return;
        }
        result = listFractions[f1] / listFractions[f2];
        cout << listFractions[f1].toString() << " / " << listFractions[f2].toString() << " = " << result.toString() << endl;
        break;
    default:
        cout << "Op ération non reconnue." << endl;
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
				comparerDeuxFractions(listFractions);
                break;
			case 5: // addition de deux fractions
            case 6: // soustraction de deux fractions
            case 7: // multiplication de deux fractions
            case 8: // division de deux fractions
                faireUneOperation(listFractions, choix);
                break;
            case 9: // simplification de toutes les fractions
                if (listFractions.size() == 0) {
                    cout << "Info : La liste est vide, il n'y a aucune fraction à simplifier." << endl;
                    break;
				}
                for (int i = 0; i < listFractions.size(); i++) {
                    listFractions[i].simplifier();
                }
                cout << "Opération de simplification terminée." << endl;
				break;
            default:
                break;
        }
        if (choix != 0) {
            system("pause");
        }
    } while (choix != 0);
}