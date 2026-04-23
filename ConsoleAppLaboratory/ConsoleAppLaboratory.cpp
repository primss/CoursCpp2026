
#include <iostream>
#include <string>
#include <vector>
#include <clocale>
#include <format>
#include <fstream>

using namespace std;

typedef struct TEtudiant TEtudiant;
struct TEtudiant {
    long numCarte;
    string nom;
    string prenom;
    float noteDST;
    float noteExam;
    float moyBrute;
    float bonus;
    float moyDef;
    string mention;
};

typedef unsigned int UInt;

string saisieStr(string field)
/**
    Purpose : saisir une donnée de type chaîne de caractères
**/
{
    string inputStr;
    do {
        cout << "Saisie " << field << " de l'étudiant: ";
        cin >> inputStr;
        if (inputStr.empty()) {
            cout << "Le champ " << field << " ne peut pas être vide. Veuillez réessayer." << endl;
        }
    } while (inputStr.empty());

    return inputStr;
} // string saisieStr(string field)


float saisieNote(string field)
/**
    Purpose : saisir une donnée de type numérique float
**/
{
    bool test;
    float inputNum;
    do {
        cout << "Saisie " << field << " de l'étudiant: ";
        cin >> inputNum;
        if (inputNum < 0) {
            test = false;
            cout << "Le champ " << field << " doit être un nombre positif. Veuillez réessayer." << endl;
        }
        else if (inputNum > 20) {
            test = false;
            cout << "Le champ " << field << " doit être compris entre 0 et 20. Veuillez réessayer." << endl;
        }
        else {
            test = true;
        }
    } while (!test);

    return inputNum;
} //float saisieNote(string field)


TEtudiant* saisieInfoEtudiant()
/**
    Purpose : saisir les informations d'un étudiant
**/
{
    TEtudiant* etudiant = new TEtudiant;
    cout << "Saisie des infos d'un nouvel étudiant :" << endl;
    cout << "Numéro de carte = ";
    cin >> etudiant->numCarte;
    etudiant->nom = saisieStr("nom");
    etudiant->prenom = saisieStr("prénoms");
    etudiant->noteDST = saisieNote("note DST");
    etudiant->noteExam = saisieNote("note d'examen");
    etudiant->bonus = 0;
    etudiant->moyBrute = 0;
    etudiant->moyDef = 0;

    //enregistrerEtudiant(etudiant)
    return etudiant;
} // TEtudiant saisieInfoEtudiant()

void enregistrerEtudiant(const TEtudiant& etu, const string& nomFichier) {
    // ios::app permet d'ajouter à la fin du fichier sans effacer l'existant
    ofstream fichier(nomFichier, ios::app);

    if (fichier.is_open()) {
        fichier << etu.numCarte << ";"
            << etu.nom << ";"
            << etu.prenom << ";"
            << etu.noteDST << ";"
            << etu.noteExam << ";"
            << etu.moyBrute << ";"
            << etu.bonus << ";"
            << etu.moyDef << ";"
            << etu.mention << endl;

        fichier.close();
        cout << "Etudiant enregistré avec succes !" << endl;
    }
    else {
        cerr << "Erreur d'ouverture du fichier." << endl;
    }
}

void saisieListeEtudiants(vector<TEtudiant*>& tab)
/**
    Purpose : saisir la liste des étudiants
**/
{
    char reponse;
    int counter(tab.size());
    TEtudiant* etd;
    cout << "Saisie de la liste des étudiants." << endl;

    do {
        cout << "N° d'ordre = " << counter + 1 << endl;
        etd = saisieInfoEtudiant();
        tab.push_back(etd);
        counter = tab.size();
        cout << "Voulez-vous continuer la saisie (O/N) ? ";
        cin >> reponse;
        reponse = toupper(reponse);
        getchar();
    } while (reponse == 'O');
} // void saisieListeEtudiants(vector<TEtudiant*> tab)


void showListEtudiants(vector<TEtudiant*> tab)
/**
    Purpose : afficher la liste des étudiants
**/
{
    cout << "Liste complète des étudiants." << endl;
    for (UInt i = 0; i < tab.size(); i++) {
        cout << i + 1 << "\t";
        cout << tab[i]->numCarte << "\t";
        cout << tab[i]->nom << "\t";
        cout << tab[i]->prenom << "\t";
        cout << tab[i]->noteDST << "\t";
        cout << tab[i]->noteExam << endl;
    }
} // void showListEtudiants(vector<TEtudiant*> tab)

void calcuMoyenneBrute(vector<TEtudiant*>& tab)
/**
    Purpose : Calcul de la moyenne brute
**/
{
    for (UInt i = 0; i < tab.size(); i++) {
        tab[i]->moyBrute = 0.45 * tab[i]->noteDST + 0.55 * tab[i]->noteExam;
    }
} // void calcuMoyenneBrute(vector<TEtudiant*> &tab)


void showResultats(vector<TEtudiant*> const& tab)
/**
    Purpose : afficher la liste des étudiants
**/
{
    cout << "Liste complète des étudiants." << endl;
    for (UInt i = 0; i < tab.size(); i++) {
        cout << i + 1 << "\t";
        cout << tab[i]->numCarte << "\t";
        cout << tab[i]->nom << "\t";
        cout << tab[i]->prenom << "\t";
        cout << tab[i]->noteDST << "\t";
        cout << tab[i]->noteExam << "\t";
        cout << tab[i]->moyBrute << "\t";
        cout << tab[i]->bonus << "\t";
        cout << tab[i]->moyDef << "\t";
        cout << tab[i]->mention << endl;
    }
} //void showResultats(vector<TEtudiant*> const &tab)

UInt choixUtilisateur()
/**
    Purpose : Permettre le choix de l'utilisateur
**/
{
    UInt choix;
    cout << "Menu utilisateur :" << endl;
    cout << "1. Saisie des informations" << endl;
    cout << "2. Affichage de la liste" << endl;
    cout << "3. Calcul moyenne brute" << endl;
    cout << "4. Calcul moyenne définitive" << endl;
    cout << "5. Calcul moyenne générale" << endl;
    cout << "6. Délibération" << endl;
    cout << "7. Affichage des résultats" << endl;
    cout << "0. Quitter" << endl;
    cout << "Entrez votre choix : ";
    cin >> choix;
    return choix;
} // UInt choixUtilisateur()


int main()
{
    vector<TEtudiant*> tabEtudiants;
    float moyGene;
    UInt choix;

    setlocale(LC_ALL, "French");

    do {
        system("CLS");
        cout << "Programme gestion des notes des étudiants." << endl;
        choix = choixUtilisateur();
        switch (choix) {
        case 1:
            saisieListeEtudiants(tabEtudiants);
            break;
        case 2:
            showListEtudiants(tabEtudiants);
            break;
        case 3:
            calcuMoyenneBrute(tabEtudiants);
            break;
        case 7:
            showResultats(tabEtudiants);
            break;
        }
        cout << "Taille totale = " << sizeof(tabEtudiants) << endl;
        system("pause");
    } while (choix != 0);

    return 0;
}
