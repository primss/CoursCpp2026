#include <iostream>

#include "Utilities.h"

void Utilities::showAppTitle()
/****************************************************************
* Purpose : Afficher le titre de l'application
****************************************************************/
{
    system("CLS");
    std::cout << "===========================================================" << std::endl;
    std::cout << "|                      CLUB DE VACANCES                   |" << std::endl;
    std::cout << "|                        GCV v0.0.1                       |" << std::endl;
    std::cout << "===========================================================" << std::endl;
}

std::string Utilities::toUpperStr(std::string str)
/****************************************************************
* Purpose : Mettre une chaîne de caractères en majuscules
****************************************************************/
{
    // On parcourt la chaîne caractère par caractère grâce à une référence (&c)
    for (char& c : str) {
        c = std::toupper(static_cast<unsigned char>(c)); // On applique toupper sur CHAQUE lettre
    }
    return str;
}